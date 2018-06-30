#include "lua_export.h"
#include "master/master.h"

int CCloseSrv(lua_State *L)
{
	Master& master = Master::GetInstance(); 
	master.SetIsRun(false); 

	return 0; 
}

void ExportCfunction(lua_State *L)
{
	lua_register(L, "CCloseSrv", CCloseSrv); 
}
