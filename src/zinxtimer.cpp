#include "zinxtimer.hpp"
#include <sys/timerfd.h>
#include <functional>
#include <thread>

ZinxTimer::ZinxTimer() {
    m_timerFd = -1;
}

// 初始化定时器类
bool ZinxTimer::Init()
{
    bool bRet = false;
    // 创建定时器文件描述符
    int tfd = timerfd_create(CLOCK_MONOTONIC, 0);
    if(tfd >= 0)
    {
        // 创建文件描述符成功
        
        // 设置定时周期
        struct itimerspec period;
        period.it_interval = {1, 0};
        period.it_value = {1, 0};
        if(timerfd_settime(tfd, 0, &period, nullptr) == 0) {
            // 设置定时器成功
            bRet = true;
            m_timerFd = tfd;
        }

    }

    return bRet;
}

// 读超时次数
bool ZinxTimer::ReadFd(std::string& _input)
{
    bool bRet = false;

    // read 读取超时次数
    uint64_t cnt = 0;
    if(read(m_timerFd, &cnt, sizeof(cnt)) == sizeof(uint64_t)) {
        // 读取成功
        // 写入到 _input 中
        _input = std::to_string(cnt);

        bRet = true;
    }
    return bRet;
}

bool ZinxTimer::WriteFd(std::string& _output)
{
    return false;
}

// 关闭文件描述符
void ZinxTimer::Fini()
{
    if(m_timerFd != -1) {
        close(m_timerFd);
        m_timerFd = -1;
    }
}

int ZinxTimer::GetFd()
{
    return m_timerFd;
}

std::string ZinxTimer::GetChannelInfo()
{
    return "timerfd";
}

AZinxHandler* ZinxTimer::GetInputNextStage(BytesMsg& _oInput)
{
    return TimerManage::getInstance();
}


IZinxMsg* OutPutHandler::InternelHandle(IZinxMsg& _oInput)
{
    // 打印数据
    std::cout << "hello world" << std::endl;
    return nullptr;
}

AZinxHandler* OutPutHandler::GetNextHandler(IZinxMsg& _oNextMsg)
{
    return nullptr;
}

TimerManage::TimerManage() {
    m_curIndex = 0;
    
    m_taskLists.resize(N);
}

TimerManage* TimerManage::getInstance() {

    static TimerManage m;
    return &m;
}

IZinxMsg* TimerManage::InternelHandle(IZinxMsg& _oInput) {


    if(m_taskLists[m_curIndex].size() == 0) {
        // 移动刻度即可
        m_curIndex = (m_curIndex + 1) % N;
    }


    std::vector<std::function<void()>> funcLists;

    for(auto it = m_taskLists[m_curIndex].begin(); it != m_taskLists[m_curIndex].end(); ++it) {

        if((*it)->m_count == 0) {
            // 缓存执行任务
            funcLists.push_back(std::bind(&TimerTask::processTask, (*it)));
        }
        --(*it)->m_count;
    }



    for(auto it = m_taskLists[m_curIndex].begin(); it != m_taskLists[m_curIndex].end(); ++it) {

       if((*it)->m_count < 0) {

        // // 将该任务添加到新的刻度中
        // this->addTask((*it));
        // // 执行完删除该刻度
        // it = m_taskLists[m_curIndex].erase(it);
        auto tmpit = *it;
        it = m_taskLists[m_curIndex].erase(it);
        if(tmpit->getTimeoutSec() == 0) {
            tmpit->m_count = tmpit->getTimeoutSec() / N;
            TimerManage::getInstance()->getTasks()[(m_curIndex + 1) % N].push_back(tmpit);
        }
        else {
             this->addTask(tmpit);
        }
       }

    }

    // 执行存放的任务
    for(const auto& fun : funcLists) {
        std::cout << m_curIndex << " ";
        fun();
    }

    // 移动刻度即可
    m_curIndex = (m_curIndex + 1) % N;

    return nullptr;
}

AZinxHandler* TimerManage::GetNextHandler(IZinxMsg& _oNextMsg) {

    return nullptr;
}

// 添加任务 
void TimerManage::addTask(TimerTask* task) {


    // 当前任务所在刻度
    int index = (task->getTimeoutSec() + m_curIndex) % N;

    // 当前任务的圈数
    int count = task->getTimeoutSec() / N;
    task->m_count = count;

    // 将任务添加到时间轮
    m_taskLists[index].push_back(task);
}

void TimerManage::delTask(TimerTask* task) {

    for(auto& list : m_taskLists) {
        for(auto& tasks : list) {
            if(tasks == task) {
                list.remove(task);
                return;
            }
        }
    }

}

std::vector<std::list<TimerTask*>>& TimerManage::getTasks() {

    return m_taskLists;
}

