#include "map.hpp"
#include <iostream>
#include <PropertyTile.hpp>
#include <SpecialTile.hpp>


//HKI-8 Map: Erstellung einer Spielfeld-Klasse
GameMap::GameMap() {
    // 初始化地图顺序（图结构用于补充连接）
    initTile();
    initAdjacency();

}
//HKI-5 Map: Nutzung Graphenalgorithmus
//HKI-7 Map: Anpassung Adjazenzmatrix
void GameMap::initAdjacency() {
    const int N = tiles.size();
    adjacencyMatrix.clear();
    adjacencyMatrix.resize(N, std::vector<bool>(N, false));

    // 构建一个单向环结构（格 i 连到 i+1）
    for (int i = 0; i < N; ++i) {
        int next = (i + 1) % N;
        adjacencyMatrix[i][next] = true;
    }
}
//HKI-7 Map: Anpassung Adjazenzmatrix
int GameMap::getTileAfterSteps(int currentTileId, int steps) const {
    int pos = currentTileId;
    for (int i = 0; i < steps; ++i) {

        for (int next = 0; next < 40; ++next) {
            if (adjacencyMatrix[pos][next]) {
                pos = next;
                break;
            }
        }


    }
    return pos;
}
//HKI-11 Map: Implementierung der Bahnhofsfelder
int GameMap::movebahn() const {
    int pos=0;
    std::cout<<"which Bahnhof do you want to move? 5(Hbf), 15(West-Bahnhof), 25(Ost-Bahnhof)?"<<std::endl;
    std::cin>>pos;
    if (pos!=5||pos!=15||pos!=25){
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

// void GameMap::addTile((std::shared_ptr<Tile> tile)) {
//     tiles.push_back(tile);
// }

// void GameMap::connectTiles(int fromID, int toID) {
//     adjacencyMatrix[fromID][toID] = 1;
//     adjacencyMatrix[toID][fromID] = 1;
// }

std::shared_ptr<Tile> GameMap::getTile(int id) const {
    return tiles.at(id);
}

// int GameMap::getNextTile(int currentID, int steps) const {
//     int total = tiles.size();
//     return (currentID + steps) % total;
// }

void GameMap::displayMap() const {
    std::cout << "--- Spielfeld Übersicht ---\n";
    for (const auto& tilePtr : tiles) {
        auto street = dynamic_cast<PropertyTile*>(tilePtr.get());
        if (street){
        std::cout << street->getId() << " | " << street->getName() << " | Typ: " << street->getTypeString()<<
        " | Group: "<<street->getGroupId()<<" | OwnID: "<<street->getOwnerId()<<" | rent: "<<street->getRent()<<std::endl;
        continue;
        }
        else{
            auto sp = dynamic_cast<SpecialTile*>(tilePtr.get());
            std::cout<<" | Typ: "<<sp->getTypeString()<<std::endl;
        }
    }
}


//HKI-9 Map: Implementierung der Straßenfelder
//HKI-10 Map: Entwicklung der Sraßen-Eigenschaften
bool GameMap::canUpgradeStreet(PropertyTile* targetStreet, int playerId) const {
    if (!targetStreet) return false;
    if (targetStreet->getPropertyType() != PropertyType::Street) return false;
    if (targetStreet->getOwnerId() != playerId) return false;
    if (targetStreet->getBuildingLevel() >= 5) return false; // 最高为酒店

    int groupId = targetStreet->getGroupId();
    int targetLevel = targetStreet->getBuildingLevel();

    // 遍历地图中的所有地块
    for (const auto& tilePtr : tiles) {
        auto street = dynamic_cast<PropertyTile*>(tilePtr.get());
        if (!street) continue;
        // 跳过非街道或不同组的
        if (!street || street->getPropertyType() != PropertyType::Street) continue;
        if (street->getGroupId() != groupId) continue;

        // 必须整组都归当前玩家所有
        if (street->getOwnerId() != playerId) return false;

        // 平均建房规则：其他街道不得比目标地块低一级以上
        if (street != targetStreet && street->getBuildingLevel() < targetLevel) {
            return false;
        }
    }

    return true;
}
//HKI-4 Map: Konzepterstellung für Spielfeld
//HKI-6 Map: Anpassung auf Karlsruhe Straßen
//HKI-13 Map: Implementierung der Steuerfelder
//HKI-14 Map: Implementierung der Ereignisfelder für Aktionskarten
//HKI-15 Map: Implementierung der sonstigen Felder
void GameMap::initTile(){
    tiles.clear();
    tiles.push_back(std::make_shared<SpecialTile>(
    0, "LOS", SpecialType::Start));
// 地块 1: Adlerstraße
tiles.push_back(std::make_shared<PropertyTile>(
    1, "Kronenstraße", PropertyType::Street, 60, 2, 0, 50));
std::dynamic_pointer_cast<PropertyTile>(tiles.back())->setRentLevels({2, 10, 30, 90, 160, 250});
tiles.push_back(std::make_shared<SpecialTile>(
    2, "Gemeinschaftsfeld", SpecialType::Community));
// 地块 2: Kronenstraße
tiles.push_back(std::make_shared<PropertyTile>(
    3, "Adlerstraße", PropertyType::Street, 60, 2, 0, 50));
std::dynamic_pointer_cast<PropertyTile>(tiles.back())->setRentLevels({2, 10, 30, 90, 160, 250});
tiles.push_back(std::make_shared<SpecialTile>(
    4, "Einkommensteuer", SpecialType::Tax));
tiles.push_back(std::make_shared<PropertyTile>(
    //5, "Hauptbahnhof", PropertyType::Station, 200, 1, -1, 0));
    5, "Hauptbahnhof", SpecialType::Bahnhof));
// 地块 3: Rüppurrer Straße
tiles.push_back(std::make_shared<PropertyTile>(
    6, "Ebertstraße", PropertyType::Street, 100, 6, 1, 50));
std::dynamic_pointer_cast<PropertyTile>(tiles.back())->setRentLevels({6, 30, 90, 270, 400, 550});
tiles.push_back(std::make_shared<SpecialTile>(
    7, "Ereignisfeld", SpecialType::Event));
tiles.push_back(std::make_shared<PropertyTile>(
    8, "Rüppurrer Straße", PropertyType::Street, 100, 6, 1, 50));
std::dynamic_pointer_cast<PropertyTile>(tiles.back())->setRentLevels({6, 30, 90, 270, 400, 550});
tiles.push_back(std::make_shared<PropertyTile>(
    9, "Ettlingerstraße", PropertyType::Street, 120, 14, 1, 100));
std::dynamic_pointer_cast<PropertyTile>(tiles.back())->setRentLevels({14, 70, 200, 550, 750, 950});
tiles.push_back(std::make_shared<SpecialTile>(
    10, "Nur zu Besuch / Gefängnis", SpecialType::Jail));
tiles.push_back(std::make_shared<PropertyTile>(
    11, "Amalienstraße", PropertyType::Street, 140, 10, 2, 100));
std::dynamic_pointer_cast<PropertyTile>(tiles.back())->setRentLevels({10, 50, 150, 450, 625, 750});

tiles.push_back(std::make_shared<PropertyTile>(
    12, "Elektrizitätwerk", PropertyType::Utility, 150, 0, -1, 0));

tiles.push_back(std::make_shared<PropertyTile>(
    13, "Hirschstraße", PropertyType::Street, 140, 10, 2, 100));
std::dynamic_pointer_cast<PropertyTile>(tiles.back())->setRentLevels({10, 50, 150, 450, 625, 750});

tiles.push_back(std::make_shared<PropertyTile>(
    14, "Kriegsstraße", PropertyType::Street, 160, 12, 2, 100));
std::dynamic_pointer_cast<PropertyTile>(tiles.back())->setRentLevels({12, 60, 180, 500, 700, 900});

tiles.push_back(std::make_shared<PropertyTile>(
    //15, "West-Bahnhof", PropertyType::Station, 200, 1, -1, 0));
    15, "West-Bahnhof", SpecialType::Bahnhof));
tiles.push_back(std::make_shared<PropertyTile>(
    16, "Fastplatz", PropertyType::Street, 180, 14, 3, 150));
std::dynamic_pointer_cast<PropertyTile>(tiles.back())->setRentLevels({14, 70, 200, 550, 750, 950});

tiles.push_back(std::make_shared<SpecialTile>(
    17, "Gemeinschaftsfeld", SpecialType::Community));

tiles.push_back(std::make_shared<PropertyTile>(
    18, "Kaiser Allee", PropertyType::Street, 180, 14, 3, 150));
std::dynamic_pointer_cast<PropertyTile>(tiles.back())->setRentLevels({14, 70, 200, 550, 750, 950});

tiles.push_back(std::make_shared<PropertyTile>(
    19, "Durlacher Allee", PropertyType::Street, 200, 16, 3, 150));
std::dynamic_pointer_cast<PropertyTile>(tiles.back())->setRentLevels({16, 80, 240, 650, 860, 1000});

tiles.push_back(std::make_shared<SpecialTile>(
    20, "Frei Parken", SpecialType::FreeParking));

tiles.push_back(std::make_shared<PropertyTile>(
    21, "Zirkel", PropertyType::Street, 220, 18, 4, 200));
std::dynamic_pointer_cast<PropertyTile>(tiles.back())->setRentLevels({18, 90, 250, 700, 875, 1050});

tiles.push_back(std::make_shared<SpecialTile>(
    22, "Gemeinschaftsfeld", SpecialType::Community));
tiles.push_back(std::make_shared<PropertyTile>(
    23, "Karlstraße", PropertyType::Street, 220, 18, 4, 200));
std::dynamic_pointer_cast<PropertyTile>(tiles.back())->setRentLevels({18, 90, 250, 700, 875, 1050});
tiles.push_back(std::make_shared<PropertyTile>(
    24, "Brauerstraße", PropertyType::Street, 240, 20, 4, 200));
std::dynamic_pointer_cast<PropertyTile>(tiles.back())->setRentLevels({20, 100, 275, 750, 900, 1100});

tiles.push_back(std::make_shared<PropertyTile>(
    //25, "OstBahnhof", PropertyType::Station, 200, 1, -1, 0));
    25, "OstBahnhof", SpecialType::Bahnhof));
tiles.push_back(std::make_shared<PropertyTile>(
    26, "Hildapromenade", PropertyType::Street, 260, 22, 5, 150));
std::dynamic_pointer_cast<PropertyTile>(tiles.back())->setRentLevels({22, 110, 330, 800, 975, 1150});

tiles.push_back(std::make_shared<PropertyTile>(
    27, "Moltkestraße", PropertyType::Street, 260, 22, 5, 150));
std::dynamic_pointer_cast<PropertyTile>(tiles.back())->setRentLevels({22, 110, 330, 800, 975, 1150});

tiles.push_back(std::make_shared<PropertyTile>(
    28, "Wasserwerk", PropertyType::Utility, 150, 0, -1, 0));

tiles.push_back(std::make_shared<PropertyTile>(
    29, "Karl-friedrichstraße", PropertyType::Street, 280, 24, 5, 150));
std::dynamic_pointer_cast<PropertyTile>(tiles.back())->setRentLevels({24, 120, 360, 850, 1025, 1200});

tiles.push_back(std::make_shared<SpecialTile>(
    30, "Gehe ins Gefängnis", SpecialType::GoToJail));

tiles.push_back(std::make_shared<PropertyTile>(
    31, "Herrenstraße", PropertyType::Street, 300, 26, 6, 200));
std::dynamic_pointer_cast<PropertyTile>(tiles.back())->setRentLevels({26, 130, 390, 900, 1100, 1275});

tiles.push_back(std::make_shared<PropertyTile>(
    32, "Waldstraße", PropertyType::Street, 300, 26, 6, 200));
std::dynamic_pointer_cast<PropertyTile>(tiles.back())->setRentLevels({26, 130, 390, 900, 1100, 1275});

tiles.push_back(std::make_shared<SpecialTile>(
    33, "Gemeinschaftsfeld", SpecialType::Community));

tiles.push_back(std::make_shared<PropertyTile>(
    34, "Erbprinzenstraße", PropertyType::Street, 320, 28, 6, 200));
std::dynamic_pointer_cast<PropertyTile>(tiles.back())->setRentLevels({28, 150, 450, 1000, 1200, 1400});

tiles.push_back(std::make_shared<PropertyTile>(
    35, "Hubschrauberlandeplatz", SpecialType::Hubschrauberlandeplatz));

tiles.push_back(std::make_shared<SpecialTile>(
    36, "Ereignisfeld", SpecialType::Event));

// 地块 4: Ebertstraße

tiles.push_back(std::make_shared<PropertyTile>(
    37, "Kaiserstraße", PropertyType::Street, 350, 40, 7, 250));
std::dynamic_pointer_cast<PropertyTile>(tiles.back())->setRentLevels({40, 180, 500, 1200, 1500, 1750});

tiles.push_back(std::make_shared<SpecialTile>(
    38, "Zusatzsteuer", SpecialType::LuxuryTax));

tiles.push_back(std::make_shared<PropertyTile>(
    39, "Schlossplatz", PropertyType::Street, 400, 50, 7, 250));
std::dynamic_pointer_cast<PropertyTile>(tiles.back())->setRentLevels({50, 200, 600, 1400, 1700, 2000});

}