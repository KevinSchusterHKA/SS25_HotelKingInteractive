#include <iostream>
#include <vector>
#include <string>
#include <thread>
#include <chrono>

#include "Server.hpp"
#include "Player.hpp"
#include "Configuration.h"
#include "GameFunctionManager.hpp"
#include "MenuManager.hpp"
#include "Menu.hpp"
#include "PropertyTile.hpp"
#include "SpecialTile.hpp"


using namespace std;

Server::Server(std::vector<Player>& playersRef)
	: players(playersRef) {
}

void Server::SpielStarten() {

	MenuManager menumanager = MenuManager();

	cout << "Pfad zur Konfiguration: " << rulesPath << endl;				// Konfigurationsdatei einlesen
	//cin >> rulesPath; Configpfad selber eingeben

	Configuration config;

	while (!config.loadConfig(rulesPath)) {

		cout << "Pfad ungültig \n \n Pfad zur Konfigurationsdatei: " << endl;
		cin >> rulesPath; //selber eingeben, falls ungültig
	};
	cout << "Konfiguraation wurde erfolgreich geladen: \n" << endl;
	//config.printSettings();

	GameSettings settings = config.getSettings();
	

	cout << "Wie viele Spieler spielen? (1-4) " << endl;					//Anzahl der Spieler abfragen
	cin >> numberOfPlayer;

	while (numberOfPlayer != 1 && numberOfPlayer != 2 && numberOfPlayer != 3 && numberOfPlayer != 4) {

		cout << "ungueltige Eingabe \n \n Wie viele Spieler spielen? (1-4) " << endl;		//Anzahl der Spieler nochmal abfragen wenn ungültig
		cin >> numberOfPlayer;

	}

	
	for (int i = 0; i < numberOfPlayer; i++) {
		cout << "Name Spieler " << i+1 << ":" << endl;		//Spieler Namen vergeben
		cin >> playerName;

		while (playerName.empty()) {

			cout << "Spielername darf nicht leer sein!? \n \n Name Spieler " << i+1 << ":" << endl;		//Spieler Namen vergeben
			cin >> playerName;

		}

		Player realPlayer(playerName, settings.startBudget, i);
			
		menumanager.getGameFunctionManager().addPlayer(realPlayer);		//Spieler mit Namen, Anfangsbudget und ID erstellen
	
		
		//players.push_back(realPlayer);
		//return players;

	}
	numberOfCPUPlayer = 4 - numberOfPlayer;
	//Player anzahlCPU = CPU(numberOfCPUPlayer); // Funktion in Player für CPU Gegner mit Übergabeparameter Anzahl CPU Gegner
	cout << "Anzahl CPU Gegner: " << numberOfCPUPlayer << endl;

	
	for (int i = 0; i < numberOfCPUPlayer; i++) {						// CPU Spieler Namen und ID erstelen

		playerName = "CPU Spieler " + to_string(i);
		

		Player cpuPlayer(playerName, settings.startBudget, 4 - numberOfCPUPlayer + i);

		//players.push_back(cpuPlayer);
		
		menumanager.getGameFunctionManager().addPlayer(cpuPlayer);

		//return players;
	}
	

	

	cout << "Spiel wird gestartet\n" << endl;

	players = menumanager.getGameFunctionManager().getPlayers();
	players[0].setPosition(0);
	players[1].setPosition(0);									//alle Spieler auf LOS
	players[2].setPosition(0);
	players[3].setPosition(0);

	cout << "Alle Spieler werden auf LOS gesetzt" << endl;
	//cout << "Position " << players[0].getName() << ": " << players[0].getPosition() << endl; //test


	
	menumanager.getGameFunctionManager().setCurrentPlayer(3);	//1. Spieler beginnt
	menumanager.getGameFunctionManager().setCurrentRound(1);	//1. Runde
	//cout << "Current Player: " << menumanager.getGameFunctionManager().getCurrentPlayer() << " Current Round: " << menumanager.getGameFunctionManager().getCurrentRound() << endl; //test
	//cout << players[0].getName() << " beginnt" << endl;

////////////////////////////////////////////////////////////saveGame speichert nicht die aktuellen Spielstände ab sondern immer die gleichen voreingestellten////////////////////////////////////////////
	config.saveGame("game.log", "savetest.txt", 4);
	cout << "Spieler gespeichert" << endl << endl;
	

	this_thread::sleep_for(chrono::milliseconds(4000));
	showIngameDialog();
}


void Server::SpielLaden() {
	Configuration config;
	gamefunctionmanager = config.loadGame("save.txt");

	cout << "Aktuelle Runde: " << gamefunctionmanager.getCurrentRound() << endl;
	cout << "Aktueller Spieler: " << gamefunctionmanager.getCurrentPlayer() << endl;

	for (const Player& p : gamefunctionmanager.getPlayers()) {
		cout << "-----------------------------" << endl;
		cout << "Name: " << p.getName() << endl;
		cout << "ID: " << p.getID() << endl;
		cout << "Budget: " << p.getMoney() << endl;
		cout << "Position: " << p.getPosition() << endl;
		cout << "Im Gefaengnis? " << (p.inPrison() ? "Ja" : "Nein") << endl;
		cout << "Gefaengnis-Runden: " << p.getPrisonCount() << endl;
		cout << endl;
	}
	players = gamefunctionmanager.getPlayers();
	//cout << "Position " << players[0].getName() << ": " << players[0].getPosition() << endl; //test

	this_thread::sleep_for(chrono::milliseconds(5000));
	showIngameDialog();
}
void Server::showIngameDialog() {
	///test ob alle Spieler richtig übernommen
	cout << "Player 1: " << players[0].getName() << "Position " << players[0].getPosition() << endl;	
	cout << "Player 2: " << players[1].getName() << "Position " << players[1].getPosition() << endl;
	cout << "Player 3: " << players[2].getName() << "Position " << players[2].getPosition() << endl;
	cout << "Player 4: " << players[3].getName() << "Position " << players[3].getPosition() << endl;	

	this_thread::sleep_for(chrono::milliseconds(50000));
	
	/*
	

	menumanager.setInGame(true);

	int id = gamefunctionmanager.getCurrentPlayer();
	//string name = players[id].getName();
	//int budget = players[id].getMoney();
	//Player player = Player(name, budget, id);
	cout << "id:" << id << endl;

	this_thread::sleep_for(chrono::milliseconds(500));
	menumanager.setCurrentMenu(menumanager.getMenus()[1]);
	menumanager.handleMenus();									//InGameMenu wird aufgerufen


	do {																			//Spielzug
		vector<int> dice = gamefunctionmanager.rollDice();
		cout << "gewürfelte Zahlen: " << dice[0] << ", " << dice[1] << endl;

		if (menumanager.getGameFunctionManager().getPaschCounter() == 3) {							//Wenn 3. Mal Pasch -> Gefängnis

			cout << "Das war dein 3. Pasch... Gehe ins Gefängnis" << endl;
			players[id].inPrison();
		}
		else {
			players[id].move(dice[0] + dice[1]); //move Summe aus gewürfelten Zahlen
			dice.clear();
			int tile = players[id].getPosition();
			cout << "Position von " << players[id].getName() << ": " << players[id].getPosition() << endl;

			/* menumanager.getGameFunctionManager().showTileInfomation(tile);								//Feldeigenschaften anzeigen

			for (int i = 0; i < 4; i++) {												//checken, ob schon jemand auf dem Feld steht
				if (i != id){
					positionofothers = players[i].getPosition();
					if (positionofothers == tile) {								//dieser Player steht schon auf dem Feld
						cout << players[i].getName() << "befindet sich gerade schon auf diesem Feld. Gib 50 Euro ab!" << endl;
						if (players[id].getMoney() >= 50) {
							players[id].addMoney(-50);
							players[i].addMoney(50);
						}
						else {													//nicht genug Geld
							cout << "Du hast aber leider nicht genug Geld dafür... \nGame Over für dich :(";
							//Game Over
						}
					}
				}
			} 
			
			auto& sTile = menumanager.getGameFunctionManager().getMap().tiles[tile];

			if (auto propTile = dynamic_cast<PropertyTile*>(sTile.get())) {		//auf Property Tile

				price = propTile->getPrice();

				if (propTile->getOwnerId() == -1) {											//wenn Feld noch nicht gekauft
					if (players[id].getMoney() >= price) {									//wenn genug Geld zum Kaufen da
						cout << "Möchtest du das Feld kaufen? (0: nein, 1:ja)" << endl;
						cin >> buyfield;
						if (buyfield) {
							players[id].addMoney(-price);
							propTile->setOwner(id);
						}
						else {
							cout << "ok dann nicht" << endl;
						}
					}
					else {
						cout << "Du hast leider nicht genug Geld, um dieses Feld zu kaufen. Weiterr" << endl;	//nicht genug Geld zum Kaufen da
					}
				}
				else {																			//Miete zahlen, wenn vergeben
					rent = propTile->getRent();
					ownerid = propTile->getOwnerId();

					cout << "Das Fed besitzt " << ownerid << ". Du musst " << rent << " Euro Miete zahlen." << endl;

					if (players[id].getMoney() >= rent) {					//genug Geld für Rente

						players[id].addMoney(-rent);
						players[ownerid].addMoney(rent);
					}
					else {													//nicht genug Geld für Rente
						cout << "Du hast aber leider nicht genug Geld dafür... \nGame Over für dich :(";
							//Game Over
					}
				}
			}
			else if (auto specialTile = dynamic_cast<const SpecialTile*>(sTile.get())) {	//auf Special Tile
				
				specialtile = specialTile->getTypeString();
				if(specialtile == "Start") {
					players[id].addMoney(400);									//Wenn man direkt auf Los landet bekommt man 400 Euro
				}
				else if (specialtile == "Jail") {
					cout << "Du bist nur zu Besuch hier" << endl;
				}
				else if (specialtile == "GoToJail") {							//Ins Gefängnis gehen
					cout << "Gehe ins Gefängnis!" << endl;
					players[id].inPrison();
				}
				else if (specialtile == "FreeParking") {						//Frei Parken
					cout << "Hier kannst du chillen und frei parken :)" << endl;
				}
				else if (specialtile == "Tax") {								//Einkommenssteuer zahlen
					cout << "Zahle 200 Euro Einkommenssteuer!" << endl;
					if (players[id].getMoney() >= 100) {					//genug Geld

						players[id].addMoney(-200);
					}
					else {													//nicht genug Geld
						cout << "Du hast aber leider nicht genug Geld dafür... \nGame Over für dich :(";
						//Game Over
					}
				}
				else if (specialtile == "LuxuryTax") {							//Zusatzsteuer zahlen

					cout << "Zahle 100 Euro Zusatzsteuer!" << endl;
					if (players[id].getMoney() >= 200) {					//genug Geld

						players[id].addMoney(-200);
					}
					else {													//nicht genug Geld
						cout << "Du hast aber leider nicht genug Geld dafür... \nGame Over für dich :(";
						//Game Over
					}
				}
				else if (specialtile == "Event") {								//Eventfeld
					//Eventkarte ziehen und ggf ausführen
				}
				else if (specialtile == "Community") {							//Gemeinschaftsfeld
					//Gemeinschaftskarte ziehen und ggf ausführen
				}
				else if (specialtile == "Hubschrauberlandeplatz") {				//Hubschrauberlandeplatz

					cout << "Du darfst dir ein beliebiges Feld aussuchen, zu dem du fliegen kannst. Das kostet aber 100 Euro." << endl;
					
					
					if (players[id].getMoney() >= 100) {					//genug Geld zum Fliegen
						cout << "Möchtest du fliegen? (nein=0 ja=1)" << endl;
						cin >> transport;

						if (transport) {											//moechte fliegen
							cout << "Wohin willst du? (Feld ID)" << endl;
							cin >> wunschfeldid;

							while (wunschfeldid < 0 || wunschfeldid > 39) {
								cout << "Dieses Feld existiert nicht" << endl;
							}
							players[id].addMoney(-100);
							players[id].setPosition(wunschfeldid);
						}
						else {												//moechte nicht fliegen
							cout << "ok dann bleibst du hier stehen" << endl;
						}
					}
					else {												//nicht genug Geld zum Fliegen
						cout << "Du hast aber nicht so viel, also bleibst du hier ;)" << endl;
					}						
				}
				else if (specialtile == "Bahnhof") {							//Bahnhof
					cout << "Du darfst dir einen beliebigen Bahnhof aussuchen, zu dem du fahren kannst. Das kostet aber 50 Euro." << endl;
					if (players[id].getMoney() >= 50) {					//genug Geld zum Fliegen
						cout << "Möchtest du fahren? (nein=0 ja=1)" << endl;
						cin >> transport;
						if (transport) {									//moechte fahren
							cout << "Wohin willst du? (Hauptbahnhof = 5 WestBahnhof = 15 OstBahnhof = 25)" << endl;
							cin >> wunschfeldid;

							while (wunschfeldid != 5 && wunschfeldid != 15 && wunschfeldid != 25) {
								cout << "Da darfst du nicht hin" << endl;
								players[id].addMoney(-50);
								players[id].setPosition(wunschfeldid);
							}
						}
						else {												//moechte nicht fahren
							cout << "ok dann bleibst du hier stehen" << endl;
						}
					}
					else {												//nicht genug Geld zum Fahren
						cout << "Du hast aber nicht so viel, also bleibst du hier ;)" << endl;
					}
					
				}


			}

		}
	
	} while (menumanager.getGameFunctionManager().getPaschCounter() == 1 || menumanager.getGameFunctionManager().getPaschCounter() == 2);		//nochmal würfeln wenn du Pasch hattest
	
	//Spielerzug zuende -> nächster Spieler

	if (id == 3) {
		id = 0;
	}
	else {
		id = id + 1;
	}	
cout << "Da wär jetzt InGameDialog" << endl;*/

}

