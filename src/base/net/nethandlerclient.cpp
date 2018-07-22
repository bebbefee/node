#include "nethandlerclient.h"
#include <arpa/inet.h>

void NetHandlerClient::OnCanRead()
{
}

void NetHandlerClient::OnCanWrite()
{
}

void NetHandlerClient::OnClose()
{
}

void NetHandlerClient::Send(const char* data, unsigned int length)
{
	send(_socket, data, length, 0); 
}
