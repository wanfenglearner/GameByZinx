#include "gamechannel.hpp"
#include "gamedata.hpp"
#include <memory>
#include "aoiworld.hpp"

void test() {

    AOIWorld aoi(20, 200, 50, 230, 6, 6);

    auto p1 = std::make_shared<Player>(60, 107, "玩家1");
    auto p2 = std::make_shared<Player>(91, 118, "玩家2");
    auto p3 = std::make_shared<Player>(147, 133, "玩家3");

    aoi.addAOIPlayer(p1);
    aoi.addAOIPlayer(p2);
    aoi.addAOIPlayer(p3);

    for(const auto& i : aoi.getAroundPlayer(p1)) {
        std::cout << i->getName() << " x:" << i->getX() << " y:" << i->getY() << std::endl; 
    }

    for(const auto& i : aoi.getAroundPlayer(p2)) {
        std::cout << i->getName() << " x:" << i->getX() << " y:" << i->getY() << std::endl; 
    }

    for(const auto& i : aoi.getAroundPlayer(p3)) {
        std::cout << i->getName() << " x:" << i->getX() << " y:" << i->getY() << std::endl; 
    } 


}

void init() {
   
//     if(argc < 2) {
//         std::cout << "./*.exe port" << std::endl;
//         return -1;
//     }

//    int port = std::stoi(argv[1]);
   
//     // 初始化框架
//     ZinxKernel::ZinxKernelInit();

//     // 创建监听通道, 并添加到框架中
//     ZinxKernel::Zinx_Add_Channel(*(new ZinxTCPListen(port, new GameConnFactory() )));

//     // 启动框架
//     ZinxKernel::Zinx_Run();

//     // 结束框架
//     ZinxKernel::ZinxKernelFini();

}
int main(int argc, char*argv[]) {

    test();

  

    return 0;
}