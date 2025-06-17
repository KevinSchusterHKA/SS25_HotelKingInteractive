
#include "Propertytile.hpp"
#include <iostream>

///
///HKI-9 Map: Implementierung der Straßenfelder
///
void PropertyTile::displayInfo() const {
    std::cout << "[Property] ID: " << id << " | " << name;

    switch (propertyType) {
        case PropertyType::Street:
            std::cout << " (Straße)";
            break;

        case PropertyType::Utility:
            std::cout << " (Versorgung)";
            break;
    }

    std::cout << " | Preis: " << price
              << " | Miete: ";

    if (propertyType == PropertyType::Utility) {
        std::cout << "(x4 oder x10 Würfelzahl)";
    } 

    else {
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

//Function: Calculate the current rent to be paid (context requires additional information). Input parameters: dice random number, whether all public facilities are owned
int PropertyTile::calculateRent(int diceRoll, bool ownsBothUtilities) const {
    switch (propertyType) {
        case PropertyType::Street:
            if (buildingLevel >= 0 && buildingLevel < rentLevels.size()) {
                return rentLevels[buildingLevel];
            } else {
                return rent; 
            }

        case PropertyType::Utility:
            return diceRoll * (ownsBothUtilities ? 10 : 4);
    }
    return 0;
}
//Function: Set up the street rent table
void PropertyTile::setRentLevels(const std::vector<int>& rents) {
    if (rents.size() != 6) {
        throw std::invalid_argument("RentLevels must have exactly 6 entries: [no house, 1H, 2H, 3H, 4H, hotel]");
    }
    this->rentLevels = rents;
}
//Function: Get street name
std::string PropertyTile::getTypeString() const {
    switch (propertyType) {
        case PropertyType::Street:  return "Street";
        // case PropertyType::Station: return "Station";
        case PropertyType::Utility: return "Utility";
        default: return "Property";
    }
}