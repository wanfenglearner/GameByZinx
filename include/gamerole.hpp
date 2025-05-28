#ifndef __GAMEROLE_H__
#define __GAMEROLE_H__

#include <zinx.h>


/*
	游戏角色层  对游戏的逻辑(业务)进行处理
*/


class GameRole : public Irole {

public:

	GameRole() = default;
	~GameRole() = default;

	bool Init() ;

	UserData* ProcMsg(UserData& _poUserData) ;

	void Fini() ;

};



#endif 