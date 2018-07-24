#ifndef _NETHANDLERSRV_H_
#define _NETHANDLERSRV_H_

#include "inethandler.h"

class NetHandlerSrv: public INetHandler
{
public:
	NetHandlerSrv():INetHandler(NetHandlerType::NH_SRV){}

	int Listen(const char* bind_ip_str, unsigned short port, int backlog); 

	void OnCanRead(); 
}; 

#endif /* _NETHANDLERSRV_H_ */
