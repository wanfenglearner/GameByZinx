#include "gamechannel.hpp"
#include "gamedata.hpp"
#include <memory>
#include "aoiworld.hpp"

int init(int argc, char*argv[]) {
   
    int port = 0;

#if 1   // 调用命令传参数
    if (argc < 2) {
        std::cout << "./*.exe port" << std::endl;
        return -1;
    }

     port = std::stoi(argv[1]);

#else
    port = 8899;
#endif 

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
int main(int argc, char*argv[]) {

    init(argc, argv);



    return 0;
}