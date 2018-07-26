#ifndef _INETHANDLER_H_
#define _INETHANDLER_H_

enum NetHandlerType
{
	NH_SRV,
	NH_CLIENT,
	NH_CONNECT,
}; 

class NetCore; 

class INetHandler
{
public:
	INetHandler(NetHandlerType handler_type):handler_type(handler_type){}
	virtual ~INetHandler(){}
	virtual void OnCanRead(){}
	virtual void OnCanWrite(){}
	virtual void OnClose(){}

	void SetNetCore(NetCore* net_core){this->net_core = net_core; }
	NetCore* GetNetCore(){return net_core; }

	void SetNetId(int net_id){this->net_id = net_id; }
	int GetNetId(){return net_id; }

	void SetSocketId(int _socket){this->_socket = _socket; }
	int GetSocketId(){return _socket; }

	NetHandlerType GetHandlerType(){return handler_type; }

protected:
	NetHandlerType handler_type; 
	NetCore* net_core; 
	int _socket; 
	int net_id; 
}; 

#endif /* _INETHANDLER_H_ */
