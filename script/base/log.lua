g_log_lib = g_log_lib or {}
g_log_lib.level = {
    info = "INFO",
    debug = "DEBUG",
    error = "ERROR",
}

local print = print
g_log_lib.print = function(level, ...)
    local str = "[" .. os.date() .. " " .. level .. "] " .. table.concat({...}, "\t")
    if (level == g_log_lib.level.info) then
        os.execute([[echo "\033[38m]] .. str .. [[ \033[0m"]])
    elseif (level == g_log_lib.level.debug) then
        os.execute([[echo "\033[34m]] .. str .. [[ \033[0m"]])
    elseif (level == g_log_lib.level.error) then
        os.execute([[echo "\033[31m]] .. str .. debug.traceback() .. [[ \033[0m"]])
    end
end

function LOG_INFO(...)
    g_log_lib.print(g_log_lib.level.info, ...)
end

function LOG_DEBUG(...)
    g_log_lib.print(g_log_lib.level.debug, ...)
end

function LOG_ERROR(...)
    g_log_lib.print(g_log_lib.level.error, ...)
end
