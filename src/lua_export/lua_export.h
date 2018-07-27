#ifndef _LUA_EXPORT_H_
#define _LUA_EXPORT_H_

extern "C" {
	#include "lua.h"
}

void ExportCfunction(lua_State* L); 

#endif /* _LUA_EXPORT_H_ */
