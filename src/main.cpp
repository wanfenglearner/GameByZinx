#include "gamechannel.hpp"
#include "gamedata.hpp"
#include <memory>

int main(int argc, char*argv[]) {


    if(argc < 2) {
        std::cout << "./*.exe port" << std::endl;
        return -1;
    }

   int port = std::stoi(argv[1]);
   
    // 初始化框架
    ZinxKernel::ZinxKernelInit();

    // 创建监听通道, 并添加到框架中
    ZinxKernel::Zinx_Add_Channel(*(new ZinxTCPListen(port, new GameConnFactory() )));

    // 启动框架
    ZinxKernel::Zinx_Run();

    // 结束框架
    ZinxKernel::ZinxKernelFini();





    return 0;
}