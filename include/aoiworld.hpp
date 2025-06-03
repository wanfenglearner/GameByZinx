#ifndef __AOIWORLD_H__
#define __AOIWORLD_H__

#include <list>
#include <vector>
#include <memory>
#include <string>


// 游戏玩家类
class Player {

public:
    Player(int x, int y, std::string name);
    ~Player() = default;

    // 设置和获得 X 坐标
    void setX(int x);
    int getX();

    // 设置和获得 Y 坐标
    void setY(int y);
    int getY();

    // 设置和获得名字
    void setName(std::string name);
    std::string getName();

public:
    int m_x;
    int m_y;
    std::string m_name;
} ;

// 网格类
class Grid {

public:
    // 添加玩家
    void addPlayer(std::shared_ptr<Player> player);
    
    // 删除玩家
    void removePlayer(std::shared_ptr<Player> player);

    // 获得所有玩家
    std::list<std::shared_ptr<Player>> getAllPlayers();


private:
    // 存放该网格的所有玩家
    std::list<std::shared_ptr<Player>> m_playerList;
};

// 游戏世界矩阵类
class AOIWorld {

public:
    AOIWorld();
    AOIWorld(int startX, int endX, int startY, int endY, int xCount, int yCount);
    ~AOIWorld();


    // 向AOI矩阵中添加玩家
    void addAOIPlayer(std::shared_ptr<Player> player);

    // 在AOI矩阵中删除玩家
    void removeAOIPlayer(std::shared_ptr<Player> player);

    // 获取周围玩家
    std::list<std::shared_ptr<Player>> getAroundPlayer(std::shared_ptr<Player> player);

private:
    // 计算玩家所在的网格
    int calPlayerGrid(int x, int y);
private:
    // 存储所有网格
    std::vector<Grid>m_gridList;


    // 存放开始X坐标
    int m_startX;

    // 存放结束X坐标
    int m_endX;

    // 存放开始Y坐标
    int m_startY;

    // 存放结束Y坐标
    int m_endY;

    // 网格的宽度
    int m_gridWidth;

    // 网格的高度
    int m_gridHeight;

    // X坐标的网格数量
    int m_xCount;

    // Y坐标的网格数量
    int m_yCount;

    // 总的网格数
    int m_totalGrid;

    std::vector<int> dGrid;
};

#endif