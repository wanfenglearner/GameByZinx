#include "gamerole.hpp"
#include "gameprotocol.hpp"
#include "gamechannel.hpp"
#include <set>
#include <algorithm>
#include <random>



// 创建唯一时间定时器对象
static ZinxTimer* zinx_timer = new ZinxTimer();

// 创建随机数引擎
static std::default_random_engine randomEngine((unsigned int)time(nullptr));

GameRole::GameRole() {

    // 新的对象连接,关闭定时器
    if(ZinxKernel::Zinx_GetAllRole().size() <= 0) {

        // 关闭定时器
        TimerManage::getInstance()->delTask(ExitTask::getInstance());
         ZinxKernel::Zinx_Del_Channel(*zinx_timer);
    }

    // 初始化位置信息 随机位置
    m_X = 100 + randomEngine() % 50; 
    m_Z = 100 + randomEngine() % 50; 
    m_Y = 0;
    m_V = randomEngine() % 360;
    m_blood = randomEngine() % 100;

    // 绑定处理函数

    // 聊天消息
    m_handlerLists.emplace(Game::MsgType::TYPE_CHAT_CONTENT, 
    std::bind(&GameRole::sendChatContent, this, std::placeholders::_1));

    // 位置移动
    m_handlerLists.emplace(Game::MsgType::TYPE_NEW_POSITION, 
    std::bind(&GameRole::movePosition, this, std::placeholders::_1));

}
bool GameRole::Init()
{
    // 初始化变量
    m_pid = m_gameProtocol->getGameChannel()->getFd();
    m_name = AOIWorld::getInstance()->getRandomName();


    // 将本玩家添加到游戏世界中
    AOIWorld::getInstance()->addAOIPlayer(this);

    // 向客户端发送ID和姓名
    GameData* data_id_name = this->createLoginData();
    ZinxKernel::Zinx_SendOut(*data_id_name, *m_gameProtocol);

    // 向客户端发送周围玩家的信息
    GameData* data_around_players = this->createAroundPos();
    ZinxKernel::Zinx_SendOut(*data_around_players, *m_gameProtocol);

    // 向周围玩家以及自己发送自己的位置信息
    auto players = AOIWorld::getInstance()->getAroundPlayer(this);
    for(const auto& player : players) {

        // 发送消息
        auto prole = dynamic_cast<GameRole*>(player);

        GameData* data_self_pos = this->createSelfPos();
        ZinxKernel::Zinx_SendOut(*data_self_pos, *(prole->getGameProtocol()));

    }

    return true;
}

// 处理消息的函数
UserData* GameRole::ProcMsg(UserData& _poUserData)
{
   GET_REF2DATA(MultiGameData, multidata, _poUserData);

   for(const auto& pmsg : multidata.getGameDataLists()) {

        char buf[1024] = {0};
        sprintf(buf, "id:%d name:%s 消息类型:%d 消息:\n%s血量:%lf", 
        m_pid, m_name.c_str(), pmsg->getMsgType(), pmsg->getMessage()->Utf8DebugString().c_str(), m_blood);
        
        std::cout << buf << std::endl;

        auto pit = this->getHandler(pmsg->getMsgType());
        pit(pmsg);
    }

    return nullptr;
}

void GameRole::Fini()
{

    // 向周围玩家发送离线消息
    auto players = AOIWorld::getInstance()->getAroundPlayer(this);

    for(const auto& player : players) {

        auto prole = dynamic_cast<GameRole*>(player);

        // 创建离线消息
        auto data_logout = this->createLogoutData();

        ZinxKernel::Zinx_SendOut(*data_logout, *(prole->getGameProtocol()));
    }

   // 将本玩家游戏世界删除
   AOIWorld::getInstance()->removeAOIPlayer(this);

    // 将姓名删除
    AOIWorld::getInstance()->delRandomName(m_name);

    // 判断是否没有玩家在线, 如果是则启动定时器
   if(ZinxKernel::Zinx_GetAllRole().size() <= 0) {

        // 1.添加唯一退出任务到时间管理类中
        TimerManage::getInstance()->addTask(ExitTask::getInstance());

        // // 2. 启动定时器
         ZinxKernel::Zinx_Add_Channel(*zinx_timer);
   }
}

// 设置和获得协议对象
void GameRole::setGameProtocol(GameProtocol* protocol) {

    m_gameProtocol = protocol;
}
GameProtocol* GameRole::getGameProtocol() {

    return m_gameProtocol;
}

// 获得pid
int GameRole::getPid() {
    return m_pid;
}
// 获取客户端X坐标
float GameRole::getCliX() {
    
    return m_X;
}

// 获取客户端Y坐标
float GameRole::getCliY() {
    return m_Y;
}

// 获取客户端Z坐标
float GameRole::getCliZ() {

    return m_Z;
}

//获取客户端V(视野)
float GameRole::getCliV() {
    return m_V;
}

// 得到具体消息类型的处理函数
Handler GameRole::getHandler(Game::MsgType type) {

    auto pit = m_handlerLists.find(type);
    if(pit == m_handlerLists.end()) {

        std::cout << "没有找到相关处理函数" << "type:" << type  << std::endl;
        return [](GameData*){};
    }

    return m_handlerLists[type];
}


// 创建登录消息
GameData* GameRole::createLoginData() {

    // 登录消息就是发送 ID 和 姓名
    auto msg = new Game::SyncPid();
    msg->set_username(m_name);
    msg->set_pid(m_pid);

    GameData* rData = new GameData(Game::MsgType::TYPE_LOGIN_ID_NAME, msg);


    return rData;
}

// 创建下线消息
GameData* GameRole::createLogoutData() {

    // 下线消息就是发送 ID 和 姓名
    auto msg = new Game::SyncPid();
    msg->set_username(m_name);
    msg->set_pid(m_pid);

    GameData* rData = new GameData(Game::MsgType::TYPE_LOGOUT_ID_NAME, msg);
    
    return rData;
}

// 创建周围玩家的信息
GameData* GameRole::createAroundPos() {

    auto pPlayers = new Game::SyncPlayers();

    // 获取周围玩家
    std::list<Player*> players =  AOIWorld::getInstance()->getAroundPlayer(this);


    // 将周围玩家添加到返回消息中
    for(const auto& player : players) {

        // 将获取到的周围玩家动态转换
        auto tmpRole = dynamic_cast<GameRole*>(player);

        // if(tmpRole == this) {
        //     // 除了自己
        //     continue;
        // }

        // 每个消息对象
        auto tmpPlayer = pPlayers->add_ps();


        tmpPlayer->set_pid(tmpRole->getPid());
        tmpPlayer->set_username(tmpRole->getName());
        tmpPlayer->mutable_p()->set_x(tmpRole->getCliX());
        tmpPlayer->mutable_p()->set_y(tmpRole->getCliY());
        tmpPlayer->mutable_p()->set_z(tmpRole->getCliZ());
        tmpPlayer->mutable_p()->set_v(tmpRole->getCliV());
        // 多余值
        tmpPlayer->mutable_p()->set_bloodvalue(m_blood);

        
    }

    auto pMsg = new GameData(Game::TYPE_AROUND_POSITION, pPlayers);
    return pMsg;
}

// 创建自己的位置信息
GameData* GameRole::createSelfPos() {


    auto pself = new Game::BroadCast();

    pself->set_pid(this->m_pid);
    pself->set_username(this->m_name);
    // 广播的是位置信息
    pself->set_tp(2);
    pself->mutable_p()->set_x(this->m_X);
    pself->mutable_p()->set_y(this->m_Y);
    pself->mutable_p()->set_z(this->m_Z);
    pself->mutable_p()->set_v(this->m_V);
    pself->mutable_p()->set_bloodvalue(m_blood);

    GameData* rData = new GameData(Game::MsgType::TYPE_BROADCAST, pself);
}


// 创建聊天消息
GameData* GameRole::createSendChatMsg(std::string chat_msg) {

    auto msg = new Game::BroadCast();

    msg->set_pid(m_pid);
    msg->set_tp(1);
    msg->set_content(chat_msg);
    msg->set_username(m_name);


    auto rData = new GameData(Game::TYPE_BROADCAST, msg);

    return rData;
}

// 视野出现函数
void GameRole::viewAppear(GameRole* role) {

    // 本玩家和对方玩家互相发送位置消息
    GameData* data;

    data = role->createSelfPos();
    ZinxKernel::Zinx_SendOut(*data, *m_gameProtocol);

    data = this->createSelfPos();
    ZinxKernel::Zinx_SendOut(*data, *(role->getGameProtocol()));

}

// 视野消失函数
void GameRole::viewDisappear(GameRole* role) {

    // 本玩家和对方玩家互相发送退出登录消息
    GameData* data;

    data = role->createLogoutData();
    ZinxKernel::Zinx_SendOut(*data, *m_gameProtocol);

    data = this->createLogoutData();
    ZinxKernel::Zinx_SendOut(*data, *(role->getGameProtocol()));
}


// 专门处理发送聊天消息的函数
void GameRole::sendChatContent(GameData*data) {

    
    // 1. 获取当前的聊天消息
    Game::Talk* chatMsg = dynamic_cast<Game::Talk*>(data->getMessage());

    if(chatMsg == nullptr) {

        return;
    }

    std::string chat_msg = chatMsg->content();

    
    
    // 2.向周围玩家发送聊天消息

    // 获取周围玩家
    auto players = AOIWorld::getInstance()->getAroundPlayer(this);

    for(const auto& player : players) {

        const auto& prole = dynamic_cast<GameRole*>(player);

        // 创建聊天消息
        auto pmsg =  this->createSendChatMsg(chat_msg);

        // 发送
        ZinxKernel::Zinx_SendOut(*pmsg, *(prole->getGameProtocol()));

    }

}

// 专门处理移动位置的函数
void GameRole::movePosition(GameData* data) {

    std::cout << "移动位置消息函数" << std::endl;
    
    // 获得新位置
    auto pPos = dynamic_cast<Game::Position*>(data->getMessage());

    float newX = pPos->x();
    float newY = pPos->y();
    float newZ = pPos->z();
    float newV = pPos->v();
    float newBlood = pPos->bloodvalue();

    // 旧玩家set
    std::set<Player*> oldSet;

    // 新玩家set
    std::set<Player*> newSet;

    // 旧玩家差值
    std::set<Player*> oldDiffSet;

    // 新玩家差值
    std::set<Player*> newDiffSet;  

    // 将旧位置的本玩家删除
    AOIWorld::getInstance()->removeAOIPlayer(this);

    // 旧位置玩家
    auto oldPlayers = AOIWorld::getInstance()->getAroundPlayer(this);
    
    for(const auto& player : oldPlayers) {

        oldSet.insert(player);
    }

    // 新位置玩家
    m_x = newX;
    m_y = newY;
    m_Z = newZ;
    m_V = newV;
    m_blood = newBlood;

    // 将新位置的本玩家添加到游戏世界中
    AOIWorld::getInstance()->addAOIPlayer(this);

    auto newPlayers = AOIWorld::getInstance()->getAroundPlayer(this);

    for(const auto& player : newPlayers) {

        newSet.emplace(player);
    }

    // 新位置玩家 - 旧位置玩家 == 新玩家与本玩家视野出现
    std::set_difference(newSet.begin(), newSet.end(), 
            oldSet.begin(), oldSet.end(), std::inserter(newDiffSet, newDiffSet.end()));


    // 旧位置玩家 - 新位置玩家 == 旧玩家与本玩家视野消失
    std::set_difference(oldSet.begin(), oldSet.end(), 
            newSet.begin(), newSet.end(), std::inserter(oldDiffSet, oldDiffSet.end()));

    // 视野出现函数
    for(const auto& player : newDiffSet) {
    
        auto role = dynamic_cast<GameRole*>(player);
        this->viewAppear(role);

    }

    // 视野消失函数
    for(const auto& player : oldDiffSet) {
    
        auto role = dynamic_cast<GameRole*>(player);
        this->viewDisappear(role);

    }

    // 向所有新位置的玩家发送本玩家新位置信息
    for(const auto& player : newPlayers) {

        auto role = dynamic_cast<GameRole*>(player);
        auto msg = this->createSelfPos();
        ZinxKernel::Zinx_SendOut(*msg, *(role->getGameProtocol()));

    }

}


/* 
    从Player继承
*/
// 获得X坐标
int GameRole::getX() {
    return static_cast<int>(m_X);
}

// 获得Y坐标
int GameRole::getY() {
    return static_cast<int>(m_Z);
}

// 获得名字
std::string GameRole::getName() {

    return m_name;
}



//-----------// 时间任务退出类 隔20s没有玩家连接就退出整个服务器--------------------

ExitTask* ExitTask::getInstance() {

    static ExitTask task;
    return &task;
}

ExitTask::~ExitTask() {
    
}

// 执行任务
void ExitTask::processTask() {

    // 调用框架退出类
    ZinxKernel::Zinx_Exit();
}
// 得到每个任务的超时次数
int ExitTask::getTimeoutSec() {

    // 隔20s
    return 20;
}

