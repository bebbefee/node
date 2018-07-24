#include "nethandlerclient.h"
#include <arpa/inet.h>
#include <errno.h>
#include "netcore.h"
#include "nettaskclose.h"

void NetHandlerClient::OnCanRead()
{
	int ret = 0; 
	char read_buff[2048]; 

	for (;;)
	{
		ret = recv(_socket, read_buff, 2048, 0); 

		if (ret == -1 and errno == EWOULDBLOCK)
		{
			return; 
		}

		if (ret <= 0)
		{
			OnClose(); 
			net_core->RemoveSocket(this); 
			return; 
		}
	}
}

void NetHandlerClient::OnCanWrite()
{
}

void NetHandlerClient::OnClose()
{

	NetTaskClose* task = new NetTaskClose(this->GetNetId()); 
	this->net_core->PushCoreTask(task); 
}

void NetHandlerClient::Send(const char* data, unsigned int length)
{
	send(_socket, data, length, 0); 
}
