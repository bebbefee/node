#ifndef _LUAMGR_H_
#define _LUAMGR_H_

#include <string>
extern "C" {
	#include "lua.h"
	#include "lualib.h"
	#include "lauxlib.h"
}

class LuaMgr
{
public:
	LuaMgr(); 
	~LuaMgr(); 

	void Init(); 
	
	void Run(); 

	void Update(int frame); 
	
	void OnAccept(int srv_id, int net_id, unsigned int ip, unsigned short port); 

	void OnRecv(int net_id, const char* data, unsigned int length); 

	void OnClose(int net_id); 

	void OnCmd(const char* cmd); 
	
private:
	lua_State* L; 
}; 

#endif /* _LUAMGR_H_ */
