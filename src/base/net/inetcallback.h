#ifndef _INETCALLBACK_H_
#define _INETCALLBACK_H_

class INetCallback
{
public:
	INetCallback(){}
	virtual ~INetCallback(){}
	virtual void OnAccept(int srv_id, int net_id, unsigned int ip, unsigned short port)=0; 
	virtual void OnRecv(int net_id, const char* data, unsigned int length)=0; 
	virtual void OnClose(int net_id)=0; 
	virtual void OnConnected(int connect_id, int net_id)=0; 
	virtual void OnConnecteFaild(int connect_id)=0; 
}; 

#endif /* _INETCALLBACK_H_ */
