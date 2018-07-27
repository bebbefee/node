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

void LuaMgr::OnAccept(int srv_id, int net_id, unsigned int ip, unsigned short port)
{

	lua_getglobal(L, "OnAccept"); 
	lua_pushinteger(L, srv_id); 
	lua_pushinteger(L, net_id); 
	lua_pushinteger(L, ip); 
	lua_pushinteger(L, port); 
	if (lua_pcall(L, 4, 0, 0))
	{
		const char* error_msg = lua_tostring(L, -1); 
		std::cout << "call OnAccept(srv_id, net_id, ip, port) Error:" << error_msg << std::endl; 
		lua_pop(L, 1); 
	}
}

void LuaMgr::OnRecv(int net_id, const char* data, unsigned int length)
{
	lua_getglobal(L, "OnRecv"); 
	lua_pushinteger(L, net_id); 
	lua_pushlstring(L, data, length); 
	lua_pushinteger(L, length); 
	if (lua_pcall(L, 3, 0, 0))
	{
		const char* error_msg = lua_tostring(L, -1); 
		std::cout << "call OnRecv(net_id, data, length) Error:" << error_msg << std::endl; 
		lua_pop(L, 1); 
	}
}

void LuaMgr::OnClose(int net_id)
{
	lua_getglobal(L, "OnClose"); 
	lua_pushinteger(L, net_id); 
	if (lua_pcall(L, 1, 0, 0))
	{
		const char* error_msg = lua_tostring(L, -1); 
		std::cout << "call OnClose(net_id) Error:" << error_msg << std::endl; 
		lua_pop(L, 1); 
	}
}

void LuaMgr::OnConnected(int connect_id, int net_id)
{
	lua_getglobal(L, "OnConnected"); 
	lua_pushinteger(L, connect_id); 
	lua_pushinteger(L, net_id); 
	if (lua_pcall(L, 2, 0, 0))
	{
		const char* error_msg = lua_tostring(L, -1); 
		std::cout << "call OnConnected(connect_id, net_id) Error:" << error_msg << std::endl; 
		lua_pop(L, 1); 
	}
}

void LuaMgr::OnConnecteFaild(int connect_id)
{
	lua_getglobal(L, "OnConnecteFaild"); 
	lua_pushinteger(L, connect_id); 
	if (lua_pcall(L, 1, 0, 0))
	{
		const char* error_msg = lua_tostring(L, -1); 
		std::cout << "call OnConnecteFaild(connect_id) Error:" << error_msg << std::endl; 
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
