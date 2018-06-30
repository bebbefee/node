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

	void OnCmd(const char* cmd); 
	
private:
	lua_State* L; 
}; 

#endif /* _LUAMGR_H_ */
