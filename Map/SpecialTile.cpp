#include "SpecialTile.hpp"
#include <iostream>


//HKI-13 Map: Implementierung der Steuerfelder
std::string SpecialTile::getTypeString() const {
    switch (specialType) {
        case SpecialType::Start:        return "Start";
        case SpecialType::Jail:         return "Jail";
        case SpecialType::GoToJail:     return "GoToJail";
        case SpecialType::FreeParking:  return "FreeParking";
        case SpecialType::Tax:     return "Tax";
        case SpecialType::Event:        return "Event";
        case SpecialType::Community:    return "Community";
        default: return "Special";
    }
}