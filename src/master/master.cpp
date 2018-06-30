#include "master.h"
#include <thread>

void Master::Init()
{
	frame = 0; 
	is_run = true; 
	
	lua_mgr.Init(); 
	cmd.Init(); 
}

void Master::Run()
{
	lua_mgr.Run(); 
	cmd.Run(); 

	Update(); 
}

void Master::Update()
{
	while (is_run)
	{
		++frame; 

		lua_mgr.Update(frame);
		cmd.Update(frame); 

		std::this_thread::sleep_for(std::chrono::milliseconds(50));
	}
}
