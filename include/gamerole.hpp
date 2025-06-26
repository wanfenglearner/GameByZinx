#ifndef __GAMEROLE_H__
#define __GAMEROLE_H__

#include <zinx.h>
#include <unordered_map>
#include "gamedata.hpp"
#include "aoiworld.hpp"
#include <functional>
#include "zinxtimer.hpp"

/*
	游戏角色层  对游戏的逻辑(业务)进行处理
*/

// 包含协议层
class GameProtocol;

using Handler = std::function<void(GameData*)>;

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

	// 得到具体消息类型的处理函数
	Handler getHandler(Game::MsgType type);

private:
	// 创建登录消息
	GameData* createLoginData();
	// 创建下线消息
	GameData* createLogoutData();

	// 创建周围玩家的信息
	GameData* createAroundPos();

	// 创建自己的位置信息
	GameData* createSelfPos();

	// 创建聊天消息
	GameData* createSendChatMsg(std::string chat_msg);

	// 视野出现函数
	void viewAppear(GameRole* role);

	// 视野消失函数
	void viewDisappear(GameRole* role);

private:

	// 专门处理发送聊天消息的函数
	void sendChatContent(GameData* data);

	// 专门处理移动位置的函数
	void movePosition(GameData* data);


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
	float m_Y; // 这个在游戏世界表示Z坐标(高度)
	float m_Z; // 这个在游戏世界表示Y坐标
	float m_V; // 视角
	float m_blood;	// 血量 

	int m_pid;
	std::string m_name;

	// 存储处理各种消息类型的函数
	std::unordered_map<Game::MsgType, Handler> m_handlerLists;
};

// 时间任务退出类 隔20s没有玩家连接就退出整个服务器
class ExitTask : public TimerTask {

public:

	static ExitTask* getInstance();

	~ExitTask();

	// 执行任务
	void processTask();
	// 得到每个任务的超时次数
	int getTimeoutSec();

private:
	ExitTask() = default;
};


#endif 