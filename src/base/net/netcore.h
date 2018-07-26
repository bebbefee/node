#ifndef _NETCORE_H_
#define _NETCORE_H_

#include "base/idvector.h"
#include "base/thread_queue.h"
#include <vector>
#include <queue>

#include <netinet/in.h>
#include <errno.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/tcp.h>

class INetCallback; 
class INetHandler; 
class INetTask; 

struct ConnectStruct
{
	unsigned int remote_ip; 
	unsigned short remote_port; 
	unsigned int time_out; 
}; 

class NetCore
{
public:
	NetCore(INetCallback* callback); 
	~NetCore(); 

	void Init(); 
	
	void Run(); 

	void Update(int frame); 

	int StartTcpServer(const char* bind_ip_str, unsigned short port, int backlog=1024); 

	int StartTcpClient(const char* remote_ip_str, unsigned short remote_port, unsigned int time_out=3000); 

	void Send(int net_id, const char* data, unsigned int length); 
	
	void Close(int net_id); 

	void PushDirtySocket(int net_id); 
	void CleanDirtySocket(); 

	void AddNetHandler(INetHandler* hander); 
	void RemoveNetHandler(INetHandler* hander); 

	void AddSocket(int _socket); 
	void RemoveSocket(int _socket); 

	void PollSocket(std::vector<unsigned int> &can_read, std::vector<unsigned int> &can_write); 

	INetCallback* GetCallBack(){return callback; }

	void PushCoreTask(INetTask* task){ core_task.push(task); }
	void MakeWorkTask(); 
	
private:
	INetCallback* callback; 
	IdVector<INetHandler*> handler_list; 
	std::mutex handler_list_mutex; 

	std::queue<INetTask*> core_task; 
	ThreadQueue<INetTask*> work_task; 

	fd_set fd_read; 
	fd_set fd_write; 

	fd_set fd_read_tmp; 
	fd_set fd_write_tmp; 
	int max_fd; 

	std::vector<int> dirty_net_id; 
	std::mutex dirty_net_id_mutex; 

	ThreadQueue<ConnectStruct*> connect_struct; 
}; 

#endif /* _NETCORE_H_ */
