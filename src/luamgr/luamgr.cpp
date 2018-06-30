#include "luamgr.h"
#include "master/master.h"
#include "lua_export/lua_export.h"
#include <iostream>
#include <cstring>


LuaMgr::LuaMgr()
{
	L = luaL_newstate(); 
	luaL_openlibs(L); 
	ExportCfunction(L); 
}

LuaMgr::~LuaMgr()
{
	lua_close(L); 
}

void LuaMgr::Init()
{
	if (luaL_dofile(L, "script/main.lua"))
	{
		const char* error_msg = lua_tostring(L, -1); 
		std::cout << "luaL_dofile(L, \"script/main.lua\") Error:" << error_msg << std::endl; 
		lua_pop(L, 1); 
	}
}

void LuaMgr::Run()
{

}

void LuaMgr::Update(int frame)
{
	lua_getglobal(L, "Update"); 
	lua_pushinteger(L, frame); 
	if (lua_pcall(L, 1, 0, 0))
	{
		const char* error_msg = lua_tostring(L, -1); 
		std::cout << "call Update(frame) Error:" << error_msg << std::endl; 
		lua_pop(L, 1); 
	}
}

void LuaMgr::OnCmd(const char* cmd)
{
	if (!strcmp(cmd, "reload"))
	{
		if (luaL_dofile(L, "script/main.lua"))
		{
			const char* error_msg = lua_tostring(L, -1); 
			std::cout << "luaL_dofile(L, \"script/main.lua\") Error:" << error_msg << std::endl; 
			lua_pop(L, 1); 
		}
	}

	lua_getglobal(L, "OnCmd"); 
	lua_pushstring(L, cmd); 
	if (lua_pcall(L, 1, 0, 0))
	{
		const char* error_msg = lua_tostring(L, -1); 
		std::cout << "call OnCmd(cmd) Error:" << error_msg << std::endl; 
		lua_pop(L, 1); 
	}
}
