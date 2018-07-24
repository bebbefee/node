#ifndef _NETTASKCLOSE_H_
#define _NETTASKCLOSE_H_

#include "inettask.h"
#include "inetcallback.h"

class NetTaskClose: public INetTask
{
public:
	NetTaskClose(int net_id):net_id(net_id){}
	~NetTaskClose(){}
	void Fire(INetCallback* callback){callback->OnClose(net_id); } 

private:
	int net_id; 
}; 

#endif /* _NETTASKCLOSE_H_ */
