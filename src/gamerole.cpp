#include "gamerole.hpp"


// 创建全局变量 AOIWorld 创建游戏时间

static AOIWorld world(0, 400, 0, 400, 20, 20);



bool GameRole::Init()
{

   // 将本玩家添加到游戏世界中
   world.addAOIPlayer(this);

   // 向客户端发送ID和姓名
   GameData* game_data = this->createLoginData();

    return true;
}

// 处理消息的函数
UserData* GameRole::ProcMsg(UserData& _poUserData)
{
   GET_REF2DATA(MultiGameData, multidata, _poUserData);

   for(const auto& pmsg : multidata.getGameDataLists()) {

        std::cout << "type:" << pmsg->getMsgType() << "消息:" << pmsg->getMessage()->Utf8DebugString() << std::endl;
   }

    return nullptr;
}

void GameRole::Fini()
{
   // 将本玩家游戏世界删除
   world.removeAOIPlayer(this);
}

// 设置和获得协议对象
void GameRole::setGameProtocol(Iprotocol* protocol) {

    m_gameProtocol = protocol;
}
Iprotocol* GameRole::getGameProtocol() {

    return m_gameProtocol;
}

// 创建登录消息
GameData* GameRole::createLoginData() {

    // 登录消息就是发送 ID 和 姓名
    auto msg = new Game::SyncPid();
    msg->set_username();
    msg->set_pid();

    GameData* rData = new GameData(Game::MsgType::TYPE_LOGIN_ID_NAME, msg);


    return rData;
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

    return std::string();
}