
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
#include <algorithm> // Für std::transform

#include "Server.hpp"
#include "Player.hpp"
#include "Configuration.h"
#include "GameFunctionManager.hpp"
#include "MenuManager.hpp"
#include "Menu.hpp"
#include "PropertyTile.hpp"
#include "SpecialTile.hpp"


using namespace std;

Server::Server(vector<Player>& playersRef)
	: players(playersRef) {
}
std::string normalizeString(const std::string& s);		//tauscht scharfe s und umlaute für Handeln


/**
 * @brief Startet ein neues Spiel, initialisiert Spieler, Menues und Einstellungen.
 * Fragt die Anzahl der Spieler ab, erstellt Spielerobjekte, setzt Startpositionen und startet das Spiel.
 */
void Server::SpielStarten() {

	MenuManager manager = MenuManager();
	setMenuManager(manager);
	config.clearLog();	//Log leeren vor dem Spielstart
	cout << "Pfad zur Konfiguration: " << rulesPath << endl;				// Konfigurationsdatei einlesen
	config.loadConfig();
	while (!config.loadConfig()) {

		cout << "Pfad ungueltig \n \n Pfad zur Konfigurationsdatei: " << endl;
		cin >> rulesPath; //selber eingeben, falls ungueltig
	};
	cout << "Konfiguration wurde erfolgreich geladen: \n" << endl;
	settings = config.getSettings();
	config.printSettings();	//Einstellungen anzeigen
	
	

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

		Player realPlayer(playerName, settings.startBudget, i);		//Spieler mit Namen, Anfangsbudget und ID erstellen
		getMenuManager().getGameFunctionManager().getPlayers().push_back(realPlayer);
		
	}
	numberOfCPUPlayer = 4 - numberOfPlayer;
	cout << "Anzahl CPU Gegner: " << numberOfCPUPlayer << endl;

	
	for (int i = 0; i < numberOfCPUPlayer; i++) {						// CPU Spieler Namen und ID erstelen

		playerName = "CPU Spieler " + to_string(i);
		

		Player cpuPlayer(playerName, settings.startBudget, 4 - numberOfCPUPlayer + i);
		
		getMenuManager().getGameFunctionManager().getPlayers().push_back(cpuPlayer);

	}
	


	cout << "Spiel wird gestartet\n" << endl;

	players = getMenuManager().getGameFunctionManager().getPlayers();
	players[0].setPosition(0);
	players[1].setPosition(0);									//alle Spieler auf LOS
	players[2].setPosition(0);
	players[3].setPosition(0);

	cout << "Alle Spieler werden auf LOS gesetzt" << endl;
	getMenuManager().getGameFunctionManager().setCurrentPlayer(0);	//1. Spieler beginnt
	getMenuManager().getGameFunctionManager().setCurrentRound(1);	//1. Runde

	cout << "Current Player"<< getMenuManager().getGameFunctionManager().getCurrentPlayer() << endl;
	pot = 0;
	GameFunctionManager info = getMenuManager().getGameFunctionManager();			//log erstellen und Spieler speichern
	info.setCurrentPlayer(0);
	config.writeLog(info);
	info.setCurrentPlayer(1);
	config.writeLog(info);
	info.setCurrentPlayer(2);
	config.writeLog(info);
	info.setCurrentPlayer(3);
	config.writeLog(info);
	cout << "Spieler gesetzt" << endl << endl;
	

	this_thread::sleep_for(chrono::milliseconds(2000));
	Spielzug(getMenuManager().getGameFunctionManager());	
}

/**
 * @brief Laedt ein gespeichertes Spiel und stellt den Spielzustand wieder her.
 * Initialisiert das Menue, laedt den Spielstand und setzt die Spieler entsprechend.
 */
void Server::SpielLaden() {
	MenuManager manager = MenuManager();
	setMenuManager(manager);
	GameFunctionManager gamefunctionmanager = config.loadGame();
	getMenuManager().setGameFunctionManager(gamefunctionmanager) ;

	config.printLoadGame(gamefunctionmanager);	//Spieler und Runden anzeigen
	config.loadConfig();
	settings = config.getSettings();
	//config.printSettings();	//Einstellungen anzeigen

	players = getMenuManager().getGameFunctionManager().getPlayers();
	this_thread::sleep_for(chrono::milliseconds(5000));
	getMenuManager().setInGame(true);
	getMenuManager().getGameFunctionManager().setPlayers(players);
	Spielzug(getMenuManager().getGameFunctionManager());	//Spiel starten
	}

/**
 * @brief Fuehrt den Spielzug fuer den aktuellen Spieler aus.
 * Prueft, ob der Spieler im Gefaengnis ist, setzt das Spiel auf "InGame" und ruft das Ingame-Menue auf.
 * @param gamemanager Referenz auf den GameFunctionManager.
 */
void Server::Spielzug(GameFunctionManager& gamemanager) {
	GefaengnisCheck(gamemanager);				//Checken ob Spieler im Gefaengnis
	getMenuManager().setInGame(true);
	getMenuManager().setCurrentMenu(getMenuManager().getMenus()[1]);
	getMenuManager().handleMenus();
}

/**
 * @brief Fuehrt den Wuerfelvorgang fuer den aktuellen Spieler aus.
 * Bewegt den Spieler, checkt Pasch, führt Feldaktionen aus und wechselt zum naechsten Spieler.
 * @param manager Referenz auf den GameFunctionManager.
 */
void Server::Wuerfeln(GameFunctionManager& manager) {

	int id = manager.getCurrentPlayer();

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
					if (tile > 0) {
						manager.getPlayers()[id].addMoney(200);			//200 wenn nicht genau auf LOS, wenn genau, dann bekommt man später 400
					}
				}
		fuehreFeldAktionAus(manager, id, tile);
		}
		if ((manager.getPaschCounter() == 1 || manager.getPaschCounter() == 2) && !manager.getPlayers()[id].getGameOver()) {		//Wenn a Pasch geworfen und b noch nicht GameOver und c es kommt ein weiterer Wurf
			cout << "Pasch! Du darfst nochmal würfeln..." << endl;							// nur für delay und cout! Keine Funktionalität!
			std::this_thread::sleep_for(std::chrono::milliseconds(2000));
		}
			
	} while ((manager.getPaschCounter() == 1 || manager.getPaschCounter() == 2) && !manager.getPlayers()[id].getGameOver());		//nochmal wuerfeln, wenn du Pasch hattest und nicht Game Over bist

	naechsterSpieler(manager);
}

/**
 * @brief Prueft, ob der aktuelle Spieler im Gefaengnis ist, und behandelt die Freikauf- oder Paschlogik.
 * @param gamefunc Referenz auf den GameFunctionManager.
 */
void Server::GefaengnisCheck(GameFunctionManager& gamefunc) {
	int id = gamefunc.getCurrentPlayer();

	if (gamefunc.getPlayers()[id].getPrisonCount() > 0) {
		cout << gamefunc.getPlayers()[id].getName() <<" befindet sich leider im Gefaengnis." << endl;
		if (gamefunc.getPlayers()[id].getMoney() > 100) {			//wenn genug Geld zum Freikaufen da
			cout << "Moechtest du dich freikaufen(100 Euro) oder lieber versuchen, einen Pasch zu wuerfeln ? (dein budget : " << gamefunc.getPlayers()[id].getMoney() << ")" << " (0: wuerfeln, 1 : freikaufen)" << endl;
			cin >> freikaufen;
			if (freikaufen) {
				gamefunc.getPlayers()[id].addMoney(-100);
				gamefunc.getPlayers()[id].setPrisonCount(0);  // Gefaengnis-Zaehler zuruecksetzen
				cout << "Erfolgreich freigekauft, du kannst jetzt deinen Zug taetigen" << endl;
				this_thread::sleep_for(chrono::milliseconds(2000));
				return;
			}
			else {
				cout << "ok viel Glueck dann beim Pasch werfen ;)" << endl;
				Paschwerfen(gamefunc);
				return;
			}
		}
		else {
			cout << "Du hast leider nicht genug Geld, um dich freizukaufen :( Versuch dein Glueck beim Paschwerfen!" << endl;
			Paschwerfen(gamefunc);
			return;
		}

	}
}

/**
 * @brief Fuehrt einen Paschwurf im Gefaengnis durch und setzt ggf. den Spieler frei.
 * @param game Referenz auf den GameFunctionManager.
 */
void Server::Paschwerfen(GameFunctionManager& game){
	int id = game.getCurrentPlayer();
	dice = game.rollDice();

	if (dice[0] == dice[1]) {									//Wenn Pasch geworfen
		cout << "Du hast einen Pasch! Du kommst aus dem Gefaengnis frei." << endl;
		game.getPlayers()[id].setPrisonCount(0);  // Zaehler zuruecksetzen
		dice.clear();
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
 * @brief Wechselt zum naechsten Spieler, prueft auf Game Over und startet ggf. eine neue Runde.
 * @param manager Referenz auf den GameFunctionManager.
 */
void Server::naechsterSpieler(GameFunctionManager& manager) {
	int id = manager.getCurrentPlayer();
	bool nochmal = false;
	config.writeLog(manager);				//Log schreiben
	

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
		config.loadConfig();
		settings = config.getSettings();
		if (currentround == settings.roundLimit) {
			cout << "Rundenlimit erreicht! Das Spiel endet jetzt." << endl;		//Rundenlimit erreicht
			//wer spielt noch?
			for (int i = 0; i < 4; i++) {
				if (!manager.getPlayers()[i].getGameOver()) {
					cout << manager.getPlayers()[i].getName() << " ist noch im Spiel und hat " << manager.getPlayers()[i].getMoney() << " Euro." << endl;
				}
			}
			//welcher Spieler hat am meisten Geld?
			maxMoney = 0; 
			winnerID = -1;
			winnerID2 = -1;
			winnerID3 = -1;
			bool aufhoeren = false; //Variable zum Beenden der Schleife, wenn 3 Spieler gleich viel Geld haben
			for (int i = 0; i < 4; i++) {
				if (!manager.getPlayers()[i].getGameOver() && manager.getPlayers()[i].getMoney() > maxMoney) {
					maxMoney = manager.getPlayers()[i].getMoney();
					winnerID = i;
					winnerID2 = -1; //reset winnerID2
					winnerID3 = -1; //reset winnerID3

					for (int l = i + 1; l < 4; l++) { //checken ob naechste Spieler den gleichen Betrag haben
						if (!aufhoeren) { //solange nicht 3 Spieler gleich viel Geld haben
							if (!manager.getPlayers()[l].getGameOver() && manager.getPlayers()[l].getMoney() == maxMoney) { //wenn 2 Spieler gleich viel Geld haben
								winnerID2 = l; //letzter Spieler, der den gleichen Betrag hat
								winnerID3 = -1; //reset winnerID3
								cout << "winner2" << winnerID2 << endl;

								for (int j = l + 1; j < 4; j++) { //checken ob naechste Spieler den gleichen Betrag haben
									if (!manager.getPlayers()[j].getGameOver() && manager.getPlayers()[j].getMoney() == maxMoney) { //wenn 3 Spieler gleich viel Geld haben
										winnerID3 = j;
										cout << "winner3" << winnerID3 << endl;
										//wenn 4 Spieler gleich viel Geld haben
										int k = j + 1; //naechster Spieler
										if (!manager.getPlayers()[k].getGameOver() && manager.getPlayers()[k].getMoney() == maxMoney && k == 3) {
											cout << "Ihr habt alle gleich viel Geld, wow wie habt ihr das geschafft?" << endl; //wenn alle 4 Spieler gleich viel Geld haben	
											config.sammlungHighscore(manager.getPlayers());
											Ende();
										}
										else { aufhoeren = true; }
									}
								}
							}
						}
					}
				}
				
			}
            if (winnerID3 != -1) { //wenn 3 Spieler gleich viel Geld haben
				cout << "Es gibt einen Gleichstand zwischen " << manager.getPlayers()[winnerID].getName() << ", " << manager.getPlayers()[winnerID2].getName() << " und " << manager.getPlayers()[winnerID3].getName() << endl; //wenn 3 Spieler gleich viel Geld haben
			}
			else if (winnerID2 != -1) { //wenn 2 Spieler gleich viel Geld haben
				cout << "Es gibt einen Gleichstand zwischen " << manager.getPlayers()[winnerID].getName() << " und " << manager.getPlayers()[winnerID2].getName() << endl; //wenn 2 Spieler gleich viel Geld haben
			}
			else {
				cout << manager.getPlayers()[winnerID].getName() << " hat gewonnen mit " << maxMoney << " Euro!" << endl; //Herausfinden, welcher Spieler gewonnen hat und Spiel beenden
			}
			config.sammlungHighscore(manager.getPlayers());
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
						config.sammlungHighscore(manager.getPlayers());
						Ende();
						return;
					}
				}
			}
		}

		manager.setCurrentPlayer(id);
	} while (nochmal);

	cout << "Naechster Spieler" << endl;
	this_thread::sleep_for(chrono::milliseconds(2000));
	GefaengnisCheck(manager);									//checken, ob gerade im Gefaengnis
}

/**
 * @brief Gibt eine Referenz auf den aktuellen MenuManager zurueck.
 * @return Referenz auf MenuManager.
 */
MenuManager& Server::getMenuManager() { return *menumanager; }

/**
 * @brief Setzt den MenuManager.
 * @param manager Referenz auf MenuManager.
 */
void Server::setMenuManager(MenuManager& manager) { this->menumanager = &manager; }

/**
 * @brief Gibt die aktuelle Konfiguration zurueck.
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
void Server::SpielstandSpeichern(GameFunctionManager& manager) {

	GameFunctionManager infos = manager;		//aktuellen Spieler merkenAdd commentMore actions

	for (int i = 0; i < 4; ++i) {				//Log schreiben und Spielstand speichern
		infos.setCurrentPlayer(i);
		getConfiguration().writeLog(infos);
	}
	config.saveGame();
	cout << "Das Spiel wurde gespeichert. Bis zum naechsten Mal :)" << endl;	//Spielstaende sind bereits gespeichert worden nach jedem Zug

	exit(0);
}

/*
Funktionsdefinitionen fuer die Moeglichkeitne von Mietfrei
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

/**
 * @brief Fuehrt die Aktionen auf dem aktuellen Feld aus, basierend auf dem Typ des Feldes.
 * Prueft, ob das Feld ein PropertyTile oder SpecialTile ist und fuehrt entsprechende Aktionen aus.
 * @param manager Referenz auf den GameFunctionManager.
 * @param id ID des aktuellen Spielers.
 * @param tile ID des aktuellen Feldes.
 */
void Server::fuehreFeldAktionAus(GameFunctionManager& manager, int id, int tile, bool bonusAufLosErlaubt) {			//// ALLE if/else zu PropertyTile, SpecialTile, Action-Switch-Case, Bahnhoefe etc.
	
	manager.showTileInfomation(tile);		//Feldeigenschaften anzeigen

	for (int i = 0; i < 4; i++) {		//Checken, ob schon jemand auf dem Feld steht
		if (i != id) {
			int positionofothers = manager.getPlayers()[i].getPosition();
			if (positionofothers == tile && positionofothers!=10) {
				cout << manager.getPlayers()[i].getName()
					<< " befindet sich gerade schon auf diesem Feld. Gib 50 Euro ab!" << endl;
				if (manager.getPlayers()[id].getMoney() > 50) {
					manager.getPlayers()[id].addMoney(-50);
					manager.getPlayers()[i].addMoney(50);
				}
				else {				//nicht genug Geld
					cout << "Du hast aber leider nicht genug Geld dafuer... \nGame Over fuer dich :(";
					manager.getPlayers()[id].setGameOver();
				}
			}
		}
	}
	
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
			cout << "Das Feld gehoert dir schon." << endl;
			if (manager.getMap().canUpgradeStreet(propTile, id)) {	//kannst du hier bauen?
				if (manager.getPlayers()[id].getMoney() > propTile->getHouseCost()) {	//genug Geld zum Bauen
					cout << "Moechtest du auf dieser Straße ein Haus bauen? (Kosten: " << propTile->getHouseCost() << " Euro)(dein Budget:" << manager.getPlayers()[id].getMoney() << ") (0: nein, 1: ja)" << endl;
					cin >> bauen;
					if (bauen) {
						propTile->setBuildingLevel(propTile->getBuildingLevel() + 1);
						manager.getPlayers()[id].addMoney(-propTile->getHouseCost());
						cout << "Herzlichen Glueckwunsch! Du hast ein Haus gebaut. Die Miete steigt!" << endl;
					}
					else {
						cout << "ok dann nicht" << endl;
					}
				}
				else {												//nicht genug Geld zum Bauen
					cout << "Du könntest, aber hast nicht genug Geld, um hier ein Haus zu bauen. Naja" << endl;
				}
			}
			else {
				cout << "Du kannst hier aktuell kein Haus bauen!" << endl;
			}
		this_thread::sleep_for(chrono::milliseconds(3000));
		}
		else {																			//Miete zahlen, wenn vergeben
			rent = propTile->getRent();
			ownerid = propTile->getOwnerId();

			cout << "Das Feld besitzt " << manager.getPlayers()[ownerid].getName() << ". Du musst " << rent << " Euro Miete zahlen." << endl;

			if (manager.getPlayers()[id].getMoney() > rent) {					//genug Geld fuer Miete

				manager.getPlayers()[id].addMoney(-rent);
				manager.getPlayers()[ownerid].addMoney(rent);
			}
			else {													//nicht genug Geld fuer Miete
				cout << "Du hast aber leider nicht genug Geld dafuer... \nGame Over fuer dich :(";
				//Game Over
				manager.getPlayers()[id].setGameOver();
			}
		this_thread::sleep_for(chrono::milliseconds(3000));
		}

	}

	else if (auto specialTile = dynamic_cast<const SpecialTile*>(sTile.get())) {	//auf Special Tile

		specialtile = specialTile->getTypeString();
		if (specialtile == "Start") {
			manager.getPlayers()[id].addMoney(400);									//Wenn man direkt auf Los landet bekommt man 400 Euro
		}
		else if (specialtile == "Jail") {
			cout << "Du bist nur zu Besuch hier" << endl;
			this_thread::sleep_for(chrono::milliseconds(3000));
		}
		else if (specialtile == "GoToJail") {							//Ins Gefaengnis gehen
			cout << "Gehe ins Gefaengnis!" << endl;
			manager.getPlayers()[id].setPrisonCount(3);
			manager.getPlayers()[id].setPosition(10);
			this_thread::sleep_for(chrono::milliseconds(3000));
		}
		else if (specialtile == "FreeParking") {						//Frei Parken
			int potGewinn = getPot();
			if (potGewinn > 0) {
				manager.getPlayers()[id].addMoney(potGewinn);
				clearPot();
				cout << "Glueckwunsch! Du bekommst " << potGewinn << "Euro vom Freiparken-Pot!" << endl;
			}
			else {
				cout << "Hier kannst du chillen und frei parken :)" << endl;
			}
			this_thread::sleep_for(chrono::milliseconds(3000));
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
			this_thread::sleep_for(chrono::milliseconds(3000));
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
			this_thread::sleep_for(chrono::milliseconds(3000));
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

					manager.showTileInfomation(wunschfeldid);			//Feldeigenschaften
					fuehreFeldAktionAus(manager, id, wunschfeldid);		//Feldaktion

				}
				else {												//moechte nicht fliegen
					cout << "ok dann bleibst du hier stehen" << endl;
				}
			}
			else {												//nicht genug Geld zum Fliegen
				cout << "Du hast aber nicht so viel, also bleibst du hier ;)" << endl;
			}
			this_thread::sleep_for(chrono::milliseconds(3000));
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
			this_thread::sleep_for(chrono::milliseconds(3000));
		}

		else if (specialtile == "Action") {
			cout << "Du bist auf einem Aktionsfeld gelandet! Hoffentlich bekommst du was gutes:" << endl;
			int actionCard = ((manager.randomNumber() - 1) * 6 + (manager.randomNumber() - 1)) % 12 + 1;
			this_thread::sleep_for(chrono::milliseconds(3000));
			//berechnet random number zwischen 1 und 12 anhand einer funktion die nur zwischen 1 und 6 berechnet
			switch (actionCard) {
			case 1:
				cout << "Die Parkgebuehren steigen – zahle 50 Euro in den Freipark-Topf!" << endl;
				if (manager.getPlayers()[id].getMoney() < 50) {		//nicht genug Geld
					cout << "Du hast aber leider nicht genug Geld dafuer... \nGame Over fuer dich :(";
					manager.getPlayers()[id].setGameOver();
				}
				else {
					manager.getPlayers()[id].addMoney(-50);
					addToPot(50);
				}
				this_thread::sleep_for(chrono::milliseconds(3000));
				break;
			case 2: {
				//Gib 50 Euro an den naechsten Spieler
				int nextId;
				bool found = false;
				for (int i = 1; i < 4; ++i) {					//finde naechsten lebendigen Spieler (kandidaten)
					int candidate = (id + i) % 4;
					if (!manager.getPlayers()[candidate].getGameOver()) {
						nextId = candidate;
						found = true;
						break;
					}
				}
				if (!found) {
					cout << "Alle anderen sind Game Over! Du behaeltst deine 50 Euro. Warum spielst du eigentlich noch?" << endl;
					Ende();
				}
				else {
					
					cout << "Grosszuegigkeit tut gut: Du gibst "
						<< manager.getPlayers()[nextId].getName()
						<< " 50 Euro!" << endl;
					if (manager.getPlayers()[id].getMoney() < 50) {		//nicht genug Geld
						cout << "Du hast aber leider nicht genug Geld... \nGame Over fuer dich :(";
						manager.getPlayers()[id].setGameOver();
					}
					else {
						manager.getPlayers()[id].addMoney(-50);
						manager.getPlayers()[nextId].addMoney(50);
					}
				}
				this_thread::sleep_for(chrono::milliseconds(3000));
				break;
			}

			case 3:
				manager.getPlayers()[id].addMoney(50);
				cout << "Die Bank hat dich lieb heute – du bekommst 50 Euro geschenkt!" << endl;
				this_thread::sleep_for(chrono::milliseconds(3000));
				break;
			case 4:
				cout << "Der Wuerfelgott schubst dich voran: Ziehe 3 Felder nach vorne." << endl;
				manager.getPlayers()[id].setPosition(
					(manager.getPlayers()[id].getPosition() + 3) % 40);
				if (manager.getPlayers()[id].getPosition() == 1 || manager.getPlayers()[id].getPosition() == 2) {	//Wenn du ueber Los kommst
					manager.getPlayers()[id].addMoney(200);				//200 Euro kassieren
				}
				this_thread::sleep_for(chrono::milliseconds(3000));
				fuehreFeldAktionAus(manager, id, manager.getPlayers()[id].getPosition());
				break;
			case 5:
				cout << "Oh je! Ein Rueckschlag. Gehe 3 Felder zurueck (aber nicht den Kopf haengen lassen)." << endl;
				manager.getPlayers()[id].setPosition((manager.getPlayers()[id].getPosition() - 3 + 40) % 40);		//aufpassen wegen negativ !
				this_thread::sleep_for(chrono::milliseconds(3000));
				fuehreFeldAktionAus(manager, id, manager.getPlayers()[id].getPosition());
				break;
			case 6:
				cout << "Ups... Direkt ins Gefaengnis! Gehe nicht ueber Los, ziehe keine 200 Euro ein. Gar nicht gut" << endl;
				manager.getPlayers()[id].setPosition(10);
				manager.getPlayers()[id].setPrisonCount(3);
				this_thread::sleep_for(chrono::milliseconds(3000));
				fuehreFeldAktionAus(manager, id, 10, false);
				break;
			case 7:
				cout << "Du Glueckspilz! Du findest 35 Euro auf der Straße!" << endl;
				manager.getPlayers()[id].addMoney(35);
				this_thread::sleep_for(chrono::milliseconds(3000));
				break;
			case 8:
				cout << "Die Sterne stehen richtig! Du bekommst 65 Euro." << endl;
				manager.getPlayers()[id].addMoney(65);
				this_thread::sleep_for(chrono::milliseconds(3000));
				break;
			case 9:
				cout << "Ab auf die Schlosspromenade! Goenn dir den besten Stadtteil (hoffentlich)." << endl;
				manager.getPlayers()[id].setPosition(39);
				this_thread::sleep_for(chrono::milliseconds(3000));
				fuehreFeldAktionAus(manager, id, 39);
				manager.showTileInfomation(39);
				break;
			case 10: {
				// Kandidaten sammeln: Alle aktiven Spieler außer dir selbst
				vector<int> kandidaten;
				for (int i = 0; i < 4; ++i) {
					if (i != id && !manager.getPlayers()[i].getGameOver())
						kandidaten.push_back(i);
				}
				// Gibt es ueberhaupt einen Kandidaten?
				if (kandidaten.empty()) {
					cout << "Alle anderen sind Game Over! Niemand zum Tauschen da. Hier ne gute Idee: Such dir Leute mit denen du spielen kannst!" << endl;
					Ende();
				}
				else {
					// Einen zufaelligen Kandidaten auswaehlen (1-6 randomnumber, modulo Kandidatenzahl)
					int randomIndex = (manager.randomNumber() - 1) % kandidaten.size();
					int tauschPartner = kandidaten[randomIndex];

					// Positionen tauschen
					int tmp = manager.getPlayers()[id].getPosition();
					manager.getPlayers()[id].setPosition(manager.getPlayers()[tauschPartner].getPosition());
					manager.getPlayers()[tauschPartner].setPosition(tmp);

					cout << "Verwirrung! Du tauschst die Position mit "
						<< manager.getPlayers()[tauschPartner].getName() << endl;
					manager.showTileInfomation(manager.getPlayers()[id].getPosition());
					fuehreFeldAktionAus(manager, id, manager.getPlayers()[id].getPosition());
				}
				this_thread::sleep_for(chrono::milliseconds(3000));
				break;
			}
			case 11:
				cout << "Alarm! SOFORT auf LOS! Mach schnell, bevor jemand merkt, dass du gleich 400 Euro kassierst!" << endl;
				manager.getPlayers()[id].setPosition(0);
				this_thread::sleep_for(chrono::milliseconds(3000));
				break;
			case 12:
				cout << "Gehe auf Frei Parken!" << endl;
				manager.getPlayers()[id].setPosition(20);
				if (getPot() > 0) {
					int gewinn = getPot();
					manager.getPlayers()[id].addMoney(gewinn);
					clearPot();
					cout << "Du bekommst " << gewinn << " Euro aus dem Freiparken-Pot!" << endl;
				}
				else {
					cout << "Leider ist der FreiParken Pot aber noch leer :( Vielleicht hast du beim naechsten Mal mehr Glück!" << endl;
				}
				this_thread::sleep_for(chrono::milliseconds(3000));
				break;
			}
		}
	}
}
void Server::handleTrade(GameFunctionManager& manager, int currentPlayerId) {
	auto& players = manager.getPlayers();
	int otherPlayerId;

	cout << "Mit wem moechtest du handeln? (Gib die Player-ID ein(0 bis 3))" << endl;
	for (int i = 0; i < players.size(); ++i) {
		if (i != currentPlayerId && !players[i].getGameOver()) { // Nicht man selbst und nur aktive Spieler
			cout << i << ": " << players[i].getName() << endl;
		}
	}
	cin >> otherPlayerId;
	if (otherPlayerId == currentPlayerId || otherPlayerId < 0 || otherPlayerId >= players.size() || players[otherPlayerId].getGameOver()) {
		cout << "Ungueltige Auswahl. Handel abgebrochen." << endl;
		return;
	}

	// Was möchtest du haben?
	string wantedProperty;
	cout << "Welche Straße moechtest du erhalten (nur eine, nicht so gierig!)? (Name eingeben, exakt wie im Spiel)" << endl;
	cin.ignore(); // Überspringt ein Zeilenende von vorherigem cin
	getline(cin, wantedProperty); // Straßenname als ganze Zeile einlesen

	// Was bist du bereit zu geben?
	vector<string> offeredProperties;
	int offerMoney = 0;

	cout << "Welche Strassen moechtest du anbieten? (Nacheinander eingeben, mit ' ' trennen. 'ende' eingeben um zu beenden.)" << endl;
	while (true) {
		string offer;
		getline(cin, offer);
		if (offer == "ende") break;
		offeredProperties.push_back(offer);
	}

	cout << "Wieviel Geld bist du bereit zusätzlich zu geben? (Betrag in Euro, 0 für kein Geld)" << endl;
	cin >> offerMoney;

	// Zeige Angebot dem Zielspieler
	cout << players[otherPlayerId].getName() << ", folgendes Angebot wurde dir gemacht:" << endl;
	cout << players[currentPlayerId].getName() << " möchte deine Straße '" << wantedProperty << "' und bietet dafür: ";
	if (!offeredProperties.empty()) {
		cout << "die Straßen ";
		for (const auto& str : offeredProperties) cout << "'" << str << "' ";
	}
	if (offerMoney > 0) cout << "und " << offerMoney << " Euro";
	cout << endl;
	std::this_thread::sleep_for(std::chrono::milliseconds(3000));
	cout << "Willst du den Handel annehmen? (0 = Ablehnen, 1 = Annehmen)" << endl;
	int akzeptieren;
	cin >> akzeptieren;

	if (!akzeptieren) {
		cout << "Handel wurde abgelehnt." << endl;
		return;
	}

	// 1. Gesuchte Straße vom targetPlayer an currentPlayer (Besitz übertragen)
	bool found = false;
	for (auto& tile : manager.getMap().tiles) {
		if (auto propTile = dynamic_cast<PropertyTile*>(tile.get())) {
			if (propTile->getName() == wantedProperty && propTile->getOwnerId() == otherPlayerId) {
				propTile->setOwner(currentPlayerId);
				found = true;
				break;
			}
		}
	}

	if (!found) {
		cout << "Der Zielspieler besitzt diese Straße nicht (mehr). Handel abgebrochen. Schau mal lieber nochmal nach!" << endl;
		std::this_thread::sleep_for(std::chrono::milliseconds(3000));
		return;
	}

	// 2. Angebote der Straßen vom currentPlayer an otherPlayer geben
	for (const auto& offerStr : offeredProperties) {
		for (auto& tile : manager.getMap().tiles) {
			if (auto propTile = dynamic_cast<PropertyTile*>(tile.get())) {
				if (propTile->getName() == offerStr && propTile->getOwnerId() == currentPlayerId) {
					propTile->setOwner(otherPlayerId);
				}
			}
		}
	}

	// 3. Geld transferieren
	if (offerMoney > 0) {
		if (players[currentPlayerId].getMoney() >= offerMoney) {
			players[currentPlayerId].addMoney(-offerMoney);
			players[otherPlayerId].addMoney(offerMoney);
		}
		else {
			std::this_thread::sleep_for(std::chrono::milliseconds(3000));
			cout << "Du hast nicht genug Geld für das Angebot. Handel abgebrochen." << endl;
			// Rückabwicklung: Die getauschte Straße zurücktauschen!
			for (auto& tile : manager.getMap().tiles) {
				if (auto propTile = dynamic_cast<PropertyTile*>(tile.get())) {
					if (propTile->getName() == wantedProperty && propTile->getOwnerId() == currentPlayerId) {
						propTile->setOwner(otherPlayerId);
						break;
					}
				}
			}
			return;
		}
	}

	cout << "Handel erfolgreich durchgeführt! Viel Erfolg!" << endl;
	std::this_thread::sleep_for(std::chrono::milliseconds(3000));
}



