#include "gamechannel.hpp"
#include "gamedata.hpp"
#include <memory>
#include "aoiworld.hpp"

class PlayerX : public Player {

public:

    PlayerX(int x, int y, std::string name):
        Player(x, y, name)
    {

    }
        // 获得X坐标
    int getX()
    {
        return m_x;
    }

    // 获得Y坐标
    int getY() {
        return m_y;
    }

    // 获得名字
    std::string getName() {
        return m_name;
    }
};




void test() {

    // AOIWorld aoi(20, 200, 50, 230, 6, 6);

    // auto p1 = std::make_shared<PlayerX>(60, 107, "玩家1");
    // auto p2 = std::make_shared<PlayerX>(91, 118, "玩家2");
    // auto p3 = std::make_shared<PlayerX>(147, 133, "玩家3");

    // aoi.addAOIPlayer(p1.get());
    // aoi.addAOIPlayer(p2.get());
    // aoi.addAOIPlayer(p3.get());

    // for(const auto& i : aoi.getAroundPlayer(p1.get())) {
    //     std::cout << i->getName() << " x:" << i->getX() << " y:" << i->getY() << std::endl; 
    // }

    // // for(const auto& i : aoi.getAroundPlayer(p2.get())) {
    // //     std::cout << i->getName() << " x:" << i->getX() << " y:" << i->getY() << std::endl; 
    // // }

    // for(const auto& i : aoi.getAroundPlayer(p3.get())) {
    //     std::cout << i->getName() << " x:" << i->getX() << " y:" << i->getY() << std::endl; 
    // } 

}

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