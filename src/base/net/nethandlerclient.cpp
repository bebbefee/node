#include "nethandlerclient.h"
#include <arpa/inet.h>
#include <errno.h>
#include "stdio.h"

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

		}

		
	}
}

void NetHandlerClient::OnCanWrite()
{
	printf("OnCanWrite\n");
}

void NetHandlerClient::OnClose()
{
}

void NetHandlerClient::Send(const char* data, unsigned int length)
{
	send(_socket, data, length, 0); 
}
