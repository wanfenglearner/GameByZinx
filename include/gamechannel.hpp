#ifndef __GAMECHANNEL_H__
#define __GAMECHANNEL_H__

#include <ZinxTCP.h>


/*
*	游戏通道类  处理网络IO
*/

class GameChannel : public ZinxTcpData {

public:
	GameChannel() = default;
	~GameChannel() = default;

	AZinxHandler* GetInputNextStage(BytesMsg& _oInput);

};


#endif 