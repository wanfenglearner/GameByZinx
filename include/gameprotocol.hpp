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


	UserData* raw2request(std::string _szInput) ;

	std::string* response2raw(UserData& _oUserData) ;

	Irole* GetMsgProcessor(UserDataMsg& _oUserDataMsg) ;

	Ichannel* GetMsgSender(BytesMsg& _oBytes) ;

};


#endif 