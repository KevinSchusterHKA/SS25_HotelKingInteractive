#ifndef MAP_HPP
#define MAP_HPP

#include "tile.hpp"
#include "PropertyTile.hpp"
#include "SpecialTile.hpp"
#include <vector>
#include <memory>
#include "Graph.hpp"

class GameMap:public Graph {
public:

    GameMap();
    //void addTile(std::shared_ptr<Tile> tile);
    //std::shared_ptr<Tile> getTile(int id) const;
    //int getNextTileId(int currentId, int steps) const;
    void displayMap() const;
    // int size() const { return tiles.size(); }
    // //
    bool canUpgradeStreet(PropertyTile* targetStreet, int playerID) const;
    // void initTile();
    // void initAdjacency();
    //int getTileAfterSteps(int currentTileId, int steps) const;
    int movebahn() const;
    int moveHub() const;
private:
    // 辅助函数：获取指定节点的地块类型名称
    //std::string getTileTypeName(int nodeId) const;

    // 辅助函数：检查两个节点是否属于同一组
    //bool isSameGroup(int node1, int node2) const;

    // 辅助函数：获取玩家拥有的地产数量
    //int getPlayerPropertyCount(int playerID) const;
    void printTileInfo(int nodeId) const;
};

#endif