/**
 * This is the implementation of the PropertyTile class. Contains street information display, rent setting and calculation
 */
#ifndef PROPERTY_TILE_HPP
#define PROPERTY_TILE_HPP

#include "tile.hpp"
#include "vector"

enum class PropertyType { Street, Utility };

class PropertyTile : public Tile {
private:
    PropertyType propertyType;
    int price;
    int rent;
    int ownerId;

    int groudID;
    int buildingLevel; 
    int houseCost;
    std::vector<int> rentLevels; 
public:
    //PropertyTile(int id, const std::string& name, PropertyType type, int price): Tile(id, name), propertyType(type), price(price){}
    PropertyTile(int id, const std::string& name, PropertyType type, int price, int rent,int groundID, int houseCost)
        : Tile(id, name), propertyType(type), price(price), rent(rent), ownerId(-1), rentLevels({ 10, 50, 150, 450, 625, 750 }), houseCost(houseCost),
        buildingLevel(0), groudID(groudID) {}
// Getter
    PropertyType getPropertyType() const { return propertyType; }
    int getHouseCost() const { return houseCost; }
    void setBuildingLevel(int newLevel) { buildingLevel = newLevel; }

    int getPrice() const { return price; }
    int getRent() const { return rent; }
    int getOwnerId() const { return ownerId; }
    int getBuildingLevel() const { return buildingLevel; }
    int getGroupId() const { return groudID; }
    std::vector<int> getRentLevels() const {return rentLevels;}
    // Setter
    void setOwner(int playerId) { ownerId = playerId; }
    void setRent(int r) { rent = r; }
    /// <summary>
    /// Function: Set up the street rent table
    /// </summary>
    /// <param name="rents"></param>
    void setRentLevels(const std::vector<int>& rents);
    /// <summary>
    /// HKI-9 Map: Implementierung der Straßenfelder
    /// </summary>
    void displayInfo() const override;
    /// <summary>
    /// Function: Get street name
    /// </summary>
    /// <returns></returns>
    std::string getTypeString() const override;
     /// <summary>
     /// Function: Calculate the current rent to be paid (context requires additional information). Input parameters: dice random number, whether all public facilities are owned
     /// </summary>
     /// <param name="diceRoll"></param>
     /// <param name="ownsBothUtilities"></param>
     /// <returns></returns>
     int calculateRent(int diceRoll, bool ownsBothUtilities) const;
};

#endif
