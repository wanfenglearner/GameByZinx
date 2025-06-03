#include "gamerole.hpp"

bool GameRole::Init()
{
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
}

// 设置和获得协议对象
void GameRole::setGameProtocol(Iprotocol* protocol) {

    m_gameProtocol = protocol;
}
Iprotocol* GameRole::getGameProtocol() {

    return m_gameProtocol;
}