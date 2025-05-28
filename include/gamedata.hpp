#ifndef __GAMEDATA_H__
#define __GAMEDATA_H__

#include <zinx.h>


/*
    自定义用户消息, 方便在服务器和客户端进行传递消息
*/

class GameData : public UserData {
	
public:
	GameData() = default;
	~GameData() = default;


};


#endif 