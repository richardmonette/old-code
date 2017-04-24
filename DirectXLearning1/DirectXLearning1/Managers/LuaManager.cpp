#include "../Global.h"
#include "LuaManager.h"

LuaManager::LuaManager()
{
	m_LuaState = lua_open();

	luaL_openlibs(m_LuaState);

	//luaL_dofile(m_LuaState, "test.lua");
}

LuaManager::~LuaManager()
{
	lua_close(m_LuaState);
}
