/*---------------------------------------------------------------------------
 Author:  thoronador
 Date:    2010-05-15
 Purpose: ObjectManager Singleton class
          holds data of all static objects, containers, lights and items in
          the game.

 History:
     - 2009-07-01 (rev 101) - initial version (by thoronador)
     - 2009-07-02 (rev 102) - fixed a bug in load function
     - 2009-07-13 (rev 104) - SaveToStream() added; EnableAllObjects() does
                              now havee SceneManager parameter
     - 2009-07-15 (rev 105) - addReference() and LoadFromStream() added
     - 2009-07-24 (rev 111) - ClearData() added
     - 2009-08-05 (rev 115) - deleteReferencesOfObject() added
     - 2009-08-09 (rev 118) - reenableReferencesOfObject() added
     - 2009-08-10 (rev 119) - updateReferencesAfterIDChange() added
     - 2009-09-22 (rev 130) - addLightReference() added; ObjectBase can now
                              handle lights, too.
                            - loading and saving is now handled via methods of
                              DuskObject and Light classes
     - 2009-09-27 (rev 132) - addContainerReference() added; ObjectBase can now
                              handle containers, too
     - 2010-01-14 (rev 153) - documentation update
                            - improved some ID checks
     - 2010-01-30 (rev 161) - obsolete load/save functions removed
     - 2010-02-06 (rev 165) - update for Item class
     - 2010-02-10 (rev 171) - moved from vector of references to map of vectors
                              of references to allow faster search for a certain
                              reference
                            - GetObjectByID() added
     - 2010-03-27 (rev 188) - removeItemReference() added
     - 2010-05-15 (rev 204) - IsObjectPresent() added
     - 2010-06-02 (rev 213) - addWeaponReference() added; ObjectData can now
                              handle Weapons, too.
     - 2010-08-18 (rev 230) - renamed from ObjectData to ObjectManager

 ToDo list:
     - extend class when further classes for non-animated objects are added
     - ???

 Bugs:
     - No known bugs. If you find one (or more), then tell me please.
 --------------------------------------------------------------------------*/


#ifndef OBJECTMANAGER_H
#define OBJECTMANAGER_H

#include "DuskObject.h"
#include "Item.h"
#include "Light.h"
#include "Container.h"
#include "Weapon.h"
#include <vector>
#include <map>
#include <fstream>

namespace Dusk
{

  class ObjectManager
  {
    public:
      virtual ~ObjectManager();

      /* Singleton access function */
      static ObjectManager& GetSingleton();

      /* returns the number of objects currently loaded */
      unsigned int NumberOfReferences() const;

      /* adds a new object instance with given ID, position, rotation and scale
         to the game and returns a pointer to the newly created object
      */
      DuskObject* addObjectReference(const std::string& ID, const Ogre::Vector3& position,
                               const Ogre::Vector3& rotation, const float scale);

      /* adds a new light instance with given ID and position to the game and
         returns a pointer to the newly created light
      */
      Light* addLightReference(const std::string& ID, const Ogre::Vector3& position);

      /* adds a new container instance with given ID, position, rotation and scale
         to the game and returns a pointer to the newly created container
      */
      Container* addContainerReference(const std::string& ID, const Ogre::Vector3& position,
                               const Ogre::Vector3& rotation, const float scale);

      /* adds a new item instance with given ID, position, rotation and scale
         to the game and returns a pointer to the newly created item
      */
      Item* addItemReference(const std::string& ID, const Ogre::Vector3& position,
                             const Ogre::Vector3& rotation, const float scale);

      /* adds a new weapon instance with given ID, position, rotation and scale
         to the game and returns a pointer to the newly created weapon
      */
      Weapon* addWeaponReference(const std::string& ID, const Ogre::Vector3& position,
                             const Ogre::Vector3& rotation, const float scale);

      /* returns the first object of given ID, or NULL if no such object is
         present
      */
      DuskObject* GetObjectByID(const std::string& ID) const;

      /* returns true, if at least one object of given ID is present
      */
      bool IsObjectPresent(const std::string& ID) const;

      /* tries to remove the given item from the reference map and returns true
         on success, false on failure.

         parameters:
             pItem - pointer to the item which shall be removed

         remarks:
             Passing NULL will return false, always.
             If the function returns true, the pointer to pItem will be invalid
             and should not be referenced any more.
      */
      bool removeItemReference(Item* pItem);

      /* Tries to save all objects to the stream and returns true on success */
      bool SaveAllToStream(std::ofstream& Stream) const;

      /* Tries to load the next object from stream and returns true on success

         parameters:
             PrefetchedHeader - the first four bytes of the record to come
      */
      bool LoadNextFromStream(std::ifstream& Stream, const unsigned int PrefetchedHeader);

      /* Tries to enable all objects, i.e. display them in the scene

         parameters:
             scm - SceneManager which shall display the objects
      */
      void EnableAllObjects(Ogre::SceneManager * scm);

      /* Tries to disable all objects */
      void DisableAllObjects();

      #ifdef DUSK_EDITOR
      /* Deletes all objects with the given ID and returns the number of deleted objects.

         remarks:
             This function should not be called in-game. It is only used by the Editor.
      */
      unsigned int deleteReferencesOfObject(const std::string& ID);

      /* Disables and re-enables all currently enabled objects of given ID.

         remarks:
             This is the method to update all enabled objects of one ID after
             the mesh path has changed. This will not happen in-game and is
             only used by the Editor.
      */
      unsigned int reenableReferencesOfObject(const std::string& ID, Ogre::SceneManager * scm);

      /* Changes the ID of all objects with ID oldID to newID

         remarks:
             Do not call this manually or in-game.
             Method is used to update all references of an object after the ID
             was changed (by Editor application).
      */
      unsigned int updateReferencesAfterIDChange(const std::string& oldID, const std::string& newID, Ogre::SceneManager* scm);
      #endif //DUSK_EDITOR

      /* deletes all objects

         remarks:
             Never call this one manually, unless you know what you are doing here.
      */
      void ClearData();
    private:
      /* private constructor (singleton pattern) */
      ObjectManager();
      /* empty copy constructor (singleton pattern) */
      ObjectManager(const ObjectManager& op){}
      //std::vector<DuskObject*> m_ReferenceList;
      std::map<std::string, std::vector<DuskObject*> > m_ReferenceMap;
      unsigned int m_RefCount;
  };//class

}//namespace

#endif // OBJECTMANAGER_H