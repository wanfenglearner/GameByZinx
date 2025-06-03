#ifndef __GAMEPROTOCOL_H__
#define __GAMEPROTOCOL_H__

#include <zinx.h>


/*
*	游戏协议类 对请求和响应的数据进行处理
*/

class GameProtocol : public Iprotocol{

public:
	GameProtocol() = default;
	~GameProtocol() = default;


	// 将原始数据转换成可以在内部互通的数据 消息格式: 消息大小(4字节) + 消息id(4字节) + 具体消息
	UserData* raw2request(std::string _szInput) ;

	// 将内部互通的数据换换成原始数据 消息格式: 消息大小(4字节) + 消息id(4字节) + 具体消息
	std::string* response2raw(UserData& _oUserData) ;

	// 设置消息的处理的role层
	Irole* GetMsgProcessor(UserDataMsg& _oUserDataMsg) ;

	// 设置结果消息传递给通道层
	Ichannel* GetMsgSender(BytesMsg& _oBytes) ;

	// 设置和获得本协议层绑定的通道层
	void setGameChannel(Ichannel* channel);
	Ichannel* getGameChannel();

	// 设置和获得绑定的角色层
	void setGameRole(Irole* role);
	Irole* getGameRole();

private:

	// 记录每个TCP连接信息(标识)
	std::string m_tcpInfo;	

	// 记录总的接受的原始数据
	std::string m_totalMsg;

	// 记录本协议层绑定的通道层
	Ichannel* m_gameChannel;

	// 记录本协议层绑定的角色层
	Irole* m_gameRole;


};


#endif 