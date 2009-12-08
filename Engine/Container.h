#ifndef CONTAINER_H
#define CONTAINER_H
#include "DuskObject.h"
#include "Inventory.h"
#include <string>
#include <OgreVector3.h>

namespace Dusk
{
  class Container: public DuskObject
  {
    public:
      Container();
      Container(const std::string& _ID, const Ogre::Vector3& pos, const Ogre::Vector3& rot, const float Scale);
      virtual ~Container();
      bool IsEmpty() const;
      void TransferAllItemsTo(Inventory& target);
      void AddItem(const std::string& ItemID, const unsigned int count);
      unsigned int RemoveItem(const std::string& ItemID, const unsigned int count);
      unsigned int GetItemCount(const std::string& ItemID) const;
      virtual bool Enable(Ogre::SceneManager* scm);
      virtual ObjectTypes GetType() const;
      virtual bool SaveToStream(std::ofstream& OutStream) const;
      virtual bool LoadFromStream(std::ifstream& InStream);
    private:
      Inventory m_Contents;
      bool m_Changed;
  };//class

}//namespace

#endif // CONTAINER_H
