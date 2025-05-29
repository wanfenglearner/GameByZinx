#include "gameprotocol.hpp"
#include "gamedata.hpp"


static bool Debug = true;

static void printMsg(std::string msg) {

    std::cout << msg << std::endl;
}


// 将原始数据转换成可以在内部互通的数据 消息格式: 消息大小(4字节) + 消息id(4字节) + 具体消息
UserData* GameProtocol::raw2request(std::string _szInput)
{
    // 因为TCP是流式传输协议, 那么每次的数据大小不一定是完整的

    GameData* rMsg = nullptr;

    // 先记录传入的数据
    m_totalMsg.append(_szInput);

    // 因为 消息大小(4字节) + 消息id(4字节) == 8字节 判断当前字符串长度是否大于8

    while(m_totalMsg.size() > 8) {
            
        int msg_len = 0;
        _szInput.copy((char*)&msg_len, sizeof(int32_t), 0);

        // 消息id
        Game::MsgType msg_type;
        _szInput.copy((char*)&msg_type, sizeof(int32_t), 4);

        // 当前记录总消息大小是否满足(>= 拿出的消息的大小)
        if((m_totalMsg.size() < (8 + msg_len)) || msg_len == 0) {

            break;
        }

        // 具体的消息
        std::string msgStr = _szInput.substr(8, msg_len);
        rMsg = new GameData(msg_type, msgStr);

        // 删除拿出的数据
        m_totalMsg.erase(0, 8 + msg_len);
        
        // 拿到消息 退出循环
        break;

    }

    return rMsg;
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
    
    // 进行字符串拼接
    std::string result_str;

    result_str.insert(0, (char*)&msg_len, 4);
    result_str.insert(4, (char*)&msg_type, 4);
    result_str.append(msgStr);

    return new std::string(result_str);

}

Irole* GameProtocol::GetMsgProcessor(UserDataMsg& _oUserDataMsg)
{

    return nullptr;
}

Ichannel* GameProtocol::GetMsgSender(BytesMsg& _oBytes)
{

}
