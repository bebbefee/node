NetSrv = NetSrv or {}; 

NetSrv.Update = function(time)
	
	if not NetSrv.srv_id then
		NetSrv.srv_id = CStartTcpServer("127.0.0.1", 8888)
		LOG_DEBUG("conn_idx:", NetSrv.srv_id)
	else

		for k,v in pairs(g_accept) do
			local str = "netid:" .. k .. ", time:" .. time .. "\n"; 
			CSend(k, str, string.len(str))
		end
	end

	if not NetSrv.client then
		NetSrv.client = CStartTcpClient("127.0.0.1", 8888)
		LOG_DEBUG("net client:", NetSrv.client)
	end
end
