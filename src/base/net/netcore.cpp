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
				CleanDirtySocket(); 
				this->MakeWorkTask(); 
			}
		}
	); 

	t.detach(); 
}

void NetCore::Update(int frame)
{
	std::lock_guard<std::mutex> g1(m1); 
	std::lock_guard<std::mutex> g2(m2); 
	INetTask* task; 
	while(work_task.pop(task))
	{
		task->Fire(callback); 
		delete task; 
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
		AddNetHandler(hander_srv); 
		net_id = hander_srv->GetNetId(); 
	}

	return net_id; 
}

void NetCore::Send(int net_id, const char* data, unsigned int length)
{
	std::lock_guard<std::mutex> g(handler_list_mutex); 
	auto hander_iter = handler_list.find(net_id); 
	if (hander_iter != handler_list.end() && (*hander_iter)->GetHandlerType() == NetHandlerType::NH_CLIENT)
	{
		((NetHandlerClient*)(*hander_iter))->Send(data, length); 
	}
}

void NetCore::Close(int net_id)
{
	PushDirtySocket(net_id); 
}

void NetCore::PushDirtySocket(int net_id)
{
	std::lock_guard<std::mutex> g(dirty_net_id_mutex); 
	dirty_net_id.push_back(net_id); 
}

void NetCore::CleanDirtySocket()
{
	std::vector<int> temp; 
	{
		std::lock_guard<std::mutex> g(dirty_net_id_mutex); 
		dirty_net_id.swap(temp); 
	}

	for (std::vector<int>::iterator iter = temp.begin(); iter != temp.end(); ++iter)
	{
		auto hander_iter = handler_list.find(*iter); 
		if (hander_iter == handler_list.end())
		{
			continue; 
		}

		int _socket = (*hander_iter)->GetSocketId(); 
		(*hander_iter)->OnClose(); 
		RemoveNetHandler(*hander_iter); 
		RemoveSocket(_socket); 
	}
}

void NetCore::AddNetHandler(INetHandler* hander)
{
	int net_id = 0; 
	{
		std::lock_guard<std::mutex> g(handler_list_mutex); 
		net_id = handler_list.insert(hander); 
	}
	hander->SetNetCore(this); 
	hander->SetNetId(net_id); 

	AddSocket(hander->GetSocketId()); 
}

void NetCore::RemoveNetHandler(INetHandler* hander)
{
	std::lock_guard<std::mutex> g(handler_list_mutex); 
	handler_list.erase(hander->GetNetId()); 
	delete hander; 
}

void NetCore::AddSocket(int _socket)
{
	FD_SET(_socket, &fd_read); 
	FD_SET(_socket, &fd_write); 

	max_fd = max_fd > _socket ? max_fd : _socket; 
}

void NetCore::RemoveSocket(int _socket)
{
	FD_CLR(_socket, &fd_read); 
	FD_CLR(_socket, &fd_write); 

	if (_socket == max_fd)
	{
		max_fd = -1; 
		for (IdVector<INetHandler*>::iterator itr = handler_list.begin(); itr != handler_list.end(); ++itr)
		{
			int tmp_socket = (*itr)->GetSocketId(); 
			max_fd = max_fd > tmp_socket ? max_fd : tmp_socket; 
		}
	}
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
		for (IdVector<INetHandler*>::iterator itr = handler_list.begin(); itr != handler_list.end(); ++itr)
		{
			int _socket = (*itr)->GetSocketId(); 
			if (FD_ISSET(_socket, &fd_read_tmp))
			{
				can_read.push_back((*itr)->GetNetId()); 
			}
			if (FD_ISSET(_socket, &fd_write_tmp))
			{
				can_write.push_back((*itr)->GetNetId()); 
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
