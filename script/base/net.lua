function OnAccept(srv_id, net_id, ip, port)
	LOG_DEBUG("OnAccept:", srv_id, net_id, ip, port)
end

function OnRecv(srv_id, net_id, data, length)
	LOG_DEBUG("OnRecv:", srv_id, net_id, data, length)
end

function OnClose(srv_id, net_id)
	LOG_DEBUG("OnClose:", srv_id, net_id)
end
