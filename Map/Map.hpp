#ifndef MAP_HPP
#define MAP_HPP

#include "tile.hpp"
#include <PropertyTile.hpp>
#include <SpecialTile.hpp>
#include <vector>
#include <memory>

class GameMap {
private:
    std::vector<std::shared_ptr<Tile>> tiles;

public:
    GameMap();
    void addTile(std::shared_ptr<Tile> tile);
    std::shared_ptr<Tile> getTile(int id) const;
    int getNextTileId(int currentId, int steps) const;
    void displayMap() const;
    int size() const { return tiles.size(); }
    //
    bool canUpgradeStreet(PropertyTile* targetStreet, int playerID) const;
    void initTile();
};

#endif