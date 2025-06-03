#include "aoiworld.hpp"
#include <iostream>

// -----------------游戏世界矩阵类-----------------

AOIWorld::AOIWorld() { 
}

AOIWorld::AOIWorld(int startX, int endX, int startY, int endY, int xCount, int yCount) {

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

    dGrid = {-m_xCount, -m_xCount + 1, 1, m_xCount + 1, m_xCount, m_xCount -1, -1, -m_xCount - 1};
}

AOIWorld::~AOIWorld() {

}

// 向AOI矩阵中添加玩家
void AOIWorld::addAOIPlayer(std::shared_ptr<Player> player) {

    // 获得该玩家所在的网格id  
    int grid_id = calPlayerGrid(player->getX(), player->getY());

    
    // 向该网格中添加元素
    m_gridList[grid_id].addPlayer(player);

}

// 在AOI矩阵中删除玩家
void AOIWorld::removeAOIPlayer(std::shared_ptr<Player> player) {

    // 获得该玩家所在的网格id  
    int grid_id = calPlayerGrid(player->getX(), player->getY());
    // 删除该网格中的玩家
    m_gridList[grid_id].removePlayer(player);

}

// 获取周围玩家
std::list<std::shared_ptr<Player>> AOIWorld::getAroundPlayer(std::shared_ptr<Player> player) {


    // 获得该玩家所在的网格id 
    int grid_id = calPlayerGrid(player->getX(), player->getY());

    std::list<std::shared_ptr<Player>> result_players;

    // 周围8个相邻网格玩家的信息
    for(int i = 0; i < 8; ++i) {
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


// ----------------网格类----------------------

// 添加玩家
void Grid::addPlayer(std::shared_ptr<Player> player) {

    m_playerList.push_back(player);
}

// 删除玩家
void Grid::removePlayer(std::shared_ptr<Player> player) {

    m_playerList.remove(player);
}

// 获得所有玩家
std::list<std::shared_ptr<Player>> Grid::getAllPlayers() {
    return m_playerList;
    
}

// ----------------抽象游戏玩家类--------------
Player::Player(int x, int y, std::string name)
{
    setX(x);
    setY(y);
    setName(name);
}

// 设置和获得 X 坐标
void Player::setX(int x) {

    m_x = x;
}
int Player::getX() {

    return m_x;
}

// 设置和获得 Y 坐标
void Player::setY(int y) {

    m_y = y;
}
int Player::getY() {

    return m_y;
}

// 设置和获得名字
void Player::setName(std::string  name) {

    m_name = name;
}
std::string Player::getName() {

    return m_name;
}