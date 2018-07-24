#ifndef _INETTASK_H_
#define _INETTASK_H_

class INetCallback; 

class INetTask
{
public:
	INetTask(){}
	virtual ~INetTask(){}
	virtual void Fire(INetCallback* callback)=0; 
}; 

#endif /* _INETTASK_H_ */
