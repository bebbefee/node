#ifndef _NETMGR_H_
#define _NETMGR_H_

#include "base/net/netcore.h"
#include "base/net/inetcallback.h"

class NetMgr: public INetCallback
{
public:
	NetMgr(); 
	~NetMgr(); 

	void Init(); 
	
	void Run(); 

	void Update(int frame); 

	int StartTcpServer(const char* bind_ip_str, unsigned short port); 
	int StartTcpClient(const char* remote_ip_str, unsigned short remote_port); 
	
	void Send(int net_id, const char* data, unsigned int length); 

	void Close(int net_id); 

	void OnAccept(int srv_id, int net_id, unsigned int ip, unsigned short port); 

	void OnRecv(int net_id, const char* data, unsigned int length); 

	void OnClose(int net_id); 

private:

	NetCore net_core; 
}; 

#endif /* _NETMGR_H_ */
