#include "gamerole.hpp"
#include "gameprotocol.hpp"
#include "gamechannel.hpp"

// 创建全局变量 AOIWorld 创建游戏时间

static AOIWorld world(0, 400, 0, 400, 20, 20);


GameRole::GameRole() {

    // 初始化位置信息
    m_X = 100;
    m_Z = 100;
    m_Y = 0;
    m_V = 0;
}
bool GameRole::Init()
{
    // 初始化变量
    m_pid = m_gameProtocol->getGameChannel()->getFd();
    m_name = "Tom";


    // 将本玩家添加到游戏世界中
    world.addAOIPlayer(this);

    // 向客户端发送ID和姓名
    GameData* data_id_name = this->createLoginData();
    ZinxKernel::Zinx_SendOut(*data_id_name, *m_gameProtocol);

    // 向客户端发送周围玩家的信息
    GameData* data_around_players = this->createAroundPos();
    ZinxKernel::Zinx_SendOut(*data_around_players, *m_gameProtocol);

    // 向周围玩家以及自己发送自己的位置信息
    auto players = world.getAroundPlayer(this);
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
        sprintf(buf, "id:%d name:%s 消息类型:%d 消息:%s", 
        m_pid, m_name.c_str(), pmsg->getMsgType(), pmsg->getMessage()->Utf8DebugString().c_str());
        
        std::cout << buf << std::endl;
    }

    return nullptr;
}

void GameRole::Fini()
{

    // 向周围玩家发送离线消息
    auto players = world.getAroundPlayer(this);

    for(const auto& player : players) {

        auto prole = dynamic_cast<GameRole*>(player);

        if(prole == this) {
            // 除了自己
            continue;
        }

        // 创建离线消息
        auto data_logout = this->createLogoutData();

        ZinxKernel::Zinx_SendOut(*data_logout, *(prole->getGameProtocol()));
    }

   // 将本玩家游戏世界删除
   world.removeAOIPlayer(this);
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
    std::list<Player*> players =  world.getAroundPlayer(this);


    // 将周围玩家添加到返回消息中
    for(const auto& player : players) {

        // 将获取到的周围玩家动态转换
        auto tmpRole = dynamic_cast<GameRole*>(player);

        if(tmpRole == this) {
            // 除了自己
            continue;
        }

        // 每个消息对象
        auto tmpPlayer = pPlayers->add_ps();


        tmpPlayer->set_pid(tmpRole->getPid());
        tmpPlayer->set_username(tmpRole->getName());
        tmpPlayer->mutable_p()->set_x(tmpRole->getCliX());
        tmpPlayer->mutable_p()->set_y(tmpRole->getCliY());
        tmpPlayer->mutable_p()->set_z(tmpRole->getCliZ());
        tmpPlayer->mutable_p()->set_v(tmpRole->getCliV());
        // 多余值
        tmpPlayer->mutable_p()->set_bloodvalue(0);
        
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
    pself->mutable_p()->set_bloodvalue(0);

    GameData* rData = new GameData(Game::MsgType::TYPE_BROADCAST, pself);
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