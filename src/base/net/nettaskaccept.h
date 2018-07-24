#ifndef _NETTASKACCEPT_H_
#define _NETTASKACCEPT_H_

#include "inettask.h"
#include "inetcallback.h"

class NetTaskAcept: public INetTask
{
public:
	NetTaskAcept(int srv_id, int net_id, unsigned int ip, unsigned short port):
	srv_id(srv_id), net_id(net_id), ip(ip), port(port){}
	~NetTaskAcept(){}
	void Fire(INetCallback* callback){callback->OnAccept(srv_id, net_id, ip, port); } 

private:
	int srv_id; 
	int net_id; 
	unsigned int ip; 
	unsigned short port; 
}; 

#endif /* _NETTASKACCEPT_H_ */
