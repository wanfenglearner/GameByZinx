#include "gamedata.hpp"


// 消息存储类

GameData::GameData() :
    m_message(nullptr)
{

}
GameData::~GameData() {


}

// 传递 消息类型和具体信息的构造函数
GameData::GameData(Game::MsgType type, google::protobuf::Message* message) {

    this->setMsgType(type);
    this->setMessage(message);

}

// 传递 消息类型和字节流构造函数
GameData::GameData(Game::MsgType type, const std::string msg) {

    this->setMsgType(type);
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