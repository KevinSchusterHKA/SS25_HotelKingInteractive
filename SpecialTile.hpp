#ifndef SPECIAL_TILE_HPP
#define SPECIAL_TILE_HPP

#include "tile.hpp"

enum class SpecialType { Start, Event,Community, Bahnhof, Tax,LuxuryTax, Jail, GoToJail, FreeParking, Hubschrauberlandeplatz};

class SpecialTile : public Tile {
private:
    SpecialType specialType;

public:
    SpecialTile(int id, const std::string& name, SpecialType type)
        : Tile(id, name), specialType(type) {}

    void displayInfo() const override;
    SpecialType getSpecialType() const { return specialType; }
    std::string getTypeString() const override;
};

#endif