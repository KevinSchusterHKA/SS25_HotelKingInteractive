#include "Map.hpp"

int test_map_main() {
    GameMap gameMap;
    gameMap.displayMap();

    int next = gameMap.getNextTile(3, 7);
    std::cout << "Spieler landet auf Feld: " << gameMap.getTile(next).name << "\n";
}