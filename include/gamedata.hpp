#ifndef __GAMEDATA_H__
#define __GAMEDATA_H__

#include <zinx.h>
#include "gamemsg.pb.h"
#include <google/protobuf/message.h>
#include <string>
#include <list>


/*
    自定义用户消息, 方便在服务器和客户端进行传递消息
*/

// 消息存储类
class GameData : public UserData {
	
public:

	GameData();
	~GameData();

	// 传递 消息类型和具体信息指针的构造函数
	GameData(Game::MsgType type, google::protobuf::Message* message);

	// 传递 消息类型和字节流构造函数
	GameData(Game::MsgType type, const std::string msg);

	// 设置和获取消息类型
	void setMsgType(Game::MsgType type);
	Game::MsgType getMsgType();

	// 设置和获取消息
	void setMessage(google::protobuf::Message* message);
	google::protobuf::Message* getMessage();

	// 将消息类型进行序列化
	std::string serializeString();
	
private:
	
	// 存储消息ID
	Game::MsgType m_msgType;

	google::protobuf::Message* m_message;
	


};


// 存储多条GameData消息的类
class MultiGameData : public UserData {

public:

	MultiGameData();
	~MultiGameData();
	
	// 添加消息
	void add(GameData* data);

	// 获得存储的多条消息
	std::list<GameData*>& getGameDataLists();
private:
	// 存储多条消息
	std::list<GameData*> m_GameDataLists;
	
};

#endif 