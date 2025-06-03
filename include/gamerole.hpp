#ifndef __GAMEROLE_H__
#define __GAMEROLE_H__

#include <zinx.h>
#include "gamedata.hpp"


/*
	游戏角色层  对游戏的逻辑(业务)进行处理
*/


class GameRole : public Irole {

public:

	GameRole() = default;
	~GameRole() = default;

	bool Init() ;

	// 处理消息的函数
	UserData* ProcMsg(UserData& _poUserData) ;

	void Fini() ;


	// 设置和获得协议对象
	void setGameProtocol(Iprotocol* protocol);
	Iprotocol* getGameProtocol();
	
private:
	Iprotocol* m_gameProtocol;

};



#endif 