#include "netcore.h"
#include "inetcallback.h"
#include "inettask.h"
#include "nethandlersrv.h"
#include "nethandlerclient.h"

#include <sys/select.h>
#include "master/master.h"

#include <thread>
#include <vector>

NetCore::NetCore(INetCallback* callback):callback(callback)
{
}

NetCore::~NetCore()
{
}

void NetCore::Init()
{
}

void NetCore::Run()
{

	std::thread t(
		[this]{

			std::vector<unsigned int> can_read; 
			std::vector<unsigned int> can_write; 
			while (Master::GetInstance().GetIsRun())
			{
				if (this->handler_list.size() == 0)
				{
					std::this_thread::sleep_for(std::chrono::milliseconds(50)); 
					continue; 
				}

				this->PollSocket(can_read, can_write); 
				for (std::vector<unsigned int>::iterator i = can_read.begin(); i != can_read.end(); ++i)
				{
					this->handler_list[*i]->OnCanRead(); 
				}
				for (std::vector<unsigned int>::iterator i = can_write.begin(); i != can_write.end(); ++i)
				{
					this->handler_list[*i]->OnCanWrite(); 
				}

				can_read.clear(); 
				can_write.clear(); 
				this->MakeWorkTask(); 
			}
		}
	); 

	t.detach(); 
}

void NetCore::Update(int frame)
{
	while(!work_task.empty())
	{
		INetTask* task = work_task.front(); 
		task->Fire(callback); 
		work_task.pop_front(); 
	}
}

int NetCore::StartTcpServer(const char* bind_ip_str, unsigned short port, int backlog)
{
	NetHandlerSrv* hander_srv = new NetHandlerSrv(); 
	int _socket = hander_srv->Listen(bind_ip_str, port, backlog); 
	int net_id = -1; 
	if (_socket != -1)
	{
		hander_srv->SetSocketId(_socket); 
		AddSocket(hander_srv); 
	}

	return net_id; 
}

void NetCore::Send(int net_id, const char* data, unsigned int length)
{

	((NetHandlerClient*)handler_list[net_id])->Send(data, length); 
}

void NetCore::Close(int net_id)
{
}

void NetCore::AddSocket(INetHandler* hander)
{
	int net_id = handler_list.insert(hander); 
	hander->SetNetCore(this); 
	hander->SetNetId(net_id); 

	FD_SET(hander->GetSocketId(), &fd_read); 
	FD_SET(hander->GetSocketId(), &fd_write); 
	max_fd = hander->GetSocketId(); 
}

void NetCore::RemoveSocket(INetHandler* hander)
{
	FD_CLR(hander->GetSocketId(), &fd_read); 
	FD_CLR(hander->GetSocketId(), &fd_write); 
}

void NetCore::PollSocket(std::vector<unsigned int> &can_read, std::vector<unsigned int> &can_write)
{
	struct timeval tv; 
	tv.tv_sec = 0; 
	tv.tv_usec = 10000; 

	fd_read_tmp = fd_read; 
	fd_write_tmp = fd_write; 
	int ret = select(max_fd + 1, &fd_read_tmp, &fd_write_tmp, 0, &tv); 
	if (ret > 0)
	{
		for (int i = 0; i < handler_list.size(); ++i)
		{
			int _socket = handler_list[i]->GetSocketId(); 
			if (FD_ISSET(_socket, &fd_read_tmp))
			{
				can_read.push_back(i); 
			}

			if (FD_ISSET(_socket, &fd_write_tmp))
			{
				can_write.push_back(i); 
			}
		}
	}
}

void NetCore::MakeWorkTask()
{
	while(!core_task.empty())
	{
		INetTask* task = core_task.front(); 
		work_task.push_back(task); 
		core_task.pop(); 
	}
}

