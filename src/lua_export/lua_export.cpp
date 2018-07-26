#include "lua_export.h"
#include "master/master.h"

int CCloseSrv(lua_State *L)
{
	Master& master = Master::GetInstance(); 
	master.SetIsRun(false); 

	return 0; 
}

int CStartTcpServer(lua_State *L)
{
	const char* bind_ip_str = luaL_checkstring(L, 1); 
	unsigned short port = (unsigned short)luaL_checkinteger(L, 2); 

	NetMgr& net_mgr = Master::GetInstance().GetNetMgr(); 
	int ret = net_mgr.StartTcpServer(bind_ip_str, port); 

	lua_pushnumber(L, ret); 

	return 1; 
}

int CStartTcpClient(lua_State *L)
{
	const char* remote_ip_str = luaL_checkstring(L, 1); 
	unsigned short remote_port = (unsigned short)luaL_checkinteger(L, 2); 

	NetMgr& net_mgr = Master::GetInstance().GetNetMgr(); 
	int ret = net_mgr.StartTcpClient(remote_ip_str, remote_port); 

	lua_pushnumber(L, ret); 

	return 1; 
}

int CSend(lua_State *L)
{
	int net_id = (int)luaL_checkinteger(L, 1); 
	const char* data = luaL_checkstring(L, 2); 
	unsigned int length = (int)luaL_checkinteger(L, 3); 

	NetMgr& net_mgr = Master::GetInstance().GetNetMgr(); 
	net_mgr.Send(net_id, data, length); 

	return 1; 
}

int CClose(lua_State *L)
{
	int net_id = (int)luaL_checkinteger(L, 1); 

	NetMgr& net_mgr = Master::GetInstance().GetNetMgr(); 
	net_mgr.Close(net_id); 

	return 1; 
}

void ExportCfunction(lua_State *L)
{
	lua_register(L, "CCloseSrv", CCloseSrv); 
	lua_register(L, "CStartTcpServer", CStartTcpServer); 
	lua_register(L, "CStartTcpClient", CStartTcpClient); 
	lua_register(L, "CSend", CSend); 
	lua_register(L, "CClose", CClose); 
}
