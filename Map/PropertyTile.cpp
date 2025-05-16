#include "Propertytile.hpp"
#include <iostream>

// 显示地块基本信息
void PropertyTile::displayInfo() const {
    std::cout << "[Property] ID: " << id << " | " << name;

    switch (propertyType) {
        case PropertyType::Street:
            std::cout << " (Straße)";
            break;
        case PropertyType::Station:
            std::cout << " (Bahnhof)";
            break;
        case PropertyType::Utility:
            std::cout << " (Versorgung)";
            break;
    }

    std::cout << " | Preis: " << price
              << " | Miete: ";

    if (propertyType == PropertyType::Utility) {
        std::cout << "(x4 oder x10 Würfelzahl)";
    } else if (propertyType == PropertyType::Station) {
        std::cout << "(25/50/100/200 je nach Besitz)";
    } else {
        std::cout << rent;
    }

    std::cout << " | Besitzer: ";
    if (ownerId == -1) {
        std::cout << "Keiner";
    } else {
        std::cout << "Spieler " << ownerId;
    }

    std::cout << std::endl;
}

// 计算当前应支付租金（上下文需要提供额外信息）
int PropertyTile::calculateRent(int diceRoll, int stationOwned, bool ownsBothUtilities) const {
    switch (propertyType) {
        case PropertyType::Street:
            // 若为街道类型，使用 buildingLevel 选择对应租金（0 = 空地，5 = 酒店）
            if (buildingLevel >= 0 && buildingLevel < rentLevels.size()) {
                return rentLevels[buildingLevel];
            } else {
                return rent; // 兜底：无租金表时使用默认
            }

        case PropertyType::Station:
            // 按照拥有车站数返回标准租金
            switch (stationOwned) {
                case 1: return 25;
                case 2: return 50;
                case 3: return 100;
                case 4: return 200;
                default: return 0;
            }

        case PropertyType::Utility:
            // 水厂/电厂 → 骰子点数 × 系数（1个 ×4，两个 ×10）
            return diceRoll * (ownsBothUtilities ? 10 : 4);
    }
    return 0;
}

void PropertyTile::setRentLevels(const std::vector<int>& rents) {
    if (rents.size() != 6) {
        throw std::invalid_argument("RentLevels must have exactly 6 entries: [no house, 1H, 2H, 3H, 4H, hotel]");
    }
    this->rentLevels = rents;
}