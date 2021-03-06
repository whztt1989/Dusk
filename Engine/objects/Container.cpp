/*
 -----------------------------------------------------------------------------
    This file is part of the Dusk Engine.
    Copyright (C) 2009, 2010, 2012, 2013  Thoronador

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
 -----------------------------------------------------------------------------
*/

#include "Container.h"
#include "../database/ContainerRecord.h"
#include "../database/Database.h"
#include "../DuskConstants.h"
#include "../Messages.h"

namespace Dusk
{

Container::Container()
    : DuskObject(),
      m_Contents(Inventory()),
      m_Changed(false)
{
}

Container::Container(const std::string& _ID, const Ogre::Vector3& pos, const Ogre::Quaternion& rot, const float Scale)
    : DuskObject(_ID, pos, rot, Scale)
{
  m_Changed = false;
  m_Contents.makeEmpty();
  Database::getSingleton().getTypedRecord<ContainerRecord>(_ID).ContainerInventory.addAllItemsTo(m_Contents);
}

Container::~Container()
{
  m_Contents.makeEmpty();
  disable();
}

bool Container::isEmpty() const
{
  return m_Contents.isEmpty();
}

void Container::transferAllItemsTo(Inventory& target)
{
  m_Contents.addAllItemsTo(target);
  m_Contents.makeEmpty();
  m_Changed = true;
}

void Container::addItem(const std::string& ItemID, const unsigned int count)
{
  if (count==0 or ItemID.empty())
  {
    return;
  }
  m_Contents.addItem(ItemID, count);
  m_Changed = true;
}

unsigned int Container::removeItem(const std::string& ItemID, const unsigned int count)
{
  if (count==0 or ItemID.empty())
  {
    return 0;
  }
  m_Changed = true;
  return m_Contents.removeItem(ItemID, count);
}

unsigned int Container::getItemCount(const std::string& ItemID) const
{
  return m_Contents.getItemCount(ItemID);
}

bool Container::canCollide() const
{
  return true;
}

const std::string& Container::getObjectMesh() const
{
  return Database::getSingleton().getTypedRecord<ContainerRecord>(ID).Mesh;
}

ObjectTypes Container::getDuskType() const
{
  return otContainer;
}

bool Container::saveToStream(std::ofstream& OutStream) const
{
  if (!OutStream.good())
  {
    DuskLog() << "Container::saveToStream: ERROR: Stream contains errors!\n";
    return false;
  }
  //write header "RefC" (reference of Container)
  OutStream.write((const char*) &cHeaderRefC, sizeof(uint32_t)); //header
  //write data inherited from DuskObject
  if (!saveDuskObjectPart(OutStream))
  {
    DuskLog() << "Container::saveToStream: ERROR while writing basic data!\n";
    return false;
  }
  //write inventory
  // -- flags
  OutStream.write((const char*) &m_Changed, sizeof(bool));
  // -- inventory (only if necessary)
  if (m_Changed)
  {
    if (!m_Contents.saveToStream(OutStream))
    {
      DuskLog() << "Container::saveToStream: ERROR while writing inventory data!\n";
      return false;
    }//if
  }
  return (OutStream.good());
}

bool Container::loadFromStream(std::ifstream& InStream)
{
  if (entity!=NULL)
  {
    DuskLog() << "Container::loadFromStream: ERROR: Cannot load from stream "
              << "while container is enabled.\n";
    return false;
  }
  if (!InStream.good())
  {
    DuskLog() << "Container::loadFromStream: ERROR: Stream contains errors!\n";
    return false;
  }

  //read header "RefC"
  uint32_t Header = 0;
  InStream.read((char*) &Header, sizeof(uint32_t));
  if (Header!=cHeaderRefC)
  {
    DuskLog() << "Container::loadFromStream: ERROR: Stream contains invalid "
              << "reference header.\n";
    return false;
  }
  //load data members inherited from DuskObject
  if (!loadDuskObjectPart(InStream))
  {
    DuskLog() << "Container::loadFromStream: ERROR while reading basic data.\n";
    return false;
  }
  //Container's own stuff
  //load inventory
  // -- flags
  InStream.read((char*) &m_Changed, sizeof(bool));
  if (m_Changed)
  { //load it from stream, contents were changed
    if (!m_Contents.loadFromStream(InStream))
    {
      DuskLog() << "Container::loadFromStream: ERROR while reading container "
                << "inventory from stream.\n";
      return false;
    }
  }
  else
  { //inventory was not changed, so get it from ContainerBase
    m_Contents.makeEmpty();
    Database::getSingleton().getTypedRecord<ContainerRecord>(ID).ContainerInventory.addAllItemsTo(m_Contents);
  }
  return (InStream.good());
}

}//namespace
