#include "gamechannel.hpp"
#include <string>

static bool Debug = true;

static void printMsg(std::string msg) {

    std::cout << msg << std::endl;
}

//---------------- 产生TCP连接的类 ------------------
GameChannel::GameChannel(int fd):
    ZinxTcpData(fd),
    m_protocol(nullptr),
    m_fd(fd)
{
    if(Debug) {
        printMsg("TCP连接成功 fd = " + std::to_string(fd));
    }
}
GameChannel::~GameChannel() {

    if(Debug) {
        printMsg("TCP断开连接 fd = " + std::to_string(m_fd));
    }

}

AZinxHandler* GameChannel::GetInputNextStage(BytesMsg& _oInput)
{
    return this->getGameProtocol();
}

// 设置和获得协议对象
void GameChannel::setGameProtocol(Iprotocol* protocol) {
    m_protocol = protocol;
}

Iprotocol* GameChannel::getGameProtocol() {
    return m_protocol;
}


//---------------- TCP链接工厂 ------------------
ZinxTcpData* GameConnFactory::CreateTcpDataChannel(int _fd)
{

    // 创建TCP连接对象
    auto pchannel = new GameChannel(_fd);

    // 创建协议对象
    auto pprotocol = new GameProtocol();

    // 将协议层绑定到通道层(TCP连接对象)
    pchannel->setGameProtocol(pprotocol);

    // 将通道层绑定到协议层
    pprotocol->setGameChannel(pchannel);

    
    // 将协议对象添加到框架中
    ZinxKernel::Zinx_Add_Proto(*(pprotocol));

    return pchannel;
}