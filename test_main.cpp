#include "Configuration.h"
#include "Player.hpp"
#include "GameFunctionManager.hpp"
#include <iostream>
using namespace std;
#define ROUND		20

int config_main() {
    Configuration config;
/***************************************   test loadConfig  **********************************************/
    if (config.loadConfig()) {
        cout << "Konfiguration erfolgreich geladen:\n";
        config.printSettings();
    }
    else {
        cout << "Fehler beim Laden der Konfiguration.\n";
    }

/***************************************   game test  **********************************************/
    GameSettings setting = config.getSettings();
    Player p1("Emilie", setting.startBudget, 0);
    Player p2("bob", setting.startBudget, 1);
    Player p3("Alice", setting.startBudget, 2);

    p1.setPosition(5);
    p1.addMoney(500);
    p1.addKarte("StrasseA");
    p1.addKarte("StrasseB");
    p2.addKarte("StrasseC");
    p2.setPosition(10);
    p3.setPosition(15);

    GameFunctionManager info;
    info.addPlayer(p1);
    info.addPlayer(p2);
    info.addPlayer(p3);
    info.setCurrentPlayer(0);
    info.setCurrentRound(0);

/***************************************   test writeLog  **********************************************/
    vector<Player>& players = info.getPlayers();    //lesen Spielers
    for (int i = 1; i < ROUND; i++) {
        info.setCurrentRound(i);
        info.setCurrentPlayer(0); players[0].addMoney(100); players[0].setPosition(6*i);     //spieler1
        config.writeLog(info);
        info.setCurrentPlayer(1); players[1].addMoney(200); players[1].setPosition(2 * i);   //spieler2
        config.writeLog(info);
        info.setCurrentPlayer(2); players[2].addMoney(500); players[2].setPosition(1 * i); players[2].addKarte("TEST"); //spieler3
        config.writeLog(info);
    }
    cout << "test log fertig" << endl << endl;
    //config.clearLog();

/***************************************   test saveGame  **********************************************/
    config.saveGame();
    cout << "test save fertig" << endl << endl;

    /***************************************   test loadGame  **********************************************/
    GameFunctionManager manager = config.loadGame();
    config.printLoadGame(manager);

    /***************************************   test Highscore  **********************************************/
    config.sammlungHighscore(players);
    vector<Player> sortedPlayer;
    sortedPlayer = config.sortedHighscore();
    cout << endl;
    config.showHighscore(sortedPlayer);

    return 0;
}
