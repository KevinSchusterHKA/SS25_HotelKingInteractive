
#include "map.hpp"
#include <iostream>



//HKI-8 Map: Erstellung einer Spielfeld-Klasse
GameMap::GameMap() :Graph() {}
   

void GameMap::displayMap() const {
    std::cout << "\n=== Map Tile Attributes ===\n";
    for (int nodeId = 0; nodeId < TOTAL_NODES; ++nodeId) {
        printTileInfo(nodeId);
    }
}

//Function: Display the node information of the specified ID
void GameMap::printTileInfo(int nodeId) const {
    const auto& tile = tiles[nodeId];
    std::cout << "Node ID: " << nodeId << " | Name: " << tile->getName() << "\n";

    if (auto propTile = dynamic_cast<const PropertyTile*>(tile.get())) {
        std::cout << "  Type: Property\n"
                  << "  Price: " << propTile->getPrice() << "\n"
                  << "  Rent Levels: ";
        for (int rent : propTile->getRentLevels()) {
            std::cout << rent << " ";
        }
        std::cout << "\n";
    } else if (auto specialTile = dynamic_cast<const SpecialTile*>(tile.get())) {
        std::cout << "  Type: Special\n"
                  << "  Category: " << specialTile->getTypeString() << "\n";
    } else {
        std::cout << "  Type: Unknown\n";
    }

    std::cout << "-------------------------\n";
}




//HKI-11 Map: Implementierung der Bahnhofsfelder
int GameMap::movebahn() const {
    int pos=0;
    std::cout<<"which Bahnhof do you want to move? 5(Hbf), 15(West-Bahnhof), 25(Ost-Bahnhof)?"<<std::endl;
    std::cin>>pos;
    if (pos!=5&&pos!=15&&pos!=25){
        std::cout<<"input error, try again"<<std::endl;
        return 0;
    }
    
    else
    return pos;
}
//HKI-12 Map: Implementierung des Hubschrauberlandeplatzes
int GameMap::moveHub() const {
    int pos=0;
    std::cout<<"which pos do you want to move?"<<std::endl;
    std::cin>>pos;
    if (pos<0||pos>tiles.size()){
        std::cout<<"input error, try again"<<std::endl;
        return 0;
    }
    
    else
    return pos;
}




//HKI-9 Map: Implementierung der Straßenfelder
//HKI-10 Map: Entwicklung der Sraßen-Eigenschaften
bool GameMap::canUpgradeStreet(PropertyTile* targetStreet, int playerId) const {
    if (!targetStreet) return false;
    if (targetStreet->getPropertyType() != PropertyType::Street) return false;
    if (targetStreet->getOwnerId() != playerId) return false;
    if (targetStreet->getBuildingLevel() >= 5) return false; 

    int groupId = targetStreet->getGroupId();
    int targetLevel = targetStreet->getBuildingLevel();

   
    for (const auto& tilePtr : tiles) {
        auto street = dynamic_cast<PropertyTile*>(tilePtr.get());
        if (!street) continue;
        
        if (!street || street->getPropertyType() != PropertyType::Street) continue;
        if (street->getGroupId() != groupId) continue;

        
        if (street->getOwnerId() != playerId) return false;

        
        if (street != targetStreet && street->getBuildingLevel() < targetLevel) {
            return false;
        }
    }

    return true;
}
