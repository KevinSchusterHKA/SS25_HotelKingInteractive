/**
 * This is the implementation of the Map class. Contains functions for displaying map information, moving train stations and airports, and determining whether streets are upgradeable
 * HKI-8 Map: Erstellung einer Spielfeld-Klasse
 */
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
    /// <summary>
    /// //Function: loop to display all node information
    /// </summary>
    void displayMap() const;
    /// <summary>
    /// //Function: Verify if the street can be updated //HKI-9 Map: Implementierung der Strassenfelder HKI-10 Map: Entwicklung der Srassen-Eigenschaften
    /// </summary>
    /// <param name="targetStreet"></param>
    /// <param name="playerID"></param>
    /// <returns></returns>
    bool canUpgradeStreet(PropertyTile* targetStreet, int playerID) const;
    /// <summary>
    /// HKI-11 Map: Implementierung der Bahnhofsfelder
    /// </summary>
    /// <returns></returns>
    int movebahn() const;
    /// <summary>
    /// HKI-12 Map: Implementierung des Hubschrauberlandeplatzes
    /// </summary>
    /// <returns></returns>
    int moveHub() const;
private:
    /// <summary>
    /// Function: Display the node information of the specified ID
    /// </summary>
    /// <param name="nodeId"></param>
    void printTileInfo(int nodeId) const;
};

#endif