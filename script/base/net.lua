function OnAccept(srv_id, net_id, ip, port)
	LOG_DEBUG("OnAccept:", srv_id, net_id, ip, port)
end

function OnRecv(net_id, data, length)
	LOG_DEBUG("OnRecv:", net_id, data, length)
end

function OnClose(net_id)
	LOG_DEBUG("OnClose:", net_id)
end
