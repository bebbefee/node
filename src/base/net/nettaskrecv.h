#ifndef _NETTASKRECV_H_
#define _NETTASKRECV_H_

#include "inettask.h"
#include "inetcallback.h"

class NetTaskRecv: public INetTask
{
public:
	NetTaskRecv(int net_id, const char* data, unsigned int length):net_id(net_id),data(data),length(length){}
	~NetTaskRecv(){}
	void Fire(INetCallback* callback){callback->OnRecv(net_id, data, length); } 

private:
	int net_id; 
	const char* data; 
	unsigned int length; 
}; 

#endif /* _NETTASKRECV_H_ */
