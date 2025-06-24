
#include "Graph.hpp"
#include "PropertyTile.hpp"
#include "SpecialTile.hpp"
#include <stdexcept>

const std::string Graph::nodeNames[TOTAL_NODES] = {
    "LOS",
    "Kronenstraße",
    "Aktionsfeld",
    "Adlerstraße",
    "Einkommensteuer",
    "Hauptbahnhof",
    "Ebertstraße",
    "Aktionsfeld",
    "Rueppurrerstraße",
    "Ettlingerstraße",
    "Nur zu Besuch / Gefaengnis",
    "Amalienstraße",
    "Elektrizitaetwerk",
    "Hirschstraße",
    "Kriegsstraße",
    "West-Bahnhof",
    "Fastplatz",
    "Aktionsfeld",
    "Kaiser Allee",
    "Durlacher Allee",
    "Frei Parken",
    "Zirkel",
    "Aktionsfeld",
    "Karlstraße",
    "Brauerstraße",
    "OstBahnhof",
    "Hildapromenade",
    "Moltkestraße",
    "Wasserwerk",
    "Karl-friedrichstraße",
    "Gehe ins Gefaengnis",
    "Herrenstraße",
    "Waldstraße",
    "Aktionsfeld",
    "Erbprinzenstraße",
    "Hubschrauberlandeplatz",
    "Aktionsfeld",
    "Kaiserstraße",
    "Zusatzsteuer",
    "Schlossplatz"
};


//Constructor, initialize the adjacency matrix to construct the edges of the graph, and use a loop to insert nodes (Tile) according to the rent table

Graph::Graph() : adjacencyMatrix(TOTAL_NODES, std::vector<bool>(TOTAL_NODES, false)) {
    for (int i = 0; i < TOTAL_NODES; ++i) {
        adjacencyMatrix[i][(i + 1) % TOTAL_NODES] = true;
    }
    adjacencyMatrix[5][15]=true;
    adjacencyMatrix[15][5]=true;
    adjacencyMatrix[5][25]=true;
    adjacencyMatrix[25][5]=true;
    adjacencyMatrix[15][25]=true;
    adjacencyMatrix[25][15]=true;
    for (int i = 0; i < TOTAL_NODES; ++i) {
        if (i !=35)
        adjacencyMatrix[35][i] = true;
    }

     int currentId = 0;
    int groupIndex = 0;
    int tileIndex = 0;
    tiles.resize(TOTAL_NODES);
    tiles[0]=std::make_shared<SpecialTile>(LOS, "LOS", SpecialType::Start);
    
    
    while (groupIndex < groups.size()) {

        int groupSize;
        int basePrice;
        std::vector<int> rentLevels;
        std::vector<int> houseCosts;
        std::tie(groupSize, basePrice, rentLevels, houseCosts) = groups[groupIndex];

        for (int i = 0; i < groupSize; ++i) {
            currentId = Graph::PropertyTiles[tileIndex++];
            // Calculate the current node price
            int price = basePrice;
            if (groupSize == 3 && i == 2) {
                // Calculate the mean across groups
                int nextBase = std::get<1>(groups[(groupIndex + 1)]);
                price = (basePrice + nextBase) / 2;
                std::vector<int> nextrentLevels = std::get<2>(groups[(groupIndex + 1)]);
                // Compute element-wise average of rentLevels and nextrentLevels
                std::vector<int> averagedLevels;
                for (size_t idx = 0; idx < 6; ++idx) {
                    averagedLevels.push_back((rentLevels[idx] + nextrentLevels[idx]) / 2);
                }
                rentLevels = averagedLevels;
            }

            // Create Tile
            std::string name = nodeNames[currentId];
            auto tile = std::make_shared<PropertyTile>(
                currentId, name, PropertyType::Street, price,
                rentLevels[0], groupIndex, houseCosts[0]
            );
            
            // Set Rentlevel
            tile->setRentLevels(rentLevels);
            tiles[currentId]=tile;

        
        }

        groupIndex++;
    }

    //Inserting special tiles
    tiles[Aktionsfeld] = std::make_shared<SpecialTile>(
        Aktionsfeld, "Aktionsfeld", SpecialType::Action);

    tiles[Einkommensteuer] = std::make_shared<SpecialTile>(
        Einkommensteuer, "Einkommensteuer", SpecialType::Tax);

    tiles[Hauptbahnhof] = std::make_shared<SpecialTile>(
        Hauptbahnhof, "Hauptbahnhof", SpecialType::Bahnhof);
    tiles[WestBahnhof] = std::make_shared<SpecialTile>(
        WestBahnhof, "WestBahnhof", SpecialType::Bahnhof);

    tiles[Aktionsfeld2] = std::make_shared<SpecialTile>(
        Aktionsfeld2, "Aktionsfeld", SpecialType::Action);

    tiles[Gefaengnis] = std::make_shared<SpecialTile>(
        Gefaengnis, "Nur zu Besuch / Gefaengnis", SpecialType::GoToJail);

    tiles[Elektrizitaetwerk] = std::make_shared<PropertyTile>(
        Elektrizitaetwerk, nodeNames[Elektrizitaetwerk], PropertyType::Utility, 150, 0, -1, 0);

    tiles[Wasserwerk] = std::make_shared<PropertyTile>(
        Wasserwerk, nodeNames[Wasserwerk], PropertyType::Utility, 150, 0, -1, 0);

    tiles[Aktionsfeld3] = std::make_shared<SpecialTile>(
        Aktionsfeld3, "Aktionsfeld", SpecialType::Action);

    tiles[FreiParken] = std::make_shared<SpecialTile>(
        FreiParken, "Frei Parken", SpecialType::FreeParking);

    tiles[Aktionsfeld4] = std::make_shared<SpecialTile>(
        Aktionsfeld4, "Aktionsfeld", SpecialType::Action);

    tiles[OstBahnhof] = std::make_shared<SpecialTile>(
        OstBahnhof, "Ostbahnhof", SpecialType::Bahnhof);

    tiles[GeheinsGefaengnis] = std::make_shared<SpecialTile>(
        GeheinsGefaengnis, "Gehe ins Gefaengnis", SpecialType::GoToJail);

    tiles[Aktionsfeld5] = std::make_shared<SpecialTile>(
        Aktionsfeld5, "Aktionsfeld", SpecialType::Action);

    tiles[Hubschrauberlandeplatze] = std::make_shared<SpecialTile>(
        Hubschrauberlandeplatze, "Hubschrauberlandeplatze", SpecialType::Hubschrauberlandeplatz);

    tiles[Aktionsfeld6] = std::make_shared<SpecialTile>(
        Aktionsfeld6, "Aktionsfeld", SpecialType::Action);

    tiles[Zusatzsteuer] = std::make_shared<SpecialTile>(
        Zusatzsteuer, "Zusatzsteuer", SpecialType::LuxuryTax);


}
//Function: Given the current position ID, returns the ID of the next feasible tile
int Graph::getNextNode(int currentId) const {
    return (currentId + 1) % TOTAL_NODES;
}


//Function: Given a Tile ID, return a Tile object
std::shared_ptr<Tile> Graph::getTile(int id) const {
    return tiles[id];
}
//Function:Given a Tile ID, return a Tile name
std::string Graph::getNodeName(int id) const {
    return nodeNames[id];
}
// Function: Determine whether two Tiles are connected
bool Graph::hasEdge(int from, int to) const{


    return adjacencyMatrix[from][to];
}