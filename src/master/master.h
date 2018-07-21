#ifndef _MASTER_H_
#define _MASTER_H_

#include "luamgr/luamgr.h"
#include "netmgr/netmgr.h"
#include "cmd/cmd.h"

class Master
{
public:
	static Master& GetInstance()
	{
		static Master instance; 
		return instance; 
	}

	void Init(); 
	
	void Run(); 

	void Update(); 

	inline bool GetIsRun() {return is_run; }
	inline void SetIsRun(bool is_run) {this->is_run = is_run; }

	inline LuaMgr& GetLuaMgr() {return lua_mgr; }
	inline NetMgr& GetNetMgr() {return net_mgr; }
	inline Cmd& GetCmd() {return cmd; }

private:
	LuaMgr lua_mgr; 
	NetMgr net_mgr; 
	Cmd cmd; 
	int frame; 
	bool is_run; 
}; 

#endif /* _MASTER_H_ */
