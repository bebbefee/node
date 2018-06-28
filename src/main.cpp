#include <iostream>
#include <string>
#include <thread>
#include <csignal>
#include "thread_queue.h"

extern "C" {  
    #include "lua.h"  
    #include "lualib.h"  
    #include "lauxlib.h"  
} 

int main(void)
{
    signal(SIGINT, [](int){}); 
    
    lua_State* L = luaL_newstate(); 
    luaL_openlibs(L);
    if (luaL_dofile(L, "main.lua"))
    {
        const char* error_msg = lua_tostring(L, -1); 
        std::cout << "luaL_dofile(L, \"main.lua\") Error:" << error_msg << std::endl; 
        lua_pop(L, 1); 
    }

    ThreadQueue<std::string> cmd_queue; 

    std::thread t1(
        [L, &cmd_queue]{
            int frame = 0; 
            for(;;)
            {
                if (cmd_queue.empty() == false)
                {
                    std::string& cmd = cmd_queue.front(); 
                    cmd_queue.pop_front(); 
                    if (cmd == "reload")
                    {
                        if (luaL_dofile(L, "main.lua"))
                        {
                            const char* error_msg = lua_tostring(L, -1); 
                            std::cout << "luaL_dofile(L, \"main.lua\") Error:" << error_msg << std::endl; 
                            lua_pop(L, 1); 
                        }
                    }
                    else if (cmd == "stop")
                    {
                        return; 
                    }
                    lua_getglobal(L, "OnCmd"); 
                    lua_pushstring(L, cmd.c_str()); 
                    if (lua_pcall(L, 1, 0, 0))
                    {
                        const char* error_msg = lua_tostring(L, -1); 
                        std::cout << "call OnCmd(cmd) Error:" << error_msg << std::endl; 
                        lua_pop(L, 1); 
                    } 
                }
                lua_getglobal(L, "Update"); 
                lua_pushinteger(L, frame++); 
                if(lua_pcall(L, 1, 0, 0))
                {
                    const char* error_msg = lua_tostring(L, -1); 
                    std::cout << "call Update(frame) Error:" << error_msg << std::endl; 
                    lua_pop(L, 1); 
                }

                std::this_thread::sleep_for(std::chrono::milliseconds(50));
            }
        }
    ); 
    t1.detach(); 

    for(;;)
    {
        std::string line; 
        getline(std::cin, line, '\n'); 
        cmd_queue.push_back(line); 
        if (line == "stop")
        {
            break; 
        }
    }
    
    std::cout << "end." << std::endl; 

    return 0; 
}
