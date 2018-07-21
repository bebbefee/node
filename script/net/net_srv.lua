NetSrv = NetSrv or {}; 

NetSrv.Update = function(time)
	
	if not NetSrv.srv_id then
		NetSrv.srv_id = CStartTcpServer("127.0.0.1", 8888); 
		LOG_DEBUG("conn_idx:", NetSrv.srv_id)
	end
end
