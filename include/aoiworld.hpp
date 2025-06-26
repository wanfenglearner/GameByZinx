#ifndef __AOIWORLD_H__
#define __AOIWORLD_H__

#include <list>
#include <vector>
#include <memory>
#include <string>
#include <random>
#include <unordered_set>


// 游戏玩家类(纯虚函数)
class Player {

public:
    Player() = default;
    Player(int x, int y, std::string name);
    virtual ~Player() = default;

    // 获得X坐标
    virtual int getX() = 0;

    // 获得Y坐标
    virtual int getY() = 0;

    // 获得名字
    virtual std::string getName() = 0;

public:
    int m_x;
    int m_y;
    std::string m_name;
} ;

// 网格类
class Grid {

public:
    // 添加玩家
    void addPlayer(Player* player);
    
    // 删除玩家
    void removePlayer(Player* player);

    // 获得所有玩家
    std::list<Player*> getAllPlayers();

private:
    // 存放该网格的所有玩家
    std::list<Player*> m_playerList;
};

// 游戏世界矩阵类
class AOIWorld {

public:

    static AOIWorld* getInstance();
    
    AOIWorld(const AOIWorld&) = delete;
    AOIWorld& operator=(const AOIWorld&) = delete;


    ~AOIWorld();

    // 向AOI矩阵中添加玩家
    void addAOIPlayer(Player* player);

    // 在AOI矩阵中删除玩家
    void removeAOIPlayer(Player* player);

    // 获取周围玩家
    std::list<Player*> getAroundPlayer(Player* player);


    // 得到随机姓名
    std::string getRandomName();

    // 将得到的名字删除
    void delRandomName(const std::string name);

private:

    // 加载玩家的姓名
    void loadNameConfig();

private:
    AOIWorld();

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

    // 存放玩家姓的数组
    std::vector<std::string> m_firstNameLists;

    // 存放玩家名的数组
    std::vector<std::string> m_lastNameLists;

    // 存储已经产生的随机名字
    std::unordered_set<std::string> m_nameSet;

};

#endif