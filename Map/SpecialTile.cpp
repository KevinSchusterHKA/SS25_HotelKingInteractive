#include "SpecialTile.hpp"
#include <iostream>


//HKI-13 Map: Implementierung der Steuerfelder
//HKI-14 Map: Implementierung der Ereignisfelder für Aktionskarten
std::string SpecialTile::getTypeString() const {
    switch (specialType) {
        case SpecialType::Start:        return "Start";
        case SpecialType::Jail:         return "Jail";
        case SpecialType::GoToJail:     return "GoToJail";
        case SpecialType::FreeParking:  return "FreeParking";
        case SpecialType::Tax:     return "Tax";
        case SpecialType::Event:        return "Event";
        case SpecialType::Community:    return "Community";
        case SpecialType::Hubschrauberlandeplatz: return "Hubschrauberlandeplatz";
        case SpecialType::Bahnhof:      return "Bahnhof";
        default: return "Special";
    }
}