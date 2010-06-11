#include "LuaBindingsAnimated.h"
#include "AnimationData.h"
#include "Player.h"

namespace Dusk
{

namespace Lua
{

int GetPlayer(lua_State *L)
{
  if (lua_gettop(L)==0)
  {
    AnimatedObject* a = &(Player::GetSingleton());
    lua_pushlightuserdata(L, a);
    return 1;
  }
  lua_pushstring(L, "GetPlayer does not expect any arguments!\n");
  lua_error(L);
  return 0;
}

int GetAnimated(lua_State *L)
{
  if (lua_gettop(L)==1)
  {
    lua_pushlightuserdata(L, AnimationData::GetSingleton().GetAnimatedObjectReference(lua_tostring(L, 1)));
    return 1;
  }
  lua_pushstring(L, "GetAnimated expects exactly one argument!\n");
  lua_error(L);
  return 0;
}

int GetAnimatedLoop(lua_State *L)
{
  if (lua_gettop(L)==1)
  {
    const AnimatedObject* aniPtr = static_cast<AnimatedObject*> (lua_touserdata(L, 1));
    if (aniPtr!=NULL)
    {
      if (aniPtr->GetLoopState())
      {
        lua_pushboolean(L, 1);
      }
      else
      {
        lua_pushboolean(L, 1);
      }
      return 1;
    }
    lua_pushstring(L, "GetAnimatedLoop got NULL for object pointer!\n");
    lua_error(L);
    return 0;
  }
  lua_pushstring(L, "GetAnimatedLoop expects exactly one argument!\n");
  lua_error(L);
  return 0;
}

int GetAnimatedAnimation(lua_State *L)
{
  if (lua_gettop(L)==1)
  {
    const AnimatedObject* aniPtr = static_cast<AnimatedObject*> (lua_touserdata(L, 1));
    if (aniPtr!=NULL)
    {
      lua_pushstring(L, aniPtr->GetAnimation().c_str());
      return 1;
    }
    lua_pushstring(L, "GetAnimatedLoop got NULL for object pointer!\n");
    lua_error(L);
    return 0;
  }
  lua_pushstring(L, "GetAnimatedAnimation expects exactly one argument!\n");
  lua_error(L);
  return 0;
}

int AnimatedPlayAnimation(lua_State *L)
{
  if (lua_gettop(L)==3)
  {
    AnimatedObject* aniPtr = static_cast<AnimatedObject*> (lua_touserdata(L, 1));
    if (aniPtr!=NULL)
    {
      aniPtr->PlayAnimation(lua_tostring(L, 2), lua_toboolean(L, 3));
      return 0;
    }
    lua_pushstring(L, "AnimatedPlayAnimation got NULL for object pointer!\n");
    lua_error(L);
    return 0;
  }
  lua_pushstring(L, "AnimatedPlayAnimation expects exactly three arguments!\n");
  lua_error(L);
  return 0;
}

int AnimatedStartAnimation(lua_State *L)
{
  if (lua_gettop(L)==3)
  {
    AnimatedObject* aniPtr = static_cast<AnimatedObject*> (lua_touserdata(L, 1));
    if (aniPtr!=NULL)
    {
      if (aniPtr->StartAnimation(lua_tostring(L, 2), lua_toboolean(L, 3)))
        lua_pushboolean(L, 1);
      else lua_pushboolean(L, 0);
      return 1;
    }
    lua_pushstring(L, "AnimatedStartAnimation got NULL for object pointer!\n");
    lua_error(L);
    return 0;
  }
  lua_pushstring(L, "AnimatedStartAnimation expects exactly three arguments!\n");
  lua_error(L);
  return 0;
}

int AnimatedStopAnimation(lua_State *L)
{
  if (lua_gettop(L)==2)
  {
    AnimatedObject* aniPtr = static_cast<AnimatedObject*> (lua_touserdata(L, 1));
    if (aniPtr!=NULL)
    {
      if (aniPtr->StopAnimation(lua_tostring(L, 2)))
        lua_pushboolean(L, 1);
      else lua_pushboolean(L, 0);
      return 1;
    }
    lua_pushstring(L, "AnimatedStopAnimation got NULL for object pointer!\n");
    lua_error(L);
    return 0;
  }
  lua_pushstring(L, "AnimatedStopAnimation expects exactly two arguments!\n");
  lua_error(L);
  return 0;
}

int AnimatedStopAllAnimations(lua_State *L)
{
  if (lua_gettop(L)==1)
  {
    AnimatedObject* aniPtr = static_cast<AnimatedObject*> (lua_touserdata(L, 1));
    if (aniPtr!=NULL)
    {
      lua_pushnumber(L, aniPtr->StopAllAnimations());
      return 1;
    }
    lua_pushstring(L, "AnimatedStopAllAnimations got NULL for object pointer!\n");
    lua_error(L);
    return 0;
  }
  lua_pushstring(L, "AnimatedStopAllAnimations expects exactly one argument!\n");
  lua_error(L);
  return 0;
}

int AnimatedIsAnimationActive(lua_State *L)
{
  if (lua_gettop(L)==2)
  {
    const AnimatedObject* aniPtr = static_cast<AnimatedObject*> (lua_touserdata(L, 1));
    if (aniPtr!=NULL)
    {
      if (aniPtr->IsAnimationActive(lua_tostring(L, 2)))
        lua_pushboolean(L, 1);
      else lua_pushboolean(L, 0);
      return 1;
    }
    lua_pushstring(L, "AnimatedIsAnimationActive got NULL for object pointer!\n");
    lua_error(L);
    return 0;
  }
  lua_pushstring(L, "AnimatedIsAnimationActive expects exactly two arguments!\n");
  lua_error(L);
  return 0;
}

void registerAnimated(lua_State *L)
{
  lua_register(L, "GetAnimatedObject", GetAnimated);
  lua_register(L, "GetPlayer", GetPlayer);

  lua_register(L, "GetAnimatedAnimation", GetAnimatedAnimation);
  lua_register(L, "GetAnimatedLoop", GetAnimatedLoop);
  lua_register(L, "AnimatedPlayAnimation", AnimatedPlayAnimation);
  lua_register(L, "AnimatedStartAnimation", AnimatedStartAnimation);
  lua_register(L, "AnimatedStopAnimation", AnimatedStopAnimation);
  lua_register(L, "AnimatedStopAllAnimations", AnimatedStopAllAnimations);
  lua_register(L, "AnimatedIsAnimationActive", AnimatedIsAnimationActive);
}

} //namespace Lua

} //namespace Dusk