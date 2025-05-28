#ifndef __GAMECHANNEL_H__
#define __GAMECHANNEL_H__

#include <ZinxTCP.h>
#include "gameprotocol.hpp"

/*
*	游戏通道类  处理网络IO
*/

// 产生TCP连接的类
class GameChannel : public ZinxTcpData {

public:
	GameChannel(int fd);
	~GameChannel();

	AZinxHandler* GetInputNextStage(BytesMsg& _oInput);

	// 设置和获得协议对象
	void setGameProtocol(GameProtocol* protocol);
	GameProtocol* getGameProtocol();


private:
	int m_fd;
	
	// 存放协议通道
	GameProtocol* m_protocol;

};

// TCP链接工厂
class GameConnFactory : public IZinxTcpConnFact {

public:
	ZinxTcpData* CreateTcpDataChannel(int _fd);

};


#endif 