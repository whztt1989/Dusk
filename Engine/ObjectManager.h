/*
 -----------------------------------------------------------------------------
    This file is part of the Dusk Engine.
    Copyright (C) 2009, 2010, 2011, 2012  thoronador

    The Dusk Engine is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    The Dusk Engine is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with the Dusk Engine.  If not, see <http://www.gnu.org/licenses/>.
 -----------------------------------------------------------------------------
*/

/*---------------------------------------------------------------------------
 Author:  thoronador
 Date:    2010-05-15
 Purpose: ObjectManager Singleton class
          holds all static objects, containers, lights and items in the game.

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
     - 2010-08-31 (rev 239) - naming convention from coding guidelines enforced
     - 2010-11-20 (rev 255) - rotation is now stored as Quaternion
     - 2010-12-03 (rev 266) - use DuskLog/Messages class for logging
     - 2011-05-11 (rev 287) - renamed numberOfReferences() to getNumberOfReferences()
     - 2012-07-02 (rev 310) - update to use Database instead of ItemBase,
                              LightBase and ObjectBase

 ToDo list:
     - extend class when further classes for non-animated objects are added
     - ???

 Bugs:
     - No known bugs. If you find one (or more), then tell me please.
 --------------------------------------------------------------------------*/


#ifndef OBJECTMANAGER_H
#define OBJECTMANAGER_H

#include "objects/DuskObject.h"
#include "objects/Item.h"
#include "objects/Light.h"
#include "objects/Container.h"
#include "objects/Weapon.h"
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
      static ObjectManager& getSingleton();

      /* returns the number of objects currently loaded */
      unsigned int getNumberOfReferences() const;

      /* adds a new object instance with given ID, position, rotation and scale
         to the game and returns a pointer to the newly created object
      */
      DuskObject* addObjectReference(const std::string& ID, const Ogre::Vector3& position,
                               const Ogre::Quaternion& rotation, const float scale);

      /* adds a new light instance with given ID and position to the game and
         returns a pointer to the newly created light

         parameters:
             ID       - ID of the light
             position - light's position
      */
      Light* addLightReference(const std::string& ID, const Ogre::Vector3& position);

      /* adds a new container instance with given ID, position, rotation and scale
         to the game and returns a pointer to the newly created container
      */
      Container* addContainerReference(const std::string& ID, const Ogre::Vector3& position,
                               const Ogre::Quaternion& rotation, const float scale);

      /* adds a new item instance with given ID, position, rotation and scale
         to the game and returns a pointer to the newly created item
      */
      Item* addItemReference(const std::string& ID, const Ogre::Vector3& position,
                             const Ogre::Quaternion& rotation, const float scale);

      /* adds a new weapon instance with given ID, position, rotation and scale
         to the game and returns a pointer to the newly created weapon
      */
      Weapon* addWeaponReference(const std::string& ID, const Ogre::Vector3& position,
                             const Ogre::Quaternion& rotation, const float scale);

      /* returns the first object of given ID, or NULL if no such object is
         present

         parameters:
             ID - ID of the object that shall be returned
      */
      DuskObject* getObjectByID(const std::string& ID) const;

      /* returns true, if at least one object of given ID is present

         parameters:
             ID - ID of the object whose presence should be checked
      */
      bool isObjectPresent(const std::string& ID) const;

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

      /* Tries to save all objects to the stream and returns true on success

         parameters:
             Stream - the output stream that is used to save the object
      */
      bool saveAllToStream(std::ofstream& Stream) const;

      /* Tries to load the next object from stream and returns true on success

         parameters:
             Stream           - input stream that will be used to read the data
             PrefetchedHeader - the first four bytes of the record to come
      */
      bool loadNextFromStream(std::ifstream& Stream, const unsigned int PrefetchedHeader);

      /* Tries to enable all objects, i.e. display them in the scene

         parameters:
             scm - SceneManager which shall display the objects
      */
      void enableAllObjects(Ogre::SceneManager * scm);

      /* Tries to disable all objects */
      void disableAllObjects();

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
      void clearData();
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
