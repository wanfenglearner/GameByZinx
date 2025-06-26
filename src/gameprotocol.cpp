#include "gameprotocol.hpp"
#include "gamedata.hpp"
#include "gamechannel.hpp"


static bool Debug = true;

static void printMsg(std::string msg) {

    std::cout << msg << std::endl;
}


GameProtocol::GameProtocol() {

}
GameProtocol::~GameProtocol() {
    
    // 将绑定的角色层从框架中删除并且析构
    if(m_gameRole != nullptr) {

        // 在框架中删除
        ZinxKernel::Zinx_Del_Role(*m_gameRole);
        
        delete m_gameRole;
    }
}

// 将原始数据转换成可以在内部互通的数据 消息格式: 消息大小(4字节) + 消息id(4字节) + 具体消息
UserData* GameProtocol::raw2request(std::string _szInput)
{
    // 因为TCP是流式传输协议, 那么每次的数据大小不一定是完整的

    // 可能包含多条请求消息
    MultiGameData* multiMsg = new MultiGameData();

    // 先记录传入的数据
    m_totalMsg.append(_szInput);

    // 因为 消息大小(4字节) + 消息id(4字节) == 8字节 判断当前字符串长度是否大于8

    while(m_totalMsg.size() > 8) {
            
        int msg_len = 0;
        m_totalMsg.copy((char*)&msg_len, sizeof(int32_t), 0);
       
        // 消息id
        Game::MsgType msg_type;
        m_totalMsg.copy((char*)&msg_type, sizeof(int32_t), 4);

        // 当前记录总消息大小是否满足(>= 拿出的消息的大小)
        if((m_totalMsg.size() < (8 + msg_len)) || msg_len == 0) {

            break;
        }

        // 具体的消息
        std::string msgStr = m_totalMsg.substr(8, msg_len);
        GameData* msg  = new GameData(msg_type, msgStr);

        // 将一条请求数据添加到多条消息类中
        multiMsg->add(msg);

        // 删除拿出的数据
        m_totalMsg.erase(0, 8 + msg_len);
        
    }


    return multiMsg;
}

// 将内部互通的数据换换成原始数据 消息格式: 消息大小(4字节) + 消息id(4字节) + 具体消息
std::string* GameProtocol::response2raw(UserData& _oUserData)
{
    
    // 动态类型转换
    GET_REF2DATA(GameData, outMsg, _oUserData);
    // 得到序列化的数据
    std::string msgStr = outMsg.serializeString();

    // 得到消息的大小
    int msg_len = msgStr.size();

    // 得到消息id
    Game::MsgType msg_type = outMsg.getMsgType();
    
    //std::cout << "response2raw:>> " << "msg:" << msgStr << " msg_len:" << msg_len << "msg_type:" << msg_type << std::endl; 
    // 进行字符串拼接
    std::string* result_str = new std::string();

    result_str->insert(0, (char*)&msg_len, 4);
    result_str->insert(4, (char*)&msg_type, 4);
    result_str->append(msgStr);

    return result_str;

}

// 设置消息的处理的role层
Irole* GameProtocol::GetMsgProcessor(UserDataMsg& _oUserDataMsg)
{

    return m_gameRole;
}

Ichannel* GameProtocol::GetMsgSender(BytesMsg& _oBytes)
{

    return m_gameChannel;
}

// 设置和获得本协议层绑定的通道层
void GameProtocol::setGameChannel(GameChannel* channel) {

    m_gameChannel = channel;
}
GameChannel* GameProtocol::getGameChannel() {

    return m_gameChannel;
}

// 设置和获得绑定的角色层
void GameProtocol::setGameRole(GameRole* role) {

    m_gameRole = role;

}
GameRole* GameProtocol::getGameRole() {   
    return m_gameRole;
}