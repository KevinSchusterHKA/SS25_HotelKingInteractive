#ifndef PROPERTY_TILE_HPP
#define PROPERTY_TILE_HPP

#include "tile.hpp"
#include "vector"

enum class PropertyType { Street, Station, Utility };

class PropertyTile : public Tile {
private:
    PropertyType propertyType;
    int price;
    int rent;
    int ownerId; // -1 表示无人拥有

    int groudID;
    int buildingLevel; // 0 = 无，1–4 = 房，5 = 酒店
    int houseCost;
    std::vector<int> rentLevels; // 长度6: [0房, 1房, ..., 酒店]
    // 仅对 Street 有效
    void upgrade(); // 增加一栋房子或升级到酒店
    bool canUpgrade() const; // 判断是否达到最大建筑数
    int getRentByLevel() const;
public:
    //PropertyTile(int id, const std::string& name, PropertyType type, int price): Tile(id, name), propertyType(type), price(price){}
    PropertyTile(int id, const std::string& name, PropertyType type, int price, int rent, int houseCost)
        : Tile(id, name), propertyType(type), price(price), rent(rent), ownerId(-1),rentLevels({10, 50, 150, 450, 625, 750}),houseCost(100),
buildingLevel(0),groudID(-1) {}
// Getter
    PropertyType getPropertyType() const { return propertyType; }
    int getPrice() const { return price; }
    int getRent() const { return rent; }
    int getOwnerId() const { return ownerId; }
    int getBuildingLevel() const { return buildingLevel; }
    int getGroupId() const { return groudID; }
    // Setter
    void setOwner(int playerId) { ownerId = playerId; }
    void setRentLevels(const std::vector<int>& rents);
    void displayInfo() const override;
     int calculateRent(int diceRoll, int stationOwnedCount, bool ownsBothUtilities) const;
};

#endif