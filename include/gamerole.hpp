#ifndef __GAMEROLE_H__
#define __GAMEROLE_H__

#include <zinx.h>
#include "gamedata.hpp"
#include "aoiworld.hpp"

/*
	游戏角色层  对游戏的逻辑(业务)进行处理
*/

// 包含协议层
class GameProtocol;

class GameRole : public Irole, public Player
{

public:

	GameRole();
	~GameRole() = default;

	bool Init() ;

	// 处理消息的函数
	UserData* ProcMsg(UserData& _poUserData) ;

	// 处理下线消息
	void Fini() ;


	// 设置和获得协议对象
	void setGameProtocol(GameProtocol* protocol);
	GameProtocol* getGameProtocol();

	// 获得pid
	int getPid();

	// 获取客户端X坐标
	float getCliX();

	// 获取客户端Y坐标
	float getCliY();

	// 获取客户端Z坐标
	float getCliZ();

	//获取客户端V(视野)
	float getCliV();

private:
	// 创建登录消息
	GameData* createLoginData();
	// 创建下线消息
	GameData* createLogoutData();

	// 创建周围玩家的信息
	GameData* createAroundPos();

	// 创建自己的位置信息
	GameData* createSelfPos();


public:	// 从Player继承
	// 获得X坐标
    int getX();

    // 获得Y坐标
    int getY();

    // 获得名字
    std::string getName();
	
private:
	GameProtocol* m_gameProtocol;

	// 从客户端传递而来的 X  Y  Z  V(视角 单位 0-360)
	float m_X;
	float m_Y; // 这个在游戏时间表示Z坐标(高度)
	float m_Z; // 这个在游戏时间表示Y坐标
	float m_V;

	int m_pid;
	std::string m_name;
};



#endif 