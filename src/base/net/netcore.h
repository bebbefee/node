#ifndef _NETCORE_H_
#define _NETCORE_H_

class INetCallback; 

class NetCore
{
public:
	NetCore(INetCallback* callback); 
	~NetCore(); 

	void Init(); 
	
	void Run(); 

	void Update(int frame); 

	int StartTcpServer(const char* bind_ip_str, unsigned short port); 

	void Send(int srv_id, int net_id, const char* data, unsigned int length); 
	
	void Close(int srv_id, int net_id); 

private:
	int _socket; 
	INetCallback* callback; 
}; 

#endif /* _NETCORE_H_ */
