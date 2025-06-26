#include "aoiworld.hpp"
#include <iostream>
#include <fstream>
#include <random>

static int startX = 0;
static int endX = 400;
static int startY = 0;
static int endY = 400;
static int xCount = 20;
static int yCount = 20;


static std::default_random_engine random_engine((unsigned int)time(nullptr));

// -----------------游戏世界矩阵类-----------------

// 单例对象
AOIWorld* AOIWorld::getInstance() {

    static AOIWorld world;
    
    return &world;
}


AOIWorld::AOIWorld() { 
    
    m_startX = startX;
    m_endX = endX;
    m_startY = startY;
    m_endY = endY;
    m_xCount = xCount;
    m_yCount = yCount;

    m_gridWidth = (m_endX - m_startX) / m_xCount;
    m_gridHeight = (m_endY - m_startY) / m_yCount;

    m_totalGrid = m_xCount * m_yCount;

    m_gridList.resize(m_totalGrid);

    // 周围9个位置(包括本网格)
    dGrid = {-m_xCount, -m_xCount + 1, 1, m_xCount + 1, m_xCount, m_xCount -1, -1, -m_xCount - 1, 0};

    // 加载配置文件
    this->loadNameConfig();
}

AOIWorld::~AOIWorld() {

}

// 向AOI矩阵中添加玩家
void AOIWorld::addAOIPlayer(Player* player) {

    // 获得该玩家所在的网格id  
    int grid_id = calPlayerGrid(player->getX(), player->getY());
    //std::cout << "addAOIPlayer:" << "x:" << player->getX() << " y:" << player->getY() << std::endl;
    // 向该网格中添加元素
    m_gridList[grid_id].addPlayer(player);
   // std::cout << "addAOIPlayer大小: " << m_gridList[grid_id].getAllPlayers().size() << std::endl;
}


// 在AOI矩阵中删除玩家
void AOIWorld::removeAOIPlayer(Player* player) {

    // 获得该玩家所在的网格id  
    int grid_id = calPlayerGrid(player->getX(), player->getY());
    // 删除该网格中的玩家
    m_gridList[grid_id].removePlayer(player);

}

// 获取周围玩家
std::list<Player*> AOIWorld::getAroundPlayer(Player* player) {

    // 获得该玩家所在的网格id 
    int grid_id = calPlayerGrid(player->getX(), player->getY());

    std::list<Player*> result_players;

    // 周围9个相邻网格玩家的信息(包括自己所在网格)
    for(int i = 0; i < 9; ++i) {
        int pgrid = grid_id + dGrid[i];
        
        // 超过范围
        if(pgrid < 0 || pgrid >= m_totalGrid) {
            continue;
        }

        // 获得网格内的所有玩家 
        auto tmp_players = m_gridList[pgrid].getAllPlayers();
        result_players.splice(result_players.end(), tmp_players);

    }
    

    return result_players;
}

// 计算玩家所在的网格
int AOIWorld::calPlayerGrid(int x, int y) {

    return (x - m_startX ) / m_gridWidth + (y - m_startY) / m_gridHeight * m_xCount;
}

// 得到随机姓名
std::string AOIWorld::getRandomName() {

    std::string str;
    while(true) {

        int i = random_engine() % m_firstNameLists.size();
        int j = random_engine() % m_lastNameLists.size();

        str = m_firstNameLists[i] + m_lastNameLists[j];

        auto pit = m_nameSet.find(str);
        if(pit == m_nameSet.end()) {

            break;
        }
    }

    //  记录已经产生的名字
    m_nameSet.insert(str);

    return str;
}

// 将得到的名字删除
void AOIWorld::delRandomName(const std::string name) {

    m_nameSet.erase(name);
}


// 加载玩家的姓名
void AOIWorld::loadNameConfig() {

    // 分别读取 firstname.txt  和 lastname.txt 文件, 将内容存储到数组中

    std::ifstream ifs("firstname.txt");
    if(ifs.is_open()) {

        std::string str;
        while(getline(ifs, str)) {

            m_firstNameLists.push_back(str);
        }

        ifs.close();
    }

    ifs = std::ifstream("lastname.txt");

    if(ifs.is_open()) {

        std::string str;
        while(getline(ifs, str)) {
            
            m_lastNameLists.push_back(str);
        }

        ifs.close();
    }

}

// ----------------网格类----------------------

// 添加玩家
void Grid::addPlayer(Player* player) {

    m_playerList.push_back(player);
}

// 删除玩家
void Grid::removePlayer(Player* player) {

    std::cout << "删除玩家成功" << "name:" << player->getName() << std::endl; 
    m_playerList.remove(player);
    
}

// 获得所有玩家
std::list<Player*> Grid::getAllPlayers() {
    return m_playerList;
    
}

// ----------------抽象游戏玩家类--------------
Player::Player(int x, int y, std::string name)
{
    m_x = x;
    m_y = y;
    m_name = name;
}

