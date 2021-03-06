/*
 -----------------------------------------------------------------------------
    This file is part of the Dusk Engine.
    Copyright (C) 2008, 2009, 2010, 2013  Thoronador

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

#include "CommandLoopSound.h"
#include "../sound/Sound.h"

namespace Dusk
{
  //constructor
  CommandLoopSound::CommandLoopSound(const std::string& SourceID, const bool Looping)
  : Command(),
    m_Source(SourceID),
    m_DoLoop(Looping)
  {
  }

  //destructor
  CommandLoopSound::~CommandLoopSound()
  {
    //empty
  }

  bool CommandLoopSound::execute(Dusk::Scene* scene, int count)
  {
    Source & src = Sound::get().getSource(m_Source);
    return src.loop(m_DoLoop);
  }
}
