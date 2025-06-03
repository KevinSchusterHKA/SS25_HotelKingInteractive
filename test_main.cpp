#include "Configuration.h"
#include <iostream>
using namespace std;
#define ROUND		10

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
        config.writeLog(i+1, 1, 200 + i, "egal", i + 5);  //spieler1
        config.writeLog(i+1, 2, 800 + i, "egal", i + 2);  //spieler2
        config.writeLog(i+1, 3, 2000 + i, "egal", i + 4);  //spieler3
    }

    return 0;
}
