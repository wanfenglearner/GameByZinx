#include "gamedata.hpp"


// 消息存储类

GameData::GameData() :
    m_message(nullptr)
{

}
GameData::~GameData() {

    if(m_message != nullptr) {
        delete m_message;
    }
}

// 传递 消息类型和具体信息的构造函数
GameData::GameData(Game::MsgType type, google::protobuf::Message* message) {

    this->setMsgType(type);
    this->setMessage(message);

}

// 传递 消息类型和字节流构造函数
GameData::GameData(Game::MsgType type, const std::string msg) {

    this->setMsgType(type);

    switch (type)
    {
    case Game::MsgType::TYPE_LOGIN_ID_NAME:
        m_message = new Game::SyncPid();
        break;
        
    case Game::MsgType::TYPE_CHAT_CONTENT:
        m_message = new Game::Talk();
        break;

    case Game::MsgType::TYPE_NEW_POSITION:
        m_message = new Game::Position();
        break;

    case Game::MsgType::TYPE_BROADCAST:
        m_message = new Game::BroadCast();
        break;

    case Game::MsgType::TYPE_LOGOUT_ID_NAME:
        m_message = new Game::SyncPid();
        break;        

    case Game::MsgType::TYPE_AROUND_POSITION:
        m_message = new Game::SyncPlayers();
        break;

    default:
        break;
    }

    // 解析相应的字符串
    m_message->ParseFromString(msg);
    
}

// 设置和获取消息类型
void GameData::setMsgType(Game::MsgType type) {

    m_msgType = type;
}
Game::MsgType GameData::getMsgType() {
    return m_msgType;
}

// 设置和获取消息
void GameData::setMessage(google::protobuf::Message* message) {

    m_message = message;
}
google::protobuf::Message* GameData::getMessage() {

    return m_message;
}

// 将消息类型进行序列化
std::string GameData::serializeString() {

    std::string rStr;
    
    m_message->SerializeToString(&rStr);

    return rStr;

}


//------------- 存储多条GameData消息的类---------------

MultiGameData::MultiGameData() {

}
MultiGameData::~MultiGameData() {
    
    for(const auto& p : m_GameDataLists) {
        if(p != nullptr) {

            delete p;
        }
    }
}

// 添加消息
void MultiGameData::add(GameData* data) {

    m_GameDataLists.push_back(data);

}

// 获得存储的多条消息
std::list<GameData*>& MultiGameData::getGameDataLists() {

    return m_GameDataLists;

}