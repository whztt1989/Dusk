/*
 -----------------------------------------------------------------------------
    This file is part of the Dusk Engine.
    Copyright (C) 2009, 2010, 2012 thoronador

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

#include "ContainerBase.h"
#include "DuskConstants.h"
#include "DuskExceptions.h"
#include "DuskFunctions.h"
#include "Messages.h"

namespace Dusk{

ContainerBase::ContainerBase()
{ //constructor
  m_ContainerList.clear();
}

ContainerBase::~ContainerBase()
{
  //destructor
  deleteAllContainers();
}

ContainerBase& ContainerBase::getSingleton()
{
  static ContainerBase Instance;
  return Instance;
}

void ContainerBase::addContainer(const std::string& ID, const std::string& _mesh, const Inventory& contents)
{
  if (ID=="" or _mesh=="")
  {
    DuskLog() << "ContainerBase::addContainer: ERROR: ID or Mesh is empty string!\n";
    return;
  }
  std::map<std::string, ContainerRecord>::iterator iter;
  iter = m_ContainerList.find(ID);
  if (iter==m_ContainerList.end())
  {
    //not present, so create it
    m_ContainerList[ID].Mesh = adjustDirectorySeperator(_mesh);
    m_ContainerList[ID].ContainerInventory = Inventory();
    contents.addAllItemsTo(m_ContainerList[ID].ContainerInventory);
    return;
  }//if
  iter->second.Mesh = adjustDirectorySeperator(_mesh);
  iter->second.ContainerInventory.makeEmpty();
  contents.addAllItemsTo(iter->second.ContainerInventory);
  return;
}

#ifdef DUSK_EDITOR
bool ContainerBase::deleteContainer(const std::string& ID)
{
  std::map<std::string, ContainerRecord>::iterator iter;
  iter = m_ContainerList.find(ID);
  if (iter != m_ContainerList.end())
  {
    iter->second.ContainerInventory.makeEmpty();
    m_ContainerList.erase(iter);
    return true;
  }
  return false;
}
#endif

bool ContainerBase::hasContainer(const std::string& ID) const
{
  return (m_ContainerList.find(ID)!=m_ContainerList.end());
}

const std::string& ContainerBase::getContainerMesh(const std::string& ID, const bool UseMarkerOnError) const
{
  std::map<std::string, ContainerRecord>::const_iterator iter;
  iter = m_ContainerList.find(ID);
  if (iter!=m_ContainerList.end())
  {
    return iter->second.Mesh;
  }
  DuskLog() << "ContainerBase::getContainerMesh: ERROR: no container with ID \""
            << ID << "\" found.";
  if (UseMarkerOnError)
  {
    DuskLog() << "Returning empty string.\n";
    return cErrorMesh;
  }
  DuskLog() << "Exception will be thrown.\n";
  throw IDNotFound("ContainerBase", ID);
}

const Inventory& ContainerBase::getContainerInventory(const std::string& ID) const
{
  std::map<std::string, ContainerRecord>::const_iterator iter;
  iter = m_ContainerList.find(ID);
  if (iter!=m_ContainerList.end())
  {
    return iter->second.ContainerInventory;
  }
  DuskLog() << "ContainerBase::getContainerInventory: ERROR: no container with ID \""
            << ID << "\" found. Returning empty inventory.\n";
  return Inventory::getEmptyInventory();
}

void ContainerBase::deleteAllContainers()
{
  std::map<std::string, ContainerRecord>::iterator iter;
  iter = m_ContainerList.begin();
  while (iter != m_ContainerList.end())
  {
    iter->second.ContainerInventory.makeEmpty();
  }//while
  m_ContainerList.clear();
}

unsigned int ContainerBase::numberOfContainers() const
{
  return m_ContainerList.size();
}

bool ContainerBase::saveAllToStream(std::ofstream& OutStream) const
{
  if (!OutStream.good())
  {
    DuskLog() << "ContainerBase::saveAllToStream: ERROR: stream contains errors!\n";
    return false;
  }
  unsigned int len = 0;
  std::map<std::string, ContainerRecord>::const_iterator traverse;
  traverse = m_ContainerList.begin();
  while (traverse != m_ContainerList.end())
  {
    //header "Cont"
    OutStream.write((char*) &cHeaderCont, sizeof(unsigned int));
    //ID
    len = traverse->first.length();
    OutStream.write((char*) &len, sizeof(unsigned int));
    OutStream.write(traverse->first.c_str(), len);
    //Mesh
    len = traverse->second.Mesh.length();
    OutStream.write((char*) &len, sizeof(unsigned int));
    OutStream.write(traverse->second.Mesh.c_str(), len);
    //Inventory
    if (!(traverse->second.ContainerInventory.saveToStream(OutStream)))
    {
      DuskLog() << "ContainerBase::saveAllToStream: ERROR while writing "
                << "container's inventory.\n";
      return false;
    }//if
    ++traverse;
  }//while
  return OutStream.good();
}

bool ContainerBase::loadNextContainerFromStream(std::ifstream& InStream)
{
  if (!InStream.good())
  {
    DuskLog() << "ContainerBase::loadNextContainerFromStream: ERROR: stream "
              << "contains errors!\n";
    return false;
  }
  unsigned int len = 0;
  InStream.read((char*) &len, sizeof(unsigned int));
  if (len != cHeaderCont)
  {
    DuskLog() << "ContainerBase::loadNextContainerFromStream: ERROR: stream "
              << "contains unexpected header!\n";
    return false;
  }
  char ID_Buffer[256];
  ID_Buffer[0] = ID_Buffer[255] = '\0';
  //read ID
  len = 0;
  InStream.read((char*) &len, sizeof(unsigned int));
  if (len>255)
  {
    DuskLog() << "ContainerBase::loadNextContainerFromStream: ERROR: ID is "
              << "longer than 255 characters!\n";
    return false;
  }
  InStream.read(ID_Buffer, len);
  if (!InStream.good())
  {
    DuskLog() << "ContainerBase::loadNextContainerFromStream: ERROR while "
              << "reading ID from stream!\n";
    return false;
  }
  ID_Buffer[len] = '\0';
  //read Mesh
  char Mesh_Buffer[256];
  Mesh_Buffer[0] = Mesh_Buffer[255] = '\0';
  len = 0;
  InStream.read((char*) &len, sizeof(unsigned int));
  if (len>255)
  {
    DuskLog() << "ContainerBase::loadNextContainerFromStream: ERROR: mesh path "
              << "is longer than 255 characters!\n";
    return false;
  }
  InStream.read(Mesh_Buffer, len);
  if (!InStream.good())
  {
    DuskLog() << "ContainerBase::loadNextContainerFromStream: ERROR while "
              << "reading mesh path from stream!\n";
    return false;
  }
  Mesh_Buffer[len] = '\0';
  Inventory temp;
  if (!temp.loadFromStream(InStream))
  {
    DuskLog() << "ContainerBase::loadNextContainerFromStream: ERROR while "
              << "reading inventory contens from stream!\n";
    return false;
  }
  //all right so far, add new container
  addContainer(std::string(ID_Buffer), std::string(Mesh_Buffer), temp);
  return InStream.good();
}

}//namespace
