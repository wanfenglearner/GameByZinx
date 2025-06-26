#ifndef __ZINXTIMER_H__
#define __ZINXTIMER_H__

#include <zinx.h>
#include <vector>
#include <list>


const int N = 10;	// 时间轮刻度

class ZinxTimer : public Ichannel
{
public:
	ZinxTimer();
	~ZinxTimer() = default;
	// 初始化定时器类
	bool Init() ;

	// 读超时次数
	bool ReadFd(std::string& _input) ;

	bool WriteFd(std::string& _output) ;

	// 关闭文件描述符
	void Fini() ;

	int GetFd() ;

	std::string GetChannelInfo() ;

	AZinxHandler* GetInputNextStage(BytesMsg& _oInput) ;
private:
	int m_timerFd;
};

class OutPutHandler : public AZinxHandler {

public:
	IZinxMsg* InternelHandle(IZinxMsg& _oInput);

	AZinxHandler* GetNextHandler(IZinxMsg& _oNextMsg);
	static OutPutHandler* getInstance() {
		static OutPutHandler p;
		return &p;
	}
};


// 定时器任务
class TimerTask {

public:
	TimerTask() = default;
	virtual ~TimerTask() = default;

	// 执行任务
	virtual void processTask() = 0;
	// 得到每个任务的超时次数
	virtual int getTimeoutSec() = 0;
	
	// 每个任务圈数
	int m_count;

};


// 定时器处理类
class TimerManage : public AZinxHandler {

public:
	TimerManage();

	static TimerManage* getInstance();
	IZinxMsg* InternelHandle(IZinxMsg& _oInput);

	AZinxHandler* GetNextHandler(IZinxMsg& _oNextMsg);
	
	// 添加任务 
	void addTask(TimerTask* task);
	void delTask(TimerTask* task);

	std::vector<std::list<TimerTask*>>& getTasks();

	// 时间轮现在所指的刻度
	int m_curIndex;
private:

	// 存放管理的任务
	//std::vector<TimerTask*> m_tasks;
	std::vector<std::list<TimerTask*>> m_taskLists;
};


#endif 