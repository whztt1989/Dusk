/*
 -----------------------------------------------------------------------------
    This file is part of the Dusk Engine.
    Copyright (C) 2009, 2010 thoronador

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
 Date:    2010-03-12
 Purpose: Weather Singleton class
          responsible for managing all weather effects in the game, e.g. fog,
          rain, snow

 History:
     - 2009-12-08 (rev 140) - initial version (by thoronador); fog handling
     - 2009-12-31 (rev 147) - documentation update
     - 2010-01-03 (rev 149) - snow implemented as particle system
     - 2010-01-19 (rev 157) - rain implemented as particle system
     - 2010-02-09 (rev 170) - getFogColour() added
     - 2010-03-12 (rev 180) - changes for Weather to be used in Editor, too
     - 2010-08-15 (rev 227) - changes to prevent memory access violation during
                              singleton destruction
     - 2010-08-22 (rev 233) - sun added

 ToDo list:
     - other effects, such as rain, maybe even wind
     - find better parameters for snow particle system
     - ???

 Bugs:
     - No known bugs. If you find one (or more), then tell me please.

 Issues:
     - Snow and rain will go through any solid objects, such as houses.
 --------------------------------------------------------------------------*/

#ifndef WEATHER_H
#define WEATHER_H

#include <OgreColourValue.h>
#include "Celestial.h"

namespace Dusk
{

  class Weather
  {
    public:
      ~Weather();

      /* Singleton access method */
      static Weather& getSingelton();

      /* sets the fog colour

         parameters:
             r, g, b - red, green and blue values of the fog colour
                       Each of these values has to be in [0;1].
      */
      void setFogColour(const float r, const float g, const float b);

      /* retrieves the fog colour

         parameters:
             r, g, b - pointers to variables for red, green and blue values of
                       the fog colour.
                       If they are not NULL, the values will be stored in the
                       floating point variables they are pointing at.
                       Each of these values will be in [0;1].
      */
      void getFogColour(float* r, float* g, float* b) const;

      /* creates linear fog

         parameters:
             start_distance - distance (in world units) where the fog starts.
             max_distance   - distance at which fog becomes completely opaque.
      */
      void startLinearFog(const float start_distance, const float max_distance);

      /* creates exponential fog

         parameters:
             density - the density of the fog as a value between 0 and 1
      */
      void startExponentialFog(const float density);

      /* clears any fog, if present */
      void stopFog();

      /* returns true, if some kind of fog is present */
      bool isFoggy() const;

      /* returns true, if the currently active fog is linear fog */
      bool isLinearFog() const;

      /* let there be snow - and there will be snow ;) */
      void startSnow();

      /* removes any snow, if present */
      void stopSnow();

      /* returns true, if it currently snows */
      bool isSnowing() const;

      /* let it rain down */
      void startRain();

      /* removes any rain, if present */
      void stopRain();

      /* returns true, if it's currently raining */
      bool isRaining() const;

      /* returns the current time used in calculations for sun position

         remarks:
             The value has to be interpreted as hour of the day, where 6.0 means
             6:00 in the morning, and 17.0 means 5:00 in the evening.
       */
      float getDaytime() const;

      /* updates the time by adding the seconds that have passed since last
         call

         parameters:
             seconds_passed - the amount of seconds(!) passed since last call

         remarks:
             If you do not want the sun to move in real time, multiply the given
             value by an appropriate factor to speed things up.
      */
      void addDaytime(const float seconds_passed);

      /* deletes all currently managed, celestial objects */
      void deleteAllCelestials();

    private:
      Weather();
      Weather(Weather& op) {}
      //fog-related
      float m_Fog_r, m_Fog_g, m_Fog_b;
      //render window's background colour before fog was applied
      Ogre::ColourValue m_orig_bgcolour;

      //current time
      float m_Daytime;
      //list of managed, celestial objects
      std::vector<Celestial*> m_CelestialList;

      //name of the particle system for snow
      static const std::string cSnowParticleSystem;
      static const std::string cRainParticleSystem;
  };//class

} //namespace

#endif // WEATHER_H
