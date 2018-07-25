#ifndef _NETHANDLERCLIENT_H_
#define _NETHANDLERCLIENT_H_

#include <cstdint>
#include <mutex>
#include "inethandler.h"

struct MsgBlock
{
	int32_t length; 
	char* data; 
};

class NetHandlerClient: public INetHandler
{
public:
	NetHandlerClient(); 

	void OnCanRead(); 
	void OnCanWrite(); 
	void OnClose(); 
	
	bool Send(const char* data, unsigned int length); 

	void ReAllocWriteBuff(int length); 

private:
	int curr_read_length; 
	MsgBlock read_msg_block; 

	char* wait_write_buff; 
	int wait_write_length; 
	int wait_write_max; 

	char* write_buff; 
	int write_length; 
	int write_max; 
	int curr_write_length; 
	std::mutex write_mutex; 

}; 

#endif /* _NETHANDLERCLIENT_H_ */
