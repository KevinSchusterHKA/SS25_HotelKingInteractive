
#include <conio.h>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>
#include <stdlib.h>
#include <vector>
#include <thread>
#include <chrono>
#include <random>
#include <ctime>

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

	MenuManager manager = MenuManager();
	setMenuManager(manager);

	cout << "Pfad zur Konfiguration: " << rulesPath << endl;				// Konfigurationsdatei einlesen
	//cin >> rulesPath; Configpfad selber eingeben

	Configuration config;

	while (!config.loadConfig(rulesPath)) {

		cout << "Pfad ungueltig \n \n Pfad zur Konfigurationsdatei: " << endl;
		cin >> rulesPath; //selber eingeben, falls ungueltig
	};
	cout << "Konfiguration wurde erfolgreich geladen: \n" << endl;
	//config.printSettings();

	GameSettings settings = config.getSettings();
	

	cout << "Wie viele Spieler spielen? (1-4) " << endl;					//Anzahl der Spieler abfragen
	cin >> numberOfPlayer;

	while (numberOfPlayer != 1 && numberOfPlayer != 2 && numberOfPlayer != 3 && numberOfPlayer != 4) {

		cout << "ungueltige Eingabe \n \n Wie viele Spieler spielen? (1-4) " << endl;		//Anzahl der Spieler nochmal abfragen wenn ungueltig
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
			
		//getMenuManager().getGameFunctionManager().addPlayer(realPlayer);
		getMenuManager().getGameFunctionManager().getPlayers().push_back(realPlayer);
		//Spieler mit Namen, Anfangsbudget und ID erstellen
	
		
		//players.push_back(realPlayer);
		//return players;

	}
	numberOfCPUPlayer = 4 - numberOfPlayer;
	//Player anzahlCPU = CPU(numberOfCPUPlayer); // Funktion in Player fuer CPU Gegner mit uebergabeparameter Anzahl CPU Gegner
	cout << "Anzahl CPU Gegner: " << numberOfCPUPlayer << endl;

	
	for (int i = 0; i < numberOfCPUPlayer; i++) {						// CPU Spieler Namen und ID erstelen

		playerName = "CPU Spieler " + to_string(i);
		

		Player cpuPlayer(playerName, settings.startBudget, 4 - numberOfCPUPlayer + i);

		//players.push_back(cpuPlayer);
		
		getMenuManager().getGameFunctionManager().getPlayers().push_back(cpuPlayer);

		//return players;
	}
	


	cout << "Spiel wird gestartet\n" << endl;

	players = getMenuManager().getGameFunctionManager().getPlayers();
	players[0].setPosition(0);
	players[1].setPosition(0);									//alle Spieler auf LOS
	players[2].setPosition(0);
	players[3].setPosition(0);

	cout << "Alle Spieler werden auf LOS gesetzt" << endl;
	//cout << "Position " << players[0].getName() << ": " << players[0].getPosition() << endl; //test

	//cout << "TEST" << endl;
	
	getMenuManager().getGameFunctionManager().setCurrentPlayer(0);	//1. Spieler beginnt
	getMenuManager().getGameFunctionManager().setCurrentRound(1);	//1. Runde

	//getMenuManager().getMenulog() << getMenuManager().getGameFunctionManager().getCurrentRound() << "ds" << endl;

	cout << "Current Player"<< getMenuManager().getGameFunctionManager().getCurrentPlayer() << endl;
	//cout << "Current Player: " << getMenuManager().getGameFunctionManager().getCurrentPlayer() << " Current Round: " << getMenuManager().getGameFunctionManager().getCurrentRound() << endl; //test
	//cout << players[0].getName() << " beginnt" << endl;

	GameFunctionManager info = getMenuManager().getGameFunctionManager();			//log erstellen und Spieler speichern
	info.setCurrentPlayer(0);
	config.writeLog(info);
	info.setCurrentPlayer(1);
	config.writeLog(info);
	info.setCurrentPlayer(2);
	config.writeLog(info);
	info.setCurrentPlayer(3);
	config.writeLog(info);
	config.saveGame("game.log", "savetest.txt", 4);
	cout << "Spieler gespeichert" << endl << endl;
	

	this_thread::sleep_for(chrono::milliseconds(2000));
	Spielzug(getMenuManager().getGameFunctionManager());
}


void Server::SpielLaden() {
	MenuManager manager = MenuManager();
	setMenuManager(manager);
	Configuration config;
	GameFunctionManager gamefunctionmanager = config.loadGame("savetest.txt");
	getMenuManager().setGameFunctionManager(gamefunctionmanager) ;

	cout << "Aktuelle Runde: " << getMenuManager().getGameFunctionManager().getCurrentRound() << endl;
	cout << "Aktueller Spieler: " << getMenuManager().getGameFunctionManager().getCurrentPlayer() << endl;

	for (const Player& p : getMenuManager().getGameFunctionManager().getPlayers()) {
		cout << "-----------------------------" << endl;
		cout << "Name: " << p.getName() << endl;
		cout << "ID: " << p.getID() << endl;
		cout << "Budget: " << p.getMoney() << endl;
		cout << "Position: " << p.getPosition() << endl;
		cout << "Im Gefaengnis? " << (p.inPrison() ? "Ja" : "Nein") << endl;
		cout << "Gefaengnis-Runden: " << p.getPrisonCount() << endl;
		cout << endl;
	}
	players = getMenuManager().getGameFunctionManager().getPlayers();
	//cout << "Position " << players[0].getName() << ": " << players[0].getPosition() << endl; //test
	this_thread::sleep_for(chrono::milliseconds(5000));
	getMenuManager().setInGame(true);
	getMenuManager().getGameFunctionManager().setPlayers(players);
	getMenuManager().setCurrentMenu(getMenuManager().getMenus()[1]);
	GefaengnisCheck(getMenuManager().getGameFunctionManager());	//Checken ob Spieler im Gefaengnis
	
}
void Server::showIngameDialog() {
	/*
	cout << "Player 1: " << players[0].getName() << "Position " << players[0].getPosition() << endl;
	cout << "Player 2: " << players[1].getName() << "Position " << players[1].getPosition() << endl;
	cout << "Player 3: " << players[2].getName() << "Position " << players[2].getPosition() << endl;
	cout << "Player 4: " << players[3].getName() << "Position " << players[3].getPosition() << endl;
	*/
	
	
	this_thread::sleep_for(chrono::milliseconds(3000));
	getMenuManager().setCurrentMenu(getMenuManager().getMenus()[1]);
	getMenuManager().handleMenus();									//InGameMenu wird aufgerufen
	

}

void Server::Spielzug(GameFunctionManager& gamemanager) {
	cout << "InGame?" << getMenuManager().isInGame() << endl;
	//getMenuManager().setInGame(true);
	getMenuManager().setCurrentMenu(getMenuManager().getMenus()[1]);
	getMenuManager().handleMenus();
}
void Server::Wuerfeln(GameFunctionManager& manager) {
	//players= manager.getPlayers();
	//cout << "Player 3" << players[2].getName() << endl;
	int id = manager.getCurrentPlayer();
	//cout << "id:" << id << endl;

	//Spielzug
	do {																			
		dice = manager.rollDice();
		cout << "gewuerfelte Zahlen: " << dice[0] << ", " << dice[1] << endl;

		if (manager.getPaschCounter() == 3) {							//Wenn 3. Mal Pasch -> Gefaengnis

			cout << "Das war dein 3. Pasch... Gehe ins Gefaengnis" << endl;
			manager.getPlayers()[id].inPrison();
			manager.getPlayers()[id].setPosition(10);
		}
		else {
			manager.getPlayers()[id].move(dice[0] + dice[1]); //move Summe aus gewuerfelten Zahlen
			dice.clear();
			int tile = manager.getPlayers()[id].getPosition();

			if (tile >= 40) {										//Checken ob ueber LOS gegangen
				tile = tile - 40;
				manager.getPlayers()[id].setPosition(tile);
				manager.getPlayers()[id].addMoney(200);
				if (tile == 0) {
					manager.getPlayers()[id].addMoney(200);			//Checken ob genau auf LOS
				}
			}
			cout << "Position von " << manager.getPlayers()[id].getName() << ": " << manager.getPlayers()[id].getPosition() << endl;
				
			manager.showTileInfomation(tile);								//Feldeigenschaften anzeigen

			for (int i = 0; i < 4; i++) {												//checken, ob schon jemand auf dem Feld steht
				if (i != id){
					positionofothers = manager.getPlayers()[i].getPosition();
					if (positionofothers == tile) {								//dieser Player steht schon auf dem Feld
						cout << manager.getPlayers()[i].getName() << "befindet sich gerade schon auf diesem Feld. Gib 50 Euro ab!" << endl;
						if (manager.getPlayers()[id].getMoney() >= 50) {
							manager.getPlayers()[id].addMoney(-50);
							manager.getPlayers()[i].addMoney(50);
						}
						else {													//nicht genug Geld
							cout << "Du hast aber leider nicht genug Geld dafuer... \nGame Over fuer dich :(";
							//Game Over
						}
					}
				}
			}

			auto& sTile = manager.getMap().tiles[tile];

			if (auto propTile = dynamic_cast<PropertyTile*>(sTile.get())) {		//auf Property Tile

				price = propTile->getPrice();

				if (propTile->getOwnerId() == -1) {											//wenn Feld noch nicht gekauft
					if (manager.getPlayers()[id].getMoney() >= price) {									//wenn genug Geld zum Kaufen da
						cout << "Moechtest du das Feld kaufen?(dein Budget:"<<manager.getPlayers()[id].getMoney()<<") (0: nein, 1:ja)" << endl;
						cin >> buyfield;
						if (buyfield) {
							manager.getPlayers()[id].addMoney(-price);
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

					if (manager.getPlayers()[id].getMoney() >= rent) {					//genug Geld fuer Rente

						manager.getPlayers()[id].addMoney(-rent);
						manager.getPlayers()[ownerid].addMoney(rent);
					}
					else {													//nicht genug Geld fuer Rente
						cout << "Du hast aber leider nicht genug Geld dafuer... \nGame Over fuer dich :(";
							//Game Over
					}
				}
			}
			else if (auto specialTile = dynamic_cast<const SpecialTile*>(sTile.get())) {	//auf Special Tile

				specialtile = specialTile->getTypeString();
				if(specialtile == "Start") {
					manager.getPlayers()[id].addMoney(400);									//Wenn man direkt auf Los landet bekommt man 400 Euro
				}
				else if (specialtile == "Jail") {
					cout << "Du bist nur zu Besuch hier" << endl;
				}
				else if (specialtile == "GoToJail") {							//Ins Gefaengnis gehen
					cout << "Gehe ins Gefaengnis!" << endl;
					manager.getPlayers()[id].inPrison();
					manager.getPlayers()[id].setPosition(10);
				}
				else if (specialtile == "FreeParking") {						//Frei Parken
					cout << "Hier kannst du chillen und frei parken :)" << endl;
				}
				else if (specialtile == "Tax") {								//Einkommenssteuer zahlen
					cout << "Zahle 200 Euro Einkommenssteuer!" << endl;
					if (manager.getPlayers()[id].getMoney() >= 100) {					//genug Geld

						manager.getPlayers()[id].addMoney(-200);
					}
					else {													//nicht genug Geld
						cout << "Du hast aber leider nicht genug Geld dafuer... \nGame Over fuer dich :(";
						//Game Over
					}
				}
				else if (specialtile == "LuxuryTax") {							//Zusatzsteuer zahlen

					cout << "Zahle 100 Euro Zusatzsteuer!" << endl;
					if (manager.getPlayers()[id].getMoney() >= 200) {					//genug Geld

						manager.getPlayers()[id].addMoney(-200);
					}
					else {													//nicht genug Geld
						cout << "Du hast aber leider nicht genug Geld dafuer... \nGame Over fuer dich :(";
						//Game Over
					}
				}
				else if (specialtile == "Event") {								//Eventfeld
					//Eventkarte ziehen und ggf ausfuehren
				}
				else if (specialtile == "Community") {							//Gemeinschaftsfeld
					//Gemeinschaftskarte ziehen und ggf ausfuehren
				}
				else if (specialtile == "Hubschrauberlandeplatz") {				//Hubschrauberlandeplatz

					cout << "Du darfst dir ein beliebiges Feld aussuchen, zu dem du fliegen kannst. Das kostet aber 100 Euro." << endl;


					if (manager.getPlayers()[id].getMoney() >= 100) {					//genug Geld zum Fliegen
						cout << "Moechtest du fliegen? (nein=0 ja=1)" << endl;
						cin >> transport;

						if (transport) {											//moechte fliegen
							cout << "Wohin willst du? (Feld ID)" << endl;
							cin >> wunschfeldid;

							while (wunschfeldid < 0 || wunschfeldid > 39) {
								cout << "Dieses Feld existiert nicht" << endl;
							}
							manager.getPlayers()[id].addMoney(-100);
							manager.getPlayers()[id].setPosition(wunschfeldid);
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
					if (manager.getPlayers()[id].getMoney() >= 50) {					//genug Geld zum Fliegen
						cout << "Moechtest du fahren? (nein=0 ja=1)" << endl;
						cin >> transport;
						if (transport) {									//moechte fahren
							cout << "Wohin willst du? (Hauptbahnhof = 5 WestBahnhof = 15 OstBahnhof = 25)" << endl;
							cin >> wunschfeldid;

							while (wunschfeldid != 5 && wunschfeldid != 15 && wunschfeldid != 25) {
								cout << "Da darfst du nicht hin" << endl;
								manager.getPlayers()[id].addMoney(-50);
								manager.getPlayers()[id].setPosition(wunschfeldid);
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

	} while (manager.getPaschCounter() == 1 || manager.getPaschCounter() == 2);		//nochmal wuerfeln wenn du Pasch hattest

	//Spielerzug zuende -> naechster Spieler

	naechsterSpieler(manager);
}
void Server::GefaengnisCheck(GameFunctionManager& gamefunc) {
	int id = gamefunc.getCurrentPlayer();

	if (gamefunc.getPlayers()[id].getPrisonCount() > 0) {
		cout << gamefunc.getPlayers()[id].getName() <<"befindet sich leider im Gefaengnis." << endl;
		if (gamefunc.getPlayers()[id].getMoney() >= 100) {			//wenn genug Geld zum Freikaufen da
			cout << "Moechtest du dich freikaufen(100 Euro) oder lieber versuchen, einen Pasch zu wuerfeln ? (dein budget : " << gamefunc.getPlayers()[id].getMoney() << ")" << " (0: wuerfeln, 1 : freikaufen)" << endl;
			cin >> freikaufen;
			if (freikaufen) {
				gamefunc.getPlayers()[id].addMoney(-100);
				gamefunc.getPlayers()[id].setPrisonCount(0);  // Gefaengnis-Zaehler zuruecksetzen
				cout << "Erfolgreich freigekauft, du kannst jetzt deinen Zug taetigen" << endl;
				this_thread::sleep_for(chrono::milliseconds(400));
				Spielzug(gamefunc);
			}
			else {
				cout << "ok viel Glueck dann beim Pasch werfen ;)" << endl;
				Paschwerfen(gamefunc);
			}
		}
		else {
			cout << "Du hast leider nicht genug Geld, um dich freizukaufen :( Versuch dein Glück beim Paschwerfen!" << endl;
			Paschwerfen(gamefunc);
		}

	}
	else {
		Spielzug(gamefunc);
	}
}
void Server::Paschwerfen(GameFunctionManager& game){
	int id = game.getCurrentPlayer();
	dice = game.rollDice();

	if (dice[0] == dice[1]) {									//Wenn Pasch geworfen
		cout << "Du hast einen Pasch! Du kommst aus dem Gefaengnis frei." << endl;
		game.getPlayers()[id].setPrisonCount(0);  // Zaehler zuruecksetzen
		Spielzug(game);		//Spielzug fortsetzen
	}
	else {
		game.getPlayers()[id].deductPrisonTime();
		if (game.getPlayers()[id].getPrisonCount() == 0) {		//wenn 3. Runde Gefaengnis
			cout << "Kein Pasch, aber du hast deine Strafe abgesessen. Du kommst nächstes Mal frei." << endl;
			naechsterSpieler(game);
		}
		else {
			cout << "Kein Pasch. Du musst noch " << game.getPlayers()[id].getPrisonCount() << " Runden im Gefaengnis bleiben." << endl;
		}
		naechsterSpieler(game);
	}
	dice.clear();
}

void Server::naechsterSpieler(GameFunctionManager& manager) {
	int id = manager.getCurrentPlayer();
	if (id == 3) {													//naechster Spieler
		id = 0;
		manager.setCurrentPlayer(id);
		manager.setCurrentRound(manager.getCurrentRound() + 1);		//neue Runde
	}
	else {
		id = id + 1;
		manager.setCurrentPlayer(id);
	}
	cout << "naechster Spieler" << endl;
	this_thread::sleep_for(chrono::milliseconds(400));
	GefaengnisCheck(manager);									//checken, ob gerade im Gefängnis
}
MenuManager& Server::getMenuManager() { return *menumanager; }
void Server::setMenuManager(MenuManager& manager) { this->menumanager = &manager; }