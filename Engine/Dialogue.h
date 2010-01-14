/*---------------------------------------------------------------------------
 Author:  thoronador
 Date:    2010-01-10
 Purpose: Dialogue Singleton class
          holds all information about dialogues with NPCs

 History:
     - 2010-01-10 (rev 151) - initial version (by thoronador)
     - 2010-01-13 (rev 152) - greetings improved/ changed to be dependent on
                              the NPC's ID; documentation updated

 ToDo list:
     - extend class for more conditions
     - extension for result scripts
        - execution of result scripts on demand
     - extension for audio files of speech
     - ???

 Bugs:
     - No known bugs, but class is currently untested. If you find one (or more)
       bugs, then tell me please.
 --------------------------------------------------------------------------*/

#ifndef DIALOGUE_H
#define DIALOGUE_H

#include <string>
#include <map>
#include <vector>
#include <fstream>

#include "NPC.h"
#include "DuskTypes.h"

namespace Dusk
{

class Dialogue
{
  public:
    /*record for holding dialogue conditions

        Conditions:
            NPC_ID - has to match the ID of the speaking NPC, if not empty
                     string (""). Empty string matches every NPC.
    */
    struct ConditionRecord
    {
       std::string NPC_ID;
       //more to come
    };

    /* Type used internally to keep all data of a given dialogue line  */
    struct LineRecord
    {
       std::string Text;
       //std::string SpeechFile; //maybe used later for audio
       ConditionRecord Conditions; //conditions for showing that line
       std::vector<std::string> Choices; //IDs of options the player can choose
       //Script ResultScript; //maybe used later for the script which is
                              // executed when option was chosen/ displayed

      /* Tries to save the LineRecord to the given stream and returns true on
         success, false on failure.
      */
      bool SaveToStream(std::ofstream& out) const;

      /* Tries to load the LineRecord from the given stream and returns true on
         success, false on failure. If that function failed, the data within
         this record is most likely to be corrupt.
      */
      bool LoadFromStream(std::ifstream& inp);
    };

    /* Dialogue::Handle record. Contains all neccessary data for dialogue lines.
       LineID is the ID of the current line, Text is (you guessed it) the actual
       text of the dialogue line, and Choices is a vector containing the IDs(!)
       of the answers the player can choose from.

       See descriptions of GetGreetingLine() for information on how it is used.
    */
    struct Handle
    {
      std::string LineID;
      std::string Text;
      std::vector<std::string> Choices;
    };

    /* destructor */
    virtual ~Dialogue();

    /* Singleton access method */
    static Dialogue& GetSingleton();

    /* Clears ALL dialogue data. Use with caution, or better, not at all. */
    void ClearData();

    /* returns the dialogue handle of the greeting line for the given NPC

        parameter:
            who - pointer to the NPC the character is talking to. You can pass
                  a NULL pointer, if you want, but this will only get you a
                  generic line or no line at all.

        return value:
            The Dialogue::Handle of the NPC's line. If LineID and Text are empty
            strings and length(Choices) is zero, then no greeting was found,
            i.e. there is no dialogue for that NPC. If LineID and Text are NOT
            empty, but the size of Choices is zero, then this indicates the end
            of the conversation.

        remarks:
            During the dialogue, this function should only be called once at
            the start of the conversation. Subsequent lines have to be obtained
            by calling GetDialogueLine().
    */
    Handle GetGreetingLine(const NPC* who) const;

    /* returns the dialogue handle of the dialogue line with the ID LineID.

       parameters:
           LineID - ID of the line
           who    - pointer to the speaking NPC. You can pass a NULL pointer,
                    if you want, but this will only get you a generic line or
                    no dialogue line at all.
       return value:
           The Dialogue::Handle of the NPC's line. See GetGreetingLine() for
           more information on that.

       remarks:
           This function provides the usual way to obtain dialogue data, it
           should be called every time, when you want to get (more) dialogue
           data. The only exception is the start of a dialogue - in this case
           call GetGreetingLine(), because greetings are handled a bit different
           other dialogue lines.
    */
    Handle GetDialogueLine(const std::string& LineID, const NPC* who) const;

    /* Returns the text of line LineID, or an empty string, if no such line was
       found.
    */
    std::string GetText(const std::string& LineID) const;

    /* Returns the ConditionRecord of the line with the ID LineID */
    ConditionRecord GetCondition(const std::string& LineID) const;

    /* returns true, if there is a dialogue line with the given ID */
    bool HasDialogueLine(const std::string& LineID) const;

    /* Adds a list of greeting lines for NPCs with the ID NPC_ID

       parameters:
           NPC_ID  - ID of the NPC. If empty string, the greeting will be
                     available for all NPCs, regardless of their ID.
           Choices - Vector of the IDs of the dialogue lines available for a
                     greeting
    */
    void AddGreeting(const std::string& NPC_ID, const std::vector<std::string>& Choices);

    /* Adds a new dialogue line with ID LineID and data lr.

       remarks:
           This function always succeeds, unless LineID is an empty string. In
           that case, nothing happens. If there already is a line with ID
           LineID, this line will be replaced.
    */
    void AddLine(const std::string& LineID, const LineRecord& lr);

    /* Tries to save ALL dialogue data to the given stream. Returns true on
       success, or false if an error occured.
    */
    bool SaveToStream(std::ofstream& output) const;

    /* tries to load the next dialogue record (dialogue or greeting) from the
       given stream. Returns true on success, false otherwise.
    */
    bool LoadNextRecordFromStream(std::ifstream& input);

    /* returns the number of dialogue lines, including greetings (which might
       be a bit misleading at some point)
    */
    unsigned int NumberOfLines() const;
  private:
    /* constructor */
    Dialogue();

    /* private copy constructor - there can only be one */
    Dialogue(const Dialogue& op) {}

    /* Checks whether all conditions in ConditionRecord cond are met by NPC who.
       Returns true, if all conditions are met, returns false otherwise.
    */
    bool ConditionFulfilled(const ConditionRecord& cond, const NPC* who) const;

    /* flags to indicate type of dialogue data when reading from or writhing to
       a stream
    */
    static const uint8 cGreetingFlag;
    static const uint8 cDialogueFlag;

    std::map<std::string, std::vector<std::string> > m_GreetingLines;
    std::map<std::string, LineRecord> m_DialogueLines;
}; //class

} //namespace

#endif // DIALOGUE_H