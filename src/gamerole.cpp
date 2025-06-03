#include "gamerole.hpp"

bool GameRole::Init()
{
    return false;
}

UserData* GameRole::ProcMsg(UserData& _poUserData)
{
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