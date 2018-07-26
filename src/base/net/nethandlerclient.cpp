#include "nethandlerclient.h"
#include <arpa/inet.h>
#include <errno.h>
#include "netcore.h"
#include "nettaskclose.h"
#include "nettaskrecv.h"


NetHandlerClient::NetHandlerClient():INetHandler(NetHandlerType::NH_CLIENT)
{
	curr_read_length = -(int)sizeof(int32_t); 
	curr_write_length = 0; 

	wait_write_buff = new char[2048]; 
	wait_write_length = 0; 
	wait_write_max = 2048; 

	write_buff = new char[2048]; 
	write_length = 0; 
	write_max = 2048; 
}

void NetHandlerClient::OnCanRead()
{
	if (GetSocketId() == -1)
	{
		return; 
	}

	int ret = 0; 
	char read_buff[2048]; 
	while (true)
	{
		ret = recv(_socket, read_buff, 2048, 0); 
		if (ret == -1 and errno == EWOULDBLOCK)
		{
			return; 
		}

		if (ret <= 0)
		{
			net_core->PushDirtySocket(net_id); 
			return; 
		}

		int not_read = ret; 
		while (not_read > 0)
		{
			if (curr_read_length < 0)
			{
				int left_msg = -curr_read_length; 
				int need_read = not_read > left_msg ? left_msg : not_read; 
				memcpy((char*)&read_msg_block.length + sizeof(int32_t) + curr_read_length, read_buff + ret - not_read, need_read); 
				curr_read_length += need_read; 
				not_read -= need_read; 

				if (curr_read_length < 0)
				{
					break; 
				}

				if (read_msg_block.length < 0 || read_msg_block.length > 4096)
				{
					net_core->PushDirtySocket(net_id); 
					return; 
				}

				read_msg_block.data = new char[read_msg_block.length]; 
				if (not_read == 0)
				{
					break; 
				}
			}

			int left_msg = read_msg_block.length - curr_read_length; 
			int need_read = not_read > left_msg ? left_msg : not_read; 
			memcpy(read_msg_block.data + curr_read_length, read_buff + ret - not_read, need_read); 
			curr_read_length += need_read; 
			not_read -= need_read; 

			if (curr_read_length < read_msg_block.length)
			{
				break; 
			}

			NetTaskRecv* task = new NetTaskRecv(this->GetNetId(), read_msg_block.data, read_msg_block.length); 
			this->net_core->PushCoreTask(task); 

			curr_read_length = -(int)sizeof(int32_t); 
			read_msg_block.length = 0; 
			read_msg_block.data = 0; 
		}
	}
}

void NetHandlerClient::OnCanWrite()
{
	if (GetSocketId() == -1)
	{
		return; 
	}

	while (true)
	{
		if (write_length == 0)
		{
			{
				std::lock_guard<std::mutex> g(write_mutex); 
				char* tmp_buff = wait_write_buff; 
				wait_write_buff = write_buff; 
				write_buff = tmp_buff; 

				int tmp_max_length = wait_write_max; 
				wait_write_max = write_max; 
				write_max = tmp_max_length; 

				write_length = wait_write_length; 
				wait_write_length = 0; 
			}

			if (write_length == 0)
			{
				return; 
			}
		}

		int ret = 0; 
		while (curr_write_length < write_length)
		{
			ret = send(_socket, write_buff + curr_write_length, write_length - curr_write_length, 0); 
			
			if (ret == -1 and errno == EWOULDBLOCK)
			{
				return; 
			}

			if (ret <= 0)
			{
				net_core->PushDirtySocket(net_id); 
				return; 
			}
			curr_write_length += ret; 
		}

		write_length = 0; 
		curr_write_length = 0; 
	}
}

void NetHandlerClient::OnClose()
{

	NetTaskClose* task = new NetTaskClose(this->GetNetId()); 
	this->net_core->PushCoreTask(task); 
	this->SetSocketId(-1); 
}

bool NetHandlerClient::Send(const char* data, unsigned int length)
{
	std::lock_guard<std::mutex> g(write_mutex); 
	int total_length = wait_write_length + sizeof(int32_t) + length; 
	while (total_length > wait_write_max)
	{
		if (total_length > 1024 * 1024 * 16)
		{
			printf("Send msg is big than 16M, net_id:%d. Send cancel.\n", this->GetNetId()); 
			fflush(stdout); 

			return false; 
		}

		ReAllocWriteBuff(static_cast<int>(wait_write_max * 1.5)); 
	}

	char* point = wait_write_buff + wait_write_length; 
	int32_t* data_length = (int32_t*)point; 
	*data_length = length; 
	memcpy(point + sizeof(int32_t), data, length); 

	wait_write_length = total_length; 

	return true; 
}


void NetHandlerClient::ReAllocWriteBuff(int length)
{
	char* data = new char[length]; 
	if (data != 0)
	{
		memcpy(data, wait_write_buff, wait_write_length); 
		delete wait_write_buff; 
	}

	wait_write_buff = data; 
	wait_write_max = length; 
}
