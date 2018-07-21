#include "netmgr.h"
#include "master/master.h"

NetMgr::NetMgr():net_core(this)
{}

NetMgr::~NetMgr()
{}

void NetMgr::Init()
{
	net_core.Init(); 
}

void NetMgr::Run()
{
	net_core.Run(); 
}

void NetMgr::Update(int frame)
{
	net_core.Update(frame); 
}

int NetMgr::StartTcpServer(const char* bind_ip_str, unsigned short port)
{
	return net_core.StartTcpServer(bind_ip_str, port); 
}

void NetMgr::Send(int srv_id, int net_id, const char* data, unsigned int length)
{
	net_core.Send(srv_id, net_id, data, length); 
}

void NetMgr::Close(int srv_id, int net_id)
{
	net_core.Close(srv_id, net_id); 
}

void NetMgr::OnAccept(int srv_id, int net_id, unsigned int ip, unsigned short port)
{
	LuaMgr& lua_mgr = Master::GetInstance().GetLuaMgr(); 
	lua_mgr.OnAccept(srv_id, net_id, ip, port); 
}

void NetMgr::OnRecv(int srv_id, int net_id, const char* data, unsigned int length)
{
	LuaMgr& lua_mgr = Master::GetInstance().GetLuaMgr(); 
	lua_mgr.OnRecv(srv_id, net_id, data, length); 
}

void NetMgr::OnClose(int srv_id, int net_id)
{
	LuaMgr& lua_mgr = Master::GetInstance().GetLuaMgr(); 
	lua_mgr.OnClose(srv_id, net_id); 
}