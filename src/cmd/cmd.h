#ifndef _CMD_H_
#define _CMD_H_

#include <string>
#include "base/thread_queue.h"

class Cmd
{
public:
	void Init(); 
	
	void Run(); 

	void Update(int frame); 

private:
	ThreadQueue<std::string> cmd_queue; 
}; 

#endif /* _CMD_H_ */
