#ifndef _NETTASKCONNECTED_H_
#define _NETTASKCONNECTED_H_

#include "inettask.h"
#include "inetcallback.h"

class NetTaskConnected: public INetTask
{
public:
	NetTaskConnected(int connect_id, int net_id):connect_id(connect_id),net_id(net_id){}
	~NetTaskConnected(){}
	void Fire(INetCallback* callback){callback->OnConnected(connect_id, net_id); } 

private:
	int connect_id; 
	int net_id; 
}; 

#endif /* _NETTASKCONNECTED_H_ */
