/*
 -----------------------------------------------------------------------------
    This file is part of the Dusk Engine.
    Copyright (C) 2010 thoronador

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

#include "Sun.h"
#include "API.h"
#include "Messages.h"
#include <string>
#include <OgreParticle.h>
#include <OgreVector3.h>
#include <OgreMath.h>

namespace Dusk
{

const std::string cSunBillboardSet = "Dusk/SunBBS";
const float cSunDistance = 400;

Sun::Sun()
{
  m_Start = 8.0f;
  m_End = 20.0f;
  m_BBSet = NULL;
  m_Light = NULL;
}

Sun::~Sun()
{
  hide();
}

void Sun::show()
{
  Ogre::SceneManager* scm = getAPI().getOgreSceneManager();
  if (scm==NULL)
  {
    DuskLog() << "Sun::show: ERROR: Got NULL for SceneManager.\n";
    return;
  }
  if (isVisible())
  {
    DuskLog() << "Sun::show: Hint: System is already visible. Skipping.\n";
    return;
  }
  m_BBSet = scm->createBillboardSet(cSunBillboardSet);
  m_BBSet->setMaterialName("Dusk/flare");
  const Ogre::ColourValue sunColour = Ogre::ColourValue(1.0f, 1.0f, 0.0f);
  m_BBSet->createBillboard(cSunDistance, 0, 0, sunColour);
  Ogre::SceneNode* node = scm->createSceneNode(cSunBillboardSet+"/Node");
  node->attachObject(m_BBSet);
  m_Light = scm->createLight(cSunBillboardSet+"/Light");
  m_Light->setType(Ogre::Light::LT_POINT);
  m_Light->setPosition(Ogre::Vector3(cSunDistance, 0.0f, 0.0f));
  m_Light->setDiffuseColour(sunColour);
  m_Light->setSpecularColour(sunColour);
  node->attachObject(m_Light);
  scm->getRootSceneNode()->addChild(node);
}

void Sun::hide()
{
  if (!isVisible())
  {
    DuskLog() << "Sun::hide: Hint: System is already not visible. Skipping.\n";
    return;
  }
  Ogre::SceneManager* scm = getAPI().getOgreSceneManager();
  if (scm==NULL)
  {
    DuskLog() << "Sun::hide: ERROR: Got NULL for SceneManager.\n";
    return;
  }
  Ogre::SceneNode* node = m_BBSet->getParentSceneNode();
  node->getParentSceneNode()->removeChild(node);
  node->detachObject(m_BBSet);
  node->detachObject(m_Light);
  scm->destroySceneNode(node->getName());
  node = NULL;
  scm->destroyLight(m_Light);
  m_Light = NULL;
  scm->destroyBillboardSet(m_BBSet);
  m_BBSet = NULL;
}

bool Sun::isVisible() const
{
  return m_BBSet!=NULL;
}

void Sun::updateTime(const float currentTime)
{
  if (isVisible())
  {
    if (!isInSpan(currentTime))
    {
      hide();
      return;
    }
    const float spr = getSpanRatio(currentTime);
    const Ogre::Vector3 vec3 = Ogre::Vector3(cSunDistance*Ogre::Math::Cos(Ogre::Math::PI*spr),
                               cSunDistance*Ogre::Math::Sin(Ogre::Math::PI*spr),
                               0.0f);
    m_BBSet->getBillboard(0)->setPosition(vec3);
    m_Light->setPosition(vec3);
  }//if
  else
  {
    if (isInSpan(currentTime))
    {
      show();
      return;
    }
  }
}

} //namespace
