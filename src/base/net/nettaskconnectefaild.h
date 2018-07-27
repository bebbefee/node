#ifndef _NETTASKCONNECTEFAILD_H_
#define _NETTASKCONNECTEFAILD_H_

#include "inettask.h"
#include "inetcallback.h"

class NetTaskConnecteFaild: public INetTask
{
public:
	NetTaskConnecteFaild(int connect_id):connect_id(connect_id){}
	~NetTaskConnecteFaild(){}
	void Fire(INetCallback* callback){callback->OnConnecteFaild(connect_id); } 

private:
	int connect_id; 
}; 

#endif /* _NETTASKCONNECTEFAILD_H_ */
