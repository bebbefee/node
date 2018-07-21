#include "netcore.h"
#include "inetcallback.h"
#include <iostream>

NetCore::NetCore(INetCallback* callback):callback(callback)
{
}

NetCore::~NetCore()
{
}

void NetCore::Init()
{
}

void NetCore::Run()
{
}

void NetCore::Update(int frame)
{
}

int NetCore::StartTcpServer(const char* bind_ip_str, unsigned short port)
{
	return 1; 
}

void NetCore::Send(int srv_id, int net_id, const char* data, unsigned int length)
{
}

void NetCore::Close(int srv_id, int net_id)
{
}
