
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

/**
 * @brief Startet ein neues Spiel, initialisiert Spieler, Menüs und Einstellungen.
 * Fragt die Anzahl der Spieler ab, erstellt Spielerobjekte, setzt Startpositionen und startet das Spiel.
 */
void Server::SpielStarten() {

	MenuManager manager = MenuManager();
	setMenuManager(manager);
	getConfiguration().clearLog();	//Log leeren vor dem Spielstart
	cout << "Pfad zur Konfiguration: " << rulesPath << endl;				// Konfigurationsdatei einlesen
	//cin >> rulesPath; Configpfad selber eingeben

	Configuration config;

	while (!config.loadConfig()) {

		cout << "Pfad ungueltig \n \n Pfad zur Konfigurationsdatei: " << endl;
		cin >> rulesPath; //selber eingeben, falls ungueltig
	};
	cout << "Konfiguration wurde erfolgreich geladen: \n" << endl;
	//config.printSettings();

	settings = config.getSettings();
	

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
	cout << "Spieler gespeichert" << endl << endl;
	

	this_thread::sleep_for(chrono::milliseconds(2000));
	Spielzug(getMenuManager().getGameFunctionManager());	//Spiel starten
	//GefaengnisCheck(getMenuManager().getGameFunctionManager());
}

/**
 * @brief Lädt ein gespeichertes Spiel und stellt den Spielzustand wieder her.
 * Initialisiert das Menü, lädt den Spielstand und setzt die Spieler entsprechend.
 */
void Server::SpielLaden() {
	MenuManager manager = MenuManager();
	setMenuManager(manager);
	GameFunctionManager gamefunctionmanager = config.loadGame();
	getMenuManager().setGameFunctionManager(gamefunctionmanager) ;

	config.printLoadGame(gamefunctionmanager);	//Spieler und Runden anzeigen

	players = getMenuManager().getGameFunctionManager().getPlayers();
	//cout << "Position " << players[0].getName() << ": " << players[0].getPosition() << endl; //test
	this_thread::sleep_for(chrono::milliseconds(5000));
	getMenuManager().setInGame(true);
	getMenuManager().getGameFunctionManager().setPlayers(players);
	Spielzug(getMenuManager().getGameFunctionManager());	//Spiel starten
	//GefaengnisCheck(getMenuManager().getGameFunctionManager());				//Checken ob Spieler im Gefaengnis
	//getMenuManager().setCurrentMenu(getMenuManager().getMenus()[1]);
	
}

/**
 * @brief Zeigt den Ingame-Dialog an und ruft das Ingame-Menü auf.
 */
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

/**
 * @brief Führt den Spielzug für den aktuellen Spieler aus.
 * Prüft, ob der Spieler im Gefängnis ist, setzt das Spiel auf "InGame" und ruft das Ingame-Menü auf.
 * @param gamemanager Referenz auf den GameFunctionManager.
 */
void Server::Spielzug(GameFunctionManager& gamemanager) {
	cout << "InGame?" << getMenuManager().isInGame() << endl;
	GefaengnisCheck(gamemanager);				//Checken ob Spieler im Gefaengnis
	getMenuManager().setInGame(true);
	getMenuManager().setCurrentMenu(getMenuManager().getMenus()[1]);
	getMenuManager().handleMenus();
}

/**
 * @brief Führt den Würfelvorgang für den aktuellen Spieler aus.
 * Bewegt den Spieler, prüft Spezialfelder, behandelt Miete, Kauf und Game Over.
 * @param manager Referenz auf den GameFunctionManager.
 */
void Server::Wuerfeln(GameFunctionManager& manager) {

	int id = manager.getCurrentPlayer();
	//cout << "id:" << id << endl;

	do {																			
		dice = manager.rollDice();
		cout << "gewuerfelte Zahlen: " << dice[0] << ", " << dice[1] << endl;

		if (manager.getPaschCounter() == 3) {							//Wenn 3. Mal Pasch -> Gefaengnis

			cout << "Das war dein 3. Pasch... Gehe ins Gefaengnis" << endl;
			manager.getPlayers()[id].setPrisonCount(3);
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
			manager.showTileInfomation(tile);		//Feldeigenschaften anzeigen

			for (int i = 0; i < 4; i++) {
				if (i != id) {
					int positionofothers = manager.getPlayers()[i].getPosition();
					if (positionofothers == tile) {
						cout << manager.getPlayers()[i].getName()
							<< " befindet sich gerade schon auf diesem Feld. Gib 50 Euro ab!" << endl;
						if (manager.getPlayers()[id].getMoney() > 50) {
							manager.getPlayers()[id].addMoney(-50);
							manager.getPlayers()[i].addMoney(50);
						}
						else {
							cout << "Du hast aber leider nicht genug Geld dafuer... \nGame Over fuer dich :(";
							manager.getPlayers()[id].setGameOver();
						}
					}
				}
		}

		fuehreFeldAktionAus(manager, id, tile);
		cout << "Position von " << manager.getPlayers()[id].getName() << ": " << manager.getPlayers()[id].getPosition() << endl;
		}
			
	} while (manager.getPaschCounter() == 1 || manager.getPaschCounter() == 2);		//nochmal wuerfeln wenn du Pasch hattest

	//Spielerzug zuende -> naechster Spieler

	naechsterSpieler(manager);
}

/**
 * @brief Prüft, ob der aktuelle Spieler im Gefängnis ist, und behandelt die Freikauf- oder Paschlogik.
 * @param gamefunc Referenz auf den GameFunctionManager.
 */
void Server::GefaengnisCheck(GameFunctionManager& gamefunc) {
	int id = gamefunc.getCurrentPlayer();

	if (gamefunc.getPlayers()[id].getPrisonCount() > 0) {
		cout << gamefunc.getPlayers()[id].getName() <<"befindet sich leider im Gefaengnis." << endl;
		if (gamefunc.getPlayers()[id].getMoney() > 100) {			//wenn genug Geld zum Freikaufen da
			cout << "Moechtest du dich freikaufen(100 Euro) oder lieber versuchen, einen Pasch zu wuerfeln ? (dein budget : " << gamefunc.getPlayers()[id].getMoney() << ")" << " (0: wuerfeln, 1 : freikaufen)" << endl;
			cin >> freikaufen;
			if (freikaufen) {
				gamefunc.getPlayers()[id].addMoney(-100);
				gamefunc.getPlayers()[id].setPrisonCount(0);  // Gefaengnis-Zaehler zuruecksetzen
				cout << "Erfolgreich freigekauft, du kannst jetzt deinen Zug taetigen" << endl;
				this_thread::sleep_for(chrono::milliseconds(400));
				//Spielzug(gamefunc);
				return;
			}
			else {
				cout << "ok viel Glueck dann beim Pasch werfen ;)" << endl;
				Paschwerfen(gamefunc);
				return;
			}
		}
		else {
			cout << "Du hast leider nicht genug Geld, um dich freizukaufen :( Versuch dein Glück beim Paschwerfen!" << endl;
			Paschwerfen(gamefunc);
			return;
		}

	}
	//else {
	//	Spielzug(gamefunc);
	//	return;
	//}
}

/**
 * @brief Führt einen Paschwurf im Gefängnis durch und setzt ggf. den Spieler frei.
 * @param game Referenz auf den GameFunctionManager.
 */
void Server::Paschwerfen(GameFunctionManager& game){
	int id = game.getCurrentPlayer();
	dice = game.rollDice();

	if (dice[0] == dice[1]) {									//Wenn Pasch geworfen
		cout << "Du hast einen Pasch! Du kommst aus dem Gefaengnis frei." << endl;
		game.getPlayers()[id].setPrisonCount(0);  // Zaehler zuruecksetzen
		dice.clear();
		//Spielzug(game);		//Spielzug fortsetzen
		return;
	}
	else {
		game.getPlayers()[id].deductPrisonTime();
		if (game.getPlayers()[id].getPrisonCount() == 0) {		//wenn 3. Runde Gefaengnis
			cout << "Kein Pasch, aber du hast deine Strafe abgesessen. Du kommst naechstes Mal frei." << endl;
		}
		else {
			cout << "Kein Pasch. Du musst noch " << game.getPlayers()[id].getPrisonCount() << " Runden im Gefaengnis bleiben." << endl;
		}
		dice.clear();
		naechsterSpieler(game);
		return;
	}
}

/**
 * @brief Wechselt zum nächsten Spieler, prüft auf Game Over und startet ggf. eine neue Runde.
 * @param manager Referenz auf den GameFunctionManager.
 */
void Server::naechsterSpieler(GameFunctionManager& manager) {
	int id = manager.getCurrentPlayer();
	bool nochmal = false;
	int originalCurrentPlayer = manager.getCurrentPlayer();		//aktuellen Spieler merken
	getConfiguration().writeLog(manager);				//Log schreiben und Spielstand speichern
	
	manager.setCurrentPlayer(originalCurrentPlayer);		//auf ursprünglichen Spieler für Spiellogik zurücksetzen

	//Checken ob Game Over
	do {	
		switch (id) {
		case 0:
			manager.getPlayers()[1].getGameOver() ? id = 2 : id = 1;
			id == 2 ? nochmal = true : nochmal = false;	//wenn Spieler 2, dann nochmal abfragen ob Game Over
			break;
		case 1:
			manager.getPlayers()[2].getGameOver() ? id = 3 : id = 2;
			id == 3 ? nochmal = true : nochmal = false;
			break;
		case 2:
			manager.getPlayers()[3].getGameOver() ? id = 0 : id = 3;
			id == 0 ? manager.setCurrentRound(manager.getCurrentRound() + 1) : manager.setCurrentRound(manager.getCurrentRound());	//wenn Spieler 3, dann neue Runde
			id == 0 ? nochmal = true : nochmal = false;
			break;
		case 3:
			manager.getPlayers()[0].getGameOver() ? id = 1 : id = 0;
			manager.setCurrentRound(manager.getCurrentRound() + 1);
			id == 1 ? nochmal = true : nochmal = false;
			break;
		}
		int currentround = manager.getCurrentRound();
		if (currentround = settings.roundLimit) {
			Ende();
		}
		for (int i = 0; i < 4; i++) {									//checken wie viele Spieler Game Over sind
			int gameovercount = 0;
			if (manager.getPlayers()[i].getGameOver()) {
				gameovercount++;
			}
			if (gameovercount == 3) {									//Wenn es nur noch einen Spieler gibt
				for (int i = 0; i < 4; i++) {
					if (!manager.getPlayers()[i].getGameOver()) {
						cout << "Spieler " << manager.getPlayers()[i].getName() << " hat gewonnen!" << endl; //Herausfinden, welcher Spieler gewonnen hat und Spiel beenden
						getConfiguration().sammlungHighscore(manager.getPlayers());
						Ende();
						return;
					}
				}
			}
		}

		manager.setCurrentPlayer(id);
	} while (nochmal);

	cout << "naechster Spieler" << endl;
	this_thread::sleep_for(chrono::milliseconds(2000));
	GefaengnisCheck(manager);									//checken, ob gerade im Gefaengnis
}

/**
 * @brief Gibt eine Referenz auf den aktuellen MenuManager zurück.
 * @return Referenz auf MenuManager.
 */
MenuManager& Server::getMenuManager() { return *menumanager; }

/**
 * @brief Setzt den MenuManager.
 * @param manager Referenz auf MenuManager.
 */
void Server::setMenuManager(MenuManager& manager) { this->menumanager = &manager; }

/**
 * @brief Gibt die aktuelle Konfiguration zurück.
 * @return Konfigurationsobjekt.
 */
Configuration Server::getConfiguration() { return config; }

/**
 * @brief Beendet das Spiel nach Rundenlimiterreichung oder 'last one standing'.
 */
void Server::Ende() {
	cout << "Das Spiel ist zu Ende. Hoffe ihr hattet Spaß :)" << endl;
	exit(0);
}

/**
* @brief Aufruf aus 'Spielstand Speichern'. Beendet das Spiel.
*/
void Server::SpielstandSpeichern() {
	config.saveGame();
	cout << "Das Spiel wurde gespeichert. Bis zum naechsten Mal :)" << endl;	//Spielstände sind bereits gespeichert worden nach jedem Zug

	exit(0);
}

/*
Funktionsdefinitionen für die Möglichkeitne von Mietfrei
*/
void Server::addToPot(int betrag) {
	pot += betrag;
}

int Server::getPot() const {
	return pot;
}

void Server::clearPot() {
	pot = 0;
}


void Server::fuehreFeldAktionAus(GameFunctionManager& manager, int id, int tile) {			//// ALLE if/else zu PropertyTile, SpecialTile, Action-Switch-Case, Bahnhöfe etc.
	auto& sTile = manager.getMap().tiles[tile];

	if (auto propTile = dynamic_cast<PropertyTile*>(sTile.get())) {		//auf Property Tile

		price = propTile->getPrice();

		if (propTile->getOwnerId() == -1) {											//wenn Feld noch nicht gekauft
			if (manager.getPlayers()[id].getMoney() > price) {									//wenn genug Geld zum Kaufen da
				cout << "Moechtest du das Feld kaufen?(dein Budget:" << manager.getPlayers()[id].getMoney() << ") (0: nein, 1:ja)" << endl;
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
				cout << "Du hast leider nicht genug Geld, um dieses Feld zu kaufen. Weiter geht's" << endl;	//nicht genug Geld zum Kaufen da
			}
		}
		else if (propTile->getOwnerId() == id) {					//bist du inhaber?
			if (manager.getMap().canUpgradeStreet(propTile, id)) {
				cout << "Möchtest du auf dieser Straße ein Haus bauen? (Kosten: " << propTile->getHouseCost() << " Euro) (0: nein, 1: ja)" << endl;
				int bauen = 0;
				cin >> bauen;
				if (bauen && manager.getPlayers()[id].getMoney() >= propTile->getHouseCost()) {		//darfst du hier bauen?
					propTile->setBuildingLevel(propTile->getBuildingLevel() + 1);
					manager.getPlayers()[id].addMoney(-propTile->getHouseCost());
					cout << "Herzlichen Glückwunsch! Du hast ein Haus gebaut. Die Miete steigt!" << endl;
				}
				else if (bauen) {
					cout << "Du hast leider nicht genug Geld für ein Haus." << endl;
				}
			}
			else {
				cout << "Du kannst hier aktuell kein Haus bauen. Baue zuerst auf den anderen Straßen der Farbgruppe gleichmäßig!" << endl;
			}
		}
		else {																			//Miete zahlen, wenn vergeben
			rent = propTile->getRent();
			ownerid = propTile->getOwnerId();

			cout << "Das Fed besitzt " << ownerid << ". Du musst " << rent << " Euro Miete zahlen." << endl;

			if (manager.getPlayers()[id].getMoney() > rent) {					//genug Geld fuer Miete

				manager.getPlayers()[id].addMoney(-rent);
				manager.getPlayers()[ownerid].addMoney(rent);
			}
			else {													//nicht genug Geld fuer Miete
				cout << "Du hast aber leider nicht genug Geld dafuer... \nGame Over fuer dich :(";
				//Game Over
				manager.getPlayers()[id].setGameOver();
			}
		}

	}

	else if (auto specialTile = dynamic_cast<const SpecialTile*>(sTile.get())) {	//auf Special Tile

		specialtile = specialTile->getTypeString();
		if (specialtile == "Start") {
			manager.getPlayers()[id].addMoney(400);									//Wenn man direkt auf Los landet bekommt man 400 Euro
		}
		else if (specialtile == "Jail") {
			cout << "Du bist nur zu Besuch hier" << endl;
		}
		else if (specialtile == "GoToJail") {							//Ins Gefaengnis gehen
			cout << "Gehe ins Gefaengnis!" << endl;
			manager.getPlayers()[id].setPrisonCount(3);
			manager.getPlayers()[id].setPosition(10);
		}
		else if (specialtile == "FreeParking") {						//Frei Parken
			int potGewinn = getPot();
			if (potGewinn > 0) {
				manager.getPlayers()[id].addMoney(potGewinn);
				clearPot();
				cout << "Glückwunsch! Du bekommst " << potGewinn << "Euro vom Freiparken-Pot!" << endl;
			}
			else {
				cout << "Hier kannst du chillen und frei parken :)" << endl;
			}
		}
		else if (specialtile == "Tax") {								//Einkommenssteuer zahlen
			cout << "Zahle 200 Euro Einkommenssteuer!" << endl;
			if (manager.getPlayers()[id].getMoney() > 100) {					//genug Geld

				manager.getPlayers()[id].addMoney(-200);
			}
			else {													//nicht genug Geld
				cout << "Du hast aber leider nicht genug Geld dafuer... \nGame Over fuer dich :(";
				//Game Over
				manager.getPlayers()[id].setGameOver();
			}
		}
		else if (specialtile == "LuxuryTax") {							//Zusatzsteuer zahlen

			cout << "Zahle 100 Euro Zusatzsteuer!" << endl;
			if (manager.getPlayers()[id].getMoney() > 200) {					//genug Geld

				manager.getPlayers()[id].addMoney(-200);
			}
			else {													//nicht genug Geld
				cout << "Du hast aber leider nicht genug Geld dafuer... \nGame Over fuer dich :(";
				//Game Over
				manager.getPlayers()[id].setGameOver();
			}
		}
		else if (specialtile == "Hubschrauberlandeplatz") {				//Hubschrauberlandeplatz

			cout << "Du darfst dir ein beliebiges Feld aussuchen, zu dem du fliegen kannst. Das kostet aber 100 Euro." << endl;


			if (manager.getPlayers()[id].getMoney() > 100) {					//genug Geld zum Fliegen
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
			if (manager.getPlayers()[id].getMoney() > 50) {					//genug Geld zum Fliegen
				cout << "Moechtest du fahren? (nein=0 ja=1)" << endl;
				cin >> transport;
				if (transport) {									//moechte fahren
					cout << "Wohin willst du? (Hauptbahnhof = 5 WestBahnhof = 15 OstBahnhof = 25)" << endl;
					cin >> wunschfeldid;

					while (wunschfeldid != 5 && wunschfeldid != 15 && wunschfeldid != 25) {
						cout << "Da darfst du nicht hin" << endl;
					}
					manager.getPlayers()[id].addMoney(-50);
					manager.getPlayers()[id].setPosition(wunschfeldid);
				}
				else {												//moechte nicht fahren
					cout << "ok dann bleibst du hier stehen" << endl;
				}
			}
			else {												//nicht genug Geld zum Fahren
				cout << "Du hast aber nicht so viel, also bleibst du hier ;)" << endl;
			}
		}

		else if (specialtile == "Action") {
			cout << "Du bist auf einem Aktionsfeld gelandet! Hoffentlich bekommst du was gutes:" << endl;
			int actionCard = ((manager.randomNumber() - 1) * 6 + (manager.randomNumber() - 1)) % 12 + 1;
			this_thread::sleep_for(chrono::milliseconds(2000));
			//berechnet random number zwischen 1 und 12 anhand einer funktion die nur zwischen 1 und 6 berechnet
			switch (actionCard) {
			case 1:
				std::cout << "Die Parkgebuehren steigen – zahle 50 Euro in den Freipark-Topf!" << std::endl;
				manager.getPlayers()[id].addMoney(-50);
				addToPot(50);
				this_thread::sleep_for(chrono::milliseconds(1000));
				break;
			case 2: {
				// Aktueller Spieler id
				int numPlayers = manager.getPlayers().size();
				int nextId = id;
				bool found = false;
				for (int i = 1; i < numPlayers; ++i) {					//finde nächsten lebendigen Spieler (kandidaten)
					int candidate = (id + i) % numPlayers;
					if (!manager.getPlayers()[candidate].getGameOver()) {
						nextId = candidate;
						found = true;
						break;
					}
				}
				if (!found) {
					std::cout << "Alle anderen sind Game Over! Du behaeltst deine 50 Euro. Warum spielst du eigentlich noch?" << std::endl;
				}
				else {
					manager.getPlayers()[id].addMoney(-50);
					manager.getPlayers()[nextId].addMoney(50);
					std::cout << "Großzuegigkeit tut gut: Du gibst Spieler '"
						<< manager.getPlayers()[nextId].getName()
						<< "' 50 Euro!" << std::endl;
				}
				this_thread::sleep_for(chrono::milliseconds(1000));
				break;
			}

			case 3:
				manager.getPlayers()[id].addMoney(50);
				std::cout << "Die Bank hat dich lieb heute – du bekommst 50 Euro geschenkt!" << std::endl;
				this_thread::sleep_for(chrono::milliseconds(1000));
				break;
			case 4:
				std::cout << "Der Wuerfelgott schubst dich voran: Ziehe 3 Felder nach vorne." << std::endl;
				manager.getPlayers()[id].setPosition(
					(manager.getPlayers()[id].getPosition() + 3) % 40);
				this_thread::sleep_for(chrono::milliseconds(1000));
				fuehreFeldAktionAus(manager, id, manager.getPlayers()[id].getPosition());
				break;
			case 5:
				std::cout << "Oh je! Ein Rueckschlag. Gehe 3 Felder zurueck (aber nicht den Kopf haengen lassen)." << std::endl;
				manager.getPlayers()[id].setPosition((manager.getPlayers()[id].getPosition() - 3 + 40) % 40);		//aufpassen wegen negativ !
				this_thread::sleep_for(chrono::milliseconds(1000));
				fuehreFeldAktionAus(manager, id, manager.getPlayers()[id].getPosition());
				break;
			case 6:
				std::cout << "Ups... Direkt ins Gefaengnis! Gehe nicht über Los, ziehe keine 200 Euro ein. Gar nicht gut" << std::endl;
				manager.getPlayers()[id].setPosition(10);
				manager.getPlayers()[id].setPrisonCount(3);
				this_thread::sleep_for(chrono::milliseconds(1000));
				break;
			case 7:
				std::cout << "Du Glueckspilz! Du findest 35 Euro auf der Straße!" << std::endl;
				manager.getPlayers()[id].addMoney(35);
				this_thread::sleep_for(chrono::milliseconds(1000));
				break;
			case 8:
				std::cout << "Die Sterne stehen richtig! Du bekommst 65 Euro." << std::endl;
				manager.getPlayers()[id].addMoney(65);
				this_thread::sleep_for(chrono::milliseconds(1000));
				break;
			case 9:
				std::cout << "Ab auf die Schlosspromenade! Goenn dir den besten Stadtteil (hoffentlich)." << std::endl;
				manager.getPlayers()[id].setPosition(39);
				this_thread::sleep_for(chrono::milliseconds(1000));
				fuehreFeldAktionAus(manager, id, 39);
				break;
			case 10: {
				// Kandidaten sammeln: Alle aktiven Spieler außer dir selbst
				std::vector<int> kandidaten;
				for (int i = 0; i < manager.getPlayers().size(); ++i) {
					if (i != id && !manager.getPlayers()[i].getGameOver())
						kandidaten.push_back(i);
				}
				// Gibt es überhaupt einen Kandidaten?
				if (kandidaten.empty()) {
					std::cout << "Alle anderen sind Game Over! Niemand zum Tauschen da. Hier ne gute Idee: Such dir Leute mit denen du spielen kannst!" << std::endl;
				}
				else {
					// Einen zufälligen Kandidaten auswählen (1-6 randomnumber, modulo Kandidatenzahl)
					int randomIndex = (manager.randomNumber() - 1) % kandidaten.size();
					int tauschPartner = kandidaten[randomIndex];

					// Positionen tauschen
					int tmp = manager.getPlayers()[id].getPosition();
					manager.getPlayers()[id].setPosition(manager.getPlayers()[tauschPartner].getPosition());
					manager.getPlayers()[tauschPartner].setPosition(tmp);

					std::cout << "Verwirrung! Du tauschst die Position mit Spieler '"
						<< manager.getPlayers()[tauschPartner].getName() << "'." << std::endl;
				}
				this_thread::sleep_for(chrono::milliseconds(1000));
				break;
			}
			case 11:
				std::cout << "Alarm! SOFORT auf LOS – mach schnell, bevor jemand merkt, dass du gleich 400 Euro kassierst!" << std::endl;
				manager.getPlayers()[id].setPosition(0);
				this_thread::sleep_for(chrono::milliseconds(1000));
				break;
			case 12:
				manager.getPlayers()[id].setPosition(20);
				if (getPot() > 0) {
					int gewinn = getPot();
					manager.getPlayers()[id].addMoney(gewinn);
					clearPot();
					std::cout << "Du bekommst " << gewinn << " Euro aus dem Freiparken-Pot!" << std::endl;
				}
				else {
					std::cout << "Leider ist der FreiParken Pot noch leer. Vielleicht beim nächsten Mal!" << std::endl;
				}
				this_thread::sleep_for(chrono::milliseconds(1000));
				break;
			}

		}




	}

}

