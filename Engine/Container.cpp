#include "Container.h"
#include "ContainerBase.h"
#include "DuskConstants.h"

namespace Dusk
{

Container::Container()
    : DuskObject()
{
  objectType = otContainer;
  m_Changed = false;
  m_Contents.MakeEmpty();
}

Container::Container(const std::string& _ID, const Ogre::Vector3& pos, const Ogre::Vector3& rot, const float Scale)
    : DuskObject(_ID, pos, rot, Scale)
{
  objectType = otContainer;
  m_Changed = false;
  m_Contents.MakeEmpty();
  ContainerBase::GetSingleton().GetContainerInventory(_ID).AddAllTo(m_Contents);
}

Container::~Container()
{
  m_Contents.MakeEmpty();
  Disable();
}

bool Container::IsEmpty() const
{
  return m_Contents.IsEmpty();
}

void Container::TransferAllContentTo(Inventory& target)
{
  m_Contents.AddAllTo(target);
  m_Contents.MakeEmpty();
  m_Changed = true;
}

void Container::AddItem(const std::string& ItemID, const unsigned int count)
{
  if (count==0 or ItemID=="")
  {
    return;
  }
  m_Contents.AddItem(ItemID, count);
  m_Changed = true;
}

unsigned int Container::RemoveItem(const std::string& ItemID, const unsigned int count)
{
  if (count==0 or ItemID=="")
  {
    return 0;
  }
  m_Changed = true;
  return m_Contents.RemoveItem(ItemID, count);
}

unsigned int Container::GetItemCount(const std::string& ItemID) const
{
  return m_Contents.GetItemCount(ItemID);
}

bool Container::Enable(Ogre::SceneManager* scm)
{
 if (entity!=NULL)
  {
    return true;
  }
  if (scm==NULL)
  {
    std::cout << "Container::Enable: ERROR: no scene manager present.\n";
    return false;
  }
  //generate unique entity name
  std::stringstream entity_name;
  entity_name << ID << GenerateUniqueObjectID();
  //create entity + node and attach entity to node
  entity = scm->createEntity(entity_name.str(), ContainerBase::GetSingleton().GetContainerMesh(ID));
  Ogre::SceneNode* ent_node = scm->getRootSceneNode()->createChildSceneNode(entity_name.str(), position);
  ent_node->attachObject(entity);
  ent_node->scale(m_Scale, m_Scale, m_Scale);
  //not sure whether this is the best one for rotation
  ent_node->rotate(Ogre::Vector3::UNIT_X, Ogre::Degree(rotation.x));
  ent_node->rotate(Ogre::Vector3::UNIT_Y, Ogre::Degree(rotation.y));
  ent_node->rotate(Ogre::Vector3::UNIT_Z, Ogre::Degree(rotation.z));
  //set user defined object to this object as reverse link
  entity->setUserObject(this);
  return (entity!=NULL);
}

bool Container::SaveToStream(std::ofstream& OutStream) const
{
  if (!OutStream.good())
  {
    std::cout << "Container::SaveToStream: ERROR: Stream contains errors!\n";
    return false;
  }
  unsigned int len;
  float xyz;

  //write header "RefC" (reference of Container)
  OutStream.write((char*) &cHeaderRefC, sizeof(unsigned int)); //header
  //write ID
  len = ID.length();
  OutStream.write((char*) &len, sizeof(unsigned int));
  OutStream.write(ID.c_str(), len);

  //write position and rotation, and scale
  // -- position
  xyz = position.x;
  OutStream.write((char*) &xyz, sizeof(float));
  xyz = position.y;
  OutStream.write((char*) &xyz, sizeof(float));
  xyz = position.z;
  OutStream.write((char*) &xyz, sizeof(float));
  // -- rotation
  xyz = rotation.x;
  OutStream.write((char*) &xyz, sizeof(float));
  xyz = rotation.y;
  OutStream.write((char*) &xyz, sizeof(float));
  xyz = rotation.z;
  OutStream.write((char*) &xyz, sizeof(float));
  // -- scale
  OutStream.write((char*) &m_Scale, sizeof(float));
  //write inventory
  // -- flags
  OutStream.write((char*) &m_Changed, sizeof(bool));
  // -- inventory (only if neccessary)
  if (m_Changed)
  {
    if (!m_Contents.SaveToStream(OutStream))
    {
      std::cout << "Container::SaveToStream: ERROR while writing inventory data!\n";
      return false;
    }//if
  }
  return (OutStream.good());
}

bool Container::LoadFromStream(std::ifstream& InStream)
{
  if (entity!=NULL)
  {
    std::cout << "Container::LoadFromStream: ERROR: Cannot load from stream "
              << "while container is enabled.\n";
    return false;
  }
  if (!InStream.good())
  {
    std::cout << "Container::LoadFromStream: ERROR: Stream contains errors!\n";
    return false;
  }

  char ID_Buffer[256];
  float f_temp;
  unsigned int Header, len;

  //read header "RefC"
  Header = 0;
  InStream.read((char*) &Header, sizeof(unsigned int));
  if (Header!=cHeaderRefC)
  {
    std::cout << "Container::LoadFromStream: ERROR: Stream contains invalid "
              << "reference header.\n";
    return false;
  }
  //read ID
  InStream.read((char*) &len, sizeof(unsigned int));
  if (len>255)
  {
    std::cout << "Container::LoadFromStream: ERROR: ID cannot be longer than "
              << "255 characters.\n";
    return false;
  }
  InStream.read(ID_Buffer, len);
  ID_Buffer[len] = '\0';
  if (!InStream.good())
  {
    std::cout << "Container::LoadFromStream: ERROR while reading data (ID).\n";
    return false;
  }
  ID = std::string(ID_Buffer);

  //position
  InStream.read((char*) &f_temp, sizeof(float));
  position.x = f_temp;
  InStream.read((char*) &f_temp, sizeof(float));
  position.y = f_temp;
  InStream.read((char*) &f_temp, sizeof(float));
  position.z = f_temp;
  //rotation
  InStream.read((char*) &f_temp, sizeof(float));
  rotation.x = f_temp;
  InStream.read((char*) &f_temp, sizeof(float));
  rotation.y = f_temp;
  InStream.read((char*) &f_temp, sizeof(float));
  rotation.z = f_temp;
  //scale
  InStream.read((char*) &f_temp, sizeof(float));
  m_Scale = f_temp;
  //load inventory
  // -- flags
  InStream.read((char*) &m_Changed, sizeof(bool));
  if (m_Changed)
  { //load it from stream, contents were changed
    if (!m_Contents.LoadFromStream(InStream))
    {
      std::cout << "Container::LoadFromStream: ERROR while reading container "
                << "inventory from stream.\n";
      return false;
    }
  }
  else
  { //inventory was not changed, so get it from ContainerBase
    m_Contents.MakeEmpty();
    ContainerBase::GetSingleton().GetContainerInventory(ID).AddAllTo(m_Contents);
  }
  return (InStream.good());
}

}//namespace