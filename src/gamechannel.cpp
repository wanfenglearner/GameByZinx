#include "gamechannel.hpp"

#include <string>

static bool Debug = true;

static void printMsg(std::string msg) {

    std::cout << msg << std::endl;
}

//---------------- 产生TCP连接的类 ------------------
GameChannel::GameChannel(int fd):
    ZinxTcpData(fd),
    m_gameProtocol(nullptr),
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
    
    // 将绑定的协议层从框架中删除并且析构
    if(m_gameProtocol != nullptr) {
        // 摘除框架
        ZinxKernel::Zinx_Del_Proto(*m_gameProtocol);

        // 调用析构函数
        delete m_gameProtocol;
    }
}

AZinxHandler* GameChannel::GetInputNextStage(BytesMsg& _oInput)
{
    return m_gameProtocol;
}

// 设置和获得协议对象
void GameChannel::setGameProtocol(GameProtocol* protocol) {
    m_gameProtocol = protocol;
}

GameProtocol* GameChannel::getGameProtocol() {
    return m_gameProtocol;
}

// 得到连接的fd
int GameChannel::getFd() {

    return m_fd;

}

//---------------- TCP链接工厂 ------------------
ZinxTcpData* GameConnFactory::CreateTcpDataChannel(int _fd)
{

    // 创建TCP连接对象
    auto pchannel = new GameChannel(_fd);

    // 创建协议对象
    auto pprotocol = new GameProtocol();

    // 创建角色层
    auto prole = new GameRole();


    // 将协议层绑定到通道层(TCP连接对象)
    pchannel->setGameProtocol(pprotocol);
    // 将通道层绑定到协议层
    pprotocol->setGameChannel(pchannel);

    // 将协议层绑定到角色层
    prole->setGameProtocol(pprotocol);

    // 将角色层绑定到协议层
    pprotocol->setGameRole(prole);

    // 将协议对象添加到框架中
    ZinxKernel::Zinx_Add_Proto(*pprotocol);

    // 将角色层添加到框架中
    ZinxKernel::Zinx_Add_Role(*prole);

    return pchannel;
}