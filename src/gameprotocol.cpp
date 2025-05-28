#include "gameprotocol.hpp"
#include "gamedata.hpp"


static bool Debug = true;

static void printMsg(std::string msg) {

    std::cout << msg << std::endl;
}

UserData* GameProtocol::raw2request(std::string _szInput)
{

    return nullptr;
}

std::string* GameProtocol::response2raw(UserData& _oUserData)
{
    return nullptr;
}

Irole* GameProtocol::GetMsgProcessor(UserDataMsg& _oUserDataMsg)
{

    return nullptr;
}

Ichannel* GameProtocol::GetMsgSender(BytesMsg& _oBytes)
{

}
