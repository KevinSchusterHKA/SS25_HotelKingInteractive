/**
 * This is the implementation of the SpecialTile class.Provide special tile types
 */
#ifndef SPECIAL_TILE_HPP
#define SPECIAL_TILE_HPP

#include "tile.hpp"

enum class SpecialType { Start, Action, Bahnhof, Tax,LuxuryTax, Jail, GoToJail, FreeParking, Hubschrauberlandeplatz};

class SpecialTile : public Tile {
private:
    SpecialType specialType;

public:
    SpecialTile(int id, const std::string& name, SpecialType type)
        : Tile(id, name), specialType(type) {}

    void displayInfo() const override;
    /// <summary>
    /// //HKI-13 Map: Implementierung der Steuerfelder HKI-14 Map: Implementierung der Ereignisfelder f¨¹r Aktionskarten
    /// </summary>
    /// <returns></returns>
    SpecialType getSpecialType() const { return specialType; }
    std::string getTypeString() const override;
};

#endif