/*---------------------------------------------------------------------------
 Authors: DaSteph, walljumper, thoronador
 Date:    2010-01-26
 Purpose: InputSystem Singleton class
          implements the Interface InputSystem (see InputSystem.h)

 History:
     - 2007-11-26 (rev 7)   - initial version (by DaSteph)
     - 2007-12-29 (rev 15)  - keyPressed() and keyReleased() added
     - 2007-12-29 (rev 16)  - binding list updated, input now generates Scripts
                              and sends them to Console
     - 2008-01-17 (rev 23)  - InputSystemBinding registers itself at API class
                              (by walljumper)
     - 2008-01-26 (rev 30)  - InputSystemBinding is now Singleton
     - 2008-01-26 (rev 31)  - pointer to InputSystemBinding removed from API
     - 2008-03-22 (rev 51)  - bind list updated and adjusted
     - 2008-03-25 (rev 55)  - bindings for left/right movement
     - 2008-03-26 (rev 57)  - mouseMoved(), mousePressed(), mouseReleased() added
     - 2008-04-07 (rev 61)  - some lines for handling mouse input
     - 2009-06-21 (rev 98)  - bindings for turning left/right added (by thoronador)
     - 2009-12-04 (rev 138) - bindings for zooming camera in and out added
     - 2010-01-03 (rev 149) - }  temporary bindings for toggling weather effects
     - 2010-01-19 (rev 157) - }  added (will be removed later)
     - 2010-01-26 (rev 159) - key handling for dialogue menu added

 ToDo list:
     - method to load bind list from an external resource
     - ???

 Bugs:
     - No known bugs. If you find one (or more), then tell me please.
 --------------------------------------------------------------------------*/

#ifndef INPUTSYSTEMBINDING_H_INCLUDED
#define INPUTSYSTEMBINDING_H_INCLUDED

#include "InputSystem.h"
#include "Script.h"

#include <map>

namespace Dusk
{
    /**
     * This class implements the Interface InputSystem.
     * It holds the strategy of generating a console script by a
     * lookup on an internal bind list that will be loaded from
     * an external ressource.
     */
    class InputSystemBinding : public InputSystem {
    public:
        static InputSystemBinding& get();

        /**
         * Destructer. Declared virtual so that extending classes destructors
         * will also be called.
         */
        virtual ~InputSystemBinding();

        /**
         * Implements the keyPressed event to receive notifications if an key has been pressed.
         *
         * @param arg           The KeyEvent that holds the information which button has been pressed.
         */
        virtual bool keyPressed (const OIS::KeyEvent &arg);

        /**
         * Implements the keyReleased event to receive notifications if an key has been released.
         *
         * @param arg           The KeyEvent that holds the information which button has been released.
         */
        virtual bool keyReleased (const OIS::KeyEvent &arg);


        virtual bool mouseMoved( const OIS::MouseEvent &arg );
		virtual bool mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id );
		virtual bool mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id );

    private:
        /**
         * Standard constructor
         */
        InputSystemBinding();
        InputSystemBinding(const InputSystemBinding& op){}

        /**
         * Holds the bind list where an input event is associated with a console script.
         * This list holds all durating scripts, which means that an action is done each frame.
         */
        std::map<OIS::KeyCode, Dusk::Script> myBindListHold;

        /**
         * Holds the bind list where an input event is associated with a console script.
         * This list holds all scripts that should be executed when the coressponding button is pressed
         */
        std::map<OIS::KeyCode, Dusk::Script> myBindListPress;

        /**
         * Holds the bind list where an input event is associated with a console script.
         * This list holds all scripts that should be executed when the coressponding button is pressed
         */
        std::map<OIS::KeyCode, Dusk::Script> myBindListRelease;

    };
}
#endif // INPUTSYSTEMBINDING_H_INCLUDED
