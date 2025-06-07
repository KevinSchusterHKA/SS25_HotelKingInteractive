#include <iostream>
#include <vector>
#include <string>

#include "Server.hpp"
#include "Player.hpp"
#include "Configuration.h"
#include "GameFunctionManager.hpp"
#include "MenuManager.hpp"
#include "Menu.hpp"


using namespace std;

vector<Player> Server::showMainDialog() {

	MenuManager menumanager = MenuManager();

	cout << "Wie viele Spieler spielen? (1-4) " << endl;					//Anzahl der Spieler abfragen
	cin >> numberOfPlayer;

	while (numberOfPlayer != 1 || numberOfPlayer != 2 || numberOfPlayer != 3 || numberOfPlayer != 4) {

		cout << "ungueltige Eingabe \n \n Wie viele Spieler spielen? (1-4) " << endl;		//Anzahl der Spieler nochmal abfragen wenn ungültig
		cin >> numberOfPlayer;

	}

	
	for (int i = 1; i = numberOfPlayer; i++) {
		cout << "Name Spieler " << numberOfPlayer << ":" << endl;		//Spieler Namen vergeben
		cin >> playerName;

		while (playerName.empty()) {

			cout << "Spielername darf nicht leer sein!? \n \n Name Spieler " << numberOfPlayer << ":" << endl;		//Spieler Namen vergeben
			cin >> playerName;

		}

		//menumanager.getGameFunctionManager().addPlayer(Player(playerName, 1500, i));		//Spieler mit Namen, Anfangsbudget und ID erstellen

		Player realPlayer = Player(playerName,1500,i);						
		players.push_back(realPlayer);
		return player;

	}

	numberOfCPUPlayer = 4 - numberOfPlayer;
	Player anzahlCPU = CPU(numberOfCPUPlayer); // Funktion in Player für CPU Gegner mit Übergabeparameter Anzahl CPU Gegner
	
	for (int i = 1; i = numberOfCPUPlayer; i++) {						// CPU Spieler Namen und ID erstelen

		playerName = "CPU Spieler " + i;

		//menumanager.getGameFunctionManager().addPlayer(Player(playerName, 1500, i));

		Player cpuPlayer = Player(playerName, 1500, 4 - numberOfCPUPlayer + i);
		players.push_back(cpuPlayer);
		return players;
	}
	cout << "Pfad zu den Spielregeleinstellungen: " << endl;		// Spieregeln einlesen
	cin >> rulesPath;

	Configuration config;

	while (!config.loadConfig(rulesPath)) {	
	
		cout << "Pfad ungültig \n \n Pfad zu den Spielregeleinstellungen: " << endl;
		cin >> rulesPath;
	
	};

	cout << "Spielregeln wurden erfolgreich geladen: \n" << endl;

	config.printSettings();

	cout << "Spiel wird gestartet\n" << endl;

	players[1].setPosition(0);									//alle Spieler auf Los
	players[2].setPosition(0);
	players[3].setPosition(0);
	players[4].setPosition(0);

	menumanager.setInGame(true);									
	menumanager.getGameFunctionManager().setCurrentPlayer(1);	//1. Spieler beginnt
	menumanager.getGameFunctionManager().setCurrentRound(1);	//1. Runde
	showIngameDialog();
	
}


void Server::showIngameDialog() {

	MenuManager menumanager = MenuManager();
	GameFunctionManager gamefunctionmanager;

	vector<Player>& players = gamefunctionmanager.getPlayers();
	int id = gamefunctionmanager.getCurrentPlayer();
	string name = players[id].getName();
	int budget = players[id].getMoney();
	Player player = Player(name, budget, id);


	menumanager.setCurrentMenu(menumanager.getMenus()[1]);
	menumanager.handleMenus();									//InGameMenu wird aufgerufen


	do {	
		if (gamefunctionmanager.getPaschCounter() == 3) {

			cout << "Das war dein 3. Pasch... Gehe ins Gefängnis" << endl;
			player.inPrison();
		}
		else {

			players[id].move(gamefunctionmanager.rollDice()[0] + gamefunctionmanager.rollDice()[1]); //kann man so? ich möchte Summe aus gewürfelten Zahlen


		}

	
	} while (gamefunctionmanager.getPaschCounter() == 1 || gamefunctionmanager.getPaschCounter() == 2);

	

}