#ifndef _LUA_EXPORT_H_
#define _LUA_EXPORT_H_

extern "C" {
	#include "lua.h"
}

int CCloseSrv(lua_State *L); 

void ExportCfunction(lua_State *L); 

#endif /* _LUA_EXPORT_H_ */
