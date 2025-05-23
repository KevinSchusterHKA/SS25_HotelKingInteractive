// HKI.cpp : Diese Datei enthält die Funktion "main". Hier beginnt und endet die Ausführung des Programms.
//

#include <iostream>
#include "Map.hpp"

int main()
{
    std::cout << "Hotel King Interactive T0.1\n";

    GameMap gameMap;
    gameMap.displayMap();

    int next = gameMap.getNextTile(3, 7);
    std::cout << "Spieler landet auf Feld: " << gameMap.getTile(next).name << "\n";

}

