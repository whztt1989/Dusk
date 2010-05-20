#include "AnimatedObject.h"
#include "ObjectBase.h" //should replace this one later
#include <OgreAnimationState.h>
#include "DuskConstants.h"

namespace Dusk
{

AnimatedObject::AnimatedObject()
{
  //ctor
  ID = "";
  position = Ogre::Vector3::ZERO;
  rotation = Ogre::Vector3::ZERO;
  m_Scale = 1.0f;
  entity = NULL;

  m_Anim = "";
  m_DoPlayAnim = false;
  m_LoopAnim = false;
}

AnimatedObject::AnimatedObject(const std::string& _ID, const Ogre::Vector3& pos, const Ogre::Vector3& rot, const float Scale)
{
  ID = _ID;
  position = pos;
  rotation = rot;
  if (m_Scale>0.0f)
  {
    m_Scale = Scale;
  } else {
    m_Scale = 1.0f;
  }
  entity = NULL;

  m_Anim = "";
  m_DoPlayAnim = false;
  m_LoopAnim = false;
}

AnimatedObject::~AnimatedObject()
{
  //dtor
  Disable();
}

bool AnimatedObject::Enable(Ogre::SceneManager* scm)
{
  if (entity!=NULL)
  {
    return true;
  }
  if (scm==NULL)
  {
    std::cout << "AnimatedObject::Enable: ERROR: no scene manager present.\n";
    return false;
  }

  //generate unique entity name
  std::stringstream entity_name;
  entity_name << ID << GenerateUniqueObjectID();
  //create entity + node and attach entity to node
  entity = scm->createEntity(entity_name.str(), ObjectBase::GetSingleton().GetMeshName(ID));
  Ogre::SceneNode* ent_node = scm->getRootSceneNode()->createChildSceneNode(entity_name.str(), position);
  ent_node->attachObject(entity);
  ent_node->scale(m_Scale, m_Scale, m_Scale);
  //not sure whether this is the best one for rotation
  ent_node->rotate(Ogre::Vector3::UNIT_X, Ogre::Degree(rotation.x));
  ent_node->rotate(Ogre::Vector3::UNIT_Y, Ogre::Degree(rotation.y));
  ent_node->rotate(Ogre::Vector3::UNIT_Z, Ogre::Degree(rotation.z));
  //set user defined object to this object as reverse link
  entity->setUserObject(this);
  if (m_Anim != "")
  {
    Ogre::AnimationStateSet* anim_set = entity->getAllAnimationStates();
    if (anim_set->hasAnimationState(m_Anim))
    {
      Ogre::AnimationState* state = anim_set->getAnimationState(m_Anim);
      state->setTimePosition(0.0f);
      state->setLoop(m_LoopAnim);
      state->setEnabled(true);
    }
    else
    {
      m_Anim = "";
    }
  }
  return (entity!=NULL);
}

ObjectTypes AnimatedObject::GetType() const
{
  return otAnimated;
}

void AnimatedObject::PlayAnimation(const std::string& AnimName, const bool DoLoop)
{
  if (AnimName == m_Anim and DoLoop==m_LoopAnim)
  { //no work to do here
    return;
  }
  if (entity!=NULL)
  {
    Ogre::AnimationState* state = NULL;
    Ogre::AnimationStateSet * anim_set = NULL;
    anim_set = entity->getAllAnimationStates();
    if (m_Anim != "")
    {
      if (anim_set == NULL)
      {
        std::cout << "AnimatedObject::PlayAnimation: Error: mesh has no animations!\n";
        return;
      }
      //stop old animation
      state = entity->getAnimationState(m_Anim);
      state->setEnabled(false);
    }
    //new animation
    if (AnimName!="")
    {
      if (anim_set == NULL)
      {
        std::cout << "AnimatedObject::PlayAnimation: Error: mesh has no animations!\n";
        return;
      }
      if (!anim_set->hasAnimationState(AnimName))
      {
        std::cout << "AnimatedObject::PlayAnimation: Error: mesh has no animation named \""
                  << AnimName<<"\"!\n";
        return;
      }
      //new anim
      state = entity->getAnimationState(AnimName);
      state->setTimePosition(0.0f);
      state->setLoop(DoLoop);
      state->setEnabled(true);
    }
  }//if
  m_Anim = AnimName;
  m_LoopAnim = DoLoop;
  m_DoPlayAnim = (AnimName != "");
}

std::string AnimatedObject::GetAnimation() const
{
  return m_Anim;
}

bool AnimatedObject::GetLoopState() const
{
  return m_LoopAnim;
}

void AnimatedObject::injectTime(const float SecondsPassed)
{
  if (SecondsPassed<=0.0f)
  {
    return;
  }
  //adjust animation state
  if (m_Anim!="")
  {
    entity->getAnimationState(m_Anim)->addTime(SecondsPassed);
  }
}

bool AnimatedObject::SaveToStream(std::ofstream& OutStream) const
{
  if (!OutStream.good())
  {
    std::cout << "AnimatedObject::SaveToStream: ERROR: Stream contains errors!\n";
    return false;
  }
  //write header "RefA" (reference of AnimatedObject)
  OutStream.write((char*) &cHeaderRefA, sizeof(unsigned int));
  //write all data inherited from DuskObject
  if (!SaveDuskObjectPart(OutStream))
  {
    std::cout << "AnimatedObject::SaveToStream: ERROR while writing basic "
              << "data!\n";
    return false;
  }
  // go on with new data members from AnimatedObject
  return SaveAnimatedObjectPart(OutStream);
}

bool AnimatedObject::SaveAnimatedObjectPart(std::ofstream& OutStream) const
{
  // save new data members from AnimatedObject
  // -- anim name
  const unsigned int len = m_Anim.length();
  OutStream.write((char*) &len, sizeof(len));
  // -- loop mode?
  OutStream.write((char*) &m_LoopAnim, sizeof(bool));
  // -- playing?
  OutStream.write((char*) &m_DoPlayAnim, sizeof(bool));
  return OutStream.good();
}

bool AnimatedObject::LoadFromStream(std::ifstream& InStream)
{
  if (entity!=NULL)
  {
    std::cout << "AnimatedObject::LoadFromStream: ERROR: Cannot load from "
              << "stream while object is enabled.\n";
    return false;
  }
  if (!InStream.good())
  {
    std::cout << "AnimatedObject::LoadFromStream: ERROR: Stream contains errors!\n";
    return false;
  }
  //read header "RefA"
  unsigned int Header = 0;
  InStream.read((char*) &Header, sizeof(unsigned int));
  if (Header!=cHeaderRefA)
  {
    std::cout << "AnimatedObject::LoadFromStream: ERROR: Stream contains "
              << "invalid reference header.\n";
    return false;
  }
  //read all stuff inherited from DuskObject
  if (!LoadDuskObjectPart(InStream))
  {
    std::cout << "AnimatedObject::LoadFromStream: ERROR while reading basic "
              << "object data.\n";
    return false;
  }//if
  // go on with new data members from AnimatedObject
  return LoadAnimatedObjectPart(InStream);
}

bool AnimatedObject::LoadAnimatedObjectPart(std::ifstream& InStream)
{
  //load data members from AnimatedObject
  //animation data
  // -- anim name
  unsigned int len = 0;
  InStream.read((char*) &len, sizeof(unsigned int));
  if (len>255)
  {
    std::cout << "AnimatedObject::LoadAnimatedObjectPart: ERROR: animation "
              << "name cannot be longer than 255 characters.\n";
    return false;
  }
  char ID_Buffer[256];
  InStream.read(ID_Buffer, len);
  ID_Buffer[len] = '\0';
  if (!InStream.good())
  {
    std::cout << "AnimatedObject::LoadAnimatedObjectPart: ERROR while reading "
              << "animation name.\n";
    return false;
  }
  m_Anim = std::string(ID_Buffer);
  // -- loop mode?
  InStream.read((char*) &m_LoopAnim, sizeof(bool));
  // -- playing?
  InStream.read((char*) &m_DoPlayAnim, sizeof(bool));
  return (InStream.good());
}

} //namespace
