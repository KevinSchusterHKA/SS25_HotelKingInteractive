#include "Configuration.h"
#include <iostream>
using namespace std;
#define ROUND		20

int main() {
    Configuration config;

/***************************************   test loadConfig  **********************************************/
    if (config.loadConfig("config.txt")) {
        cout << "Konfiguration erfolgreich geladen:\n";
        config.printSettings();
    }
    else {
        cout << "Fehler beim Laden der Konfiguration.\n";
    }

/***************************************   test writeLog  **********************************************/
    for (int i = 0; i < ROUND; i++) {
        config.writeLog({ i + 1, 1, 500 + i, "egal + asdf", i + 5 });  //spieler1
        config.writeLog({ i + 1, 2, 1000 + i, "egal", i + 2 });  //spieler2
        config.writeLog({ i + 1, 3, 2000 + i, "egal", i + 4 });  //spieler3
    }

/***************************************   test saveGame  **********************************************/
    config.saveGame("game.log", "save.txt", 3);

    return 0;
}
