#include "nethandlersrv.h"
#include "nethandlerclient.h"
#include "inetcallback.h"
#include "netcore.h"

#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>

int setnonblocking(int _socket)
{
	int opts = fcntl(_socket, F_GETFL); 
	if (opts < 0)
	{
		printf("fcntl failed %s\n", strerror(errno)); 
		return -1; 
	}

	opts = opts | O_NONBLOCK; 
	if (fcntl(_socket, F_SETFL, opts) < 0)
	{
		printf("fcntl failed %s\n", strerror(errno)); 
		return -1; 
	}

	return 0; 
}

int NetHandlerSrv::Listen(const char* bind_ip_str, unsigned short port, int backlog)
{
	int on = 1; 
	int _socket = socket(AF_INET, SOCK_STREAM, 0); 
	if (setsockopt(_socket, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) == -1)
	{
		printf("setsockopt failed %s\n", strerror(errno)); 
		return -1; 
	}

	struct sockaddr_in addr; 
	memset(&addr, 0, sizeof(addr)); 
	addr.sin_family = AF_INET; 
	addr.sin_port = htons(port); 
	addr.sin_addr.s_addr = htonl(INADDR_ANY); 

	if (bind_ip_str != 0)
	{
		unsigned long ip = inet_addr(bind_ip_str); 
		if (ip != INADDR_NONE)
		{
			addr.sin_addr.s_addr = ip; 
		}
	}

	if (bind(_socket, (struct sockaddr *)&addr, sizeof(addr)) == -1)
	{
		printf("bind port %d failed %s\n", port, strerror(errno)); 
		return -1; 
	}

	if (listen(_socket, backlog) == -1)
	{
		printf("listen failed %s\n", strerror(errno)); 
		return -1; 
	}

	if (setnonblocking(_socket) == -1)
	{
		return -1; 
	}

	this->_socket = _socket; 

	return _socket; 
}

void NetHandlerSrv::OnCanRead()
{
	for (;;)
	{
		struct sockaddr_in addr; 
		socklen_t len = sizeof(addr); 
		int _socket = accept(this->_socket, (struct sockaddr *)&addr, &len); 
		if (_socket == -1)
			return; 

		NetHandlerClient* hander_client = new NetHandlerClient(); 
		hander_client->SetSocketId(_socket); 
		this->net_core->AddSocket(hander_client); 

		getpeername(_socket, (struct sockaddr *)&addr, &len); 
		unsigned int ip = ntohl(addr.sin_addr.s_addr); 
		unsigned short port = ntohs(addr.sin_port); 

		this->net_core->GetCallBack()->OnAccept(this->GetNetId(), hander_client->GetNetId(), ip, port); 
	}
}
