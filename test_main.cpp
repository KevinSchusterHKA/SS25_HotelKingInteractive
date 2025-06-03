#include "Configuration.h"
#include <iostream>
using namespace std;

int main() {
    Configuration config;

    if (config.loadConfig("config.txt")) {
        cout << "Konfiguration erfolgreich geladen:\n";
        config.printSettings();
    }
    else {
        cout << "Fehler beim Laden der Konfiguration.\n";
    }

    return 0;
}