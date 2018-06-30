#include "cmd.h"
#include "master/master.h"
#include <thread>
#include <iostream>
#include <string>

void Cmd::Init()
{

}

void Cmd::Run()
{
	std::thread t(
		[this]{
			while (Master::GetInstance().GetIsRun())
			{
				std::string line; 
				getline(std::cin, line, '\n'); 
				this->cmd_queue.push_back(line); 
			}
		}
	); 

	t.detach(); 
}

void Cmd::Update(int frame)
{
	LuaMgr& lua_mgr = Master::GetInstance().GetLuaMgr(); 
	while (!cmd_queue.empty())
	{

		std::string& cmd = cmd_queue.front(); 
		cmd_queue.pop_front(); 
		lua_mgr.OnCmd(cmd.c_str()); 
	}
}
