#ifndef _NETHANDLERCLIENT_H_
#define _NETHANDLERCLIENT_H_

#include "inethandler.h"

class NetHandlerClient: public INetHandler
{
public:
	NetHandlerClient():INetHandler(NetHandlerType::NH_CLIENT){}

	void OnCanRead(); 
	void OnCanWrite(); 
	void OnClose(); 
	
	void Send(const char* data, unsigned int length); 
}; 

#endif /* _NETHANDLERCLIENT_H_ */
