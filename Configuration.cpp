#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include "Configuration.h"
#include "Player.hpp"

using namespace std;

GameSettings Configuration::getSettings() {
	return settings;
}

bool Configuration::loadConfig() {
	ifstream file(configPath);	//offen

	if (!file.is_open()) {		//Ob nicht offen
		cout << "Log-Datei konnte nicht geoeffnet werden" << endl;
		return false;
	}


	string zeile;
	while (getline(file, zeile)) {
		// Leere Zeilen oder Kommentare überspringen
		if (zeile.empty() || zeile[0] == '#') { continue; }

		//Such '='
		size_t pos = zeile.find('=');	//size_t = unsigned integer
		if (pos == string::npos) { continue; }	//wobei npos = nicht gefunden

		string key = zeile.substr(0, pos);
		string value = zeile.substr(pos + 1);

		// Entferne Leerzeichen
		key.erase(remove(key.begin(), key.end(), ' '), key.end());
		value.erase(remove(value.begin(), value.end(), ' '), value.end());

		// Zuordnen
		if (key == "startBudget") { settings.startBudget = stoi(value); }
		else if (key == "roundLimit") { settings.roundLimit = stoi(value); }
	}
	file.close();
	return true;
}

void Configuration::printSettings() {
	cout << "startBudget: " << settings.startBudget << endl;
	cout << "roundLimit: " << settings.roundLimit << endl;
}

void Configuration::writeLog(GameFunctionManager info) {
	ofstream logFile(logPath, ios::app);		//append
	if (!logFile.is_open()) {
		cout << "Fehler beim Oeffnen der Log-Datei." << endl;
		return;
	}

	//Welche Spieler hat etwas gemacht
	vector<Player> welchePlayers = info.getPlayers();
	Player p = welchePlayers[info.getCurrentPlayer()];

	//write
	logFile << "Round = " << info.getCurrentRound()
		<< ", playerID  = " << p.getID()
		<< ", name = " << p.getName()
		<< ", Budget = " << p.getMoney()
		<< ", karten = ";
	vector<string> karten = p.getKarten();
	for (size_t i = 0; i < karten.size(); ++i) {
		logFile << karten[i];
		if (i != karten.size() - 1) logFile << "|";
	}
	logFile << ", position = " << p.getPosition()
		<< ", prisonCount  = " << p.getPrisonCount();

	// Grundstücke
	logFile << ", grundstuecke = ";
	bool first = true;
	for (int i = 0; i < 40; i++) {
		PropertyTile* propertyTile = dynamic_cast<PropertyTile*>(info.getMap().getTile(i).get());
		if (propertyTile && propertyTile->getOwnerId() == p.getID()) {
			if (!first) logFile << "|";
			logFile << propertyTile->getName() << "/" << propertyTile->getRent() << "/" << propertyTile->getBuildingLevel();
			first = false;
		}
	}
	logFile << ", gameOver = " << (p.getGameOver() ? "true" : "false");
	logFile << ", isRealPlayer = " << (p.isRealPlayer() ? "true" : "false");
	logFile << endl;
	logFile.close();
}

void Configuration::clearLog() {
	ofstream logFile(logPath, ios::trunc);	//trunc for empty file
	if (!logFile.is_open()) {
		cout << "Fehler beim Oeffnen der Log-Datei zum Leeren." << endl;
		return;
	}
	logFile.close();
	cout << "Log-Datei wurde erfolgreich geleert" << endl;
}

void Configuration::saveGame() {
	/***************************   lexicalische analyse für log-Dateil  **********************************************************/
	vector<Player> parsedPlayers;
	vector<string> parsedGrundstuecke;
	int maxRound = 0;
	int wieVieleSpieler = 4;
	int naechsteSpielerID = 0;

	ifstream logFile(logPath);
	if (!logFile.is_open()) {
		cout << "Fehler beim Oeffnen der Log-Datei." << endl;
		return;
	}

	string zeile;

	while (getline(logFile, zeile)) {
		if (zeile.empty()) { continue; }
		int round = 0, playerID = 0, budget = 0, position = 0, prisonCount = 0; string karten = "", name = "", prison = "", grundstuecke = "", gameOver = "", isRealPlayer = "";

		// Aufteilen der Zeile in Schlüssel-Wert-Paare
		stringstream ss(zeile);
		string segment;
		while (getline(ss, segment, ',')) {
			size_t pos = segment.find('=');	//size_t = unsigned integer
			if (pos == string::npos) { continue; }	//wobei npos = nicht gefunden

			string key = segment.substr(0, pos);
			string value = segment.substr(pos + 1);

			// Entferne Leerzeichen
			key.erase(remove(key.begin(), key.end(), ' '), key.end());
			value.erase(remove(value.begin(), value.end(), ' '), value.end());

			if (key == "Round") round = stoi(value);
			else if (key == "playerID") playerID = stoi(value);
			else if (key == "name") name = value;
			else if (key == "Budget") budget = stoi(value);
			else if (key == "karten") karten = value;
			else if (key == "position") position = stoi(value);
			else if (key == "prisonCount") prisonCount = stoi(value);
			else if (key == "grundstuecke") grundstuecke = value;
			else if (key == "gameOver") gameOver = value;
			else if (key == "isRealPlayer") isRealPlayer = value;
		}

		if (round > maxRound) { maxRound = round; }	//update round

		parsedGrundstuecke.push_back(grundstuecke);

		// Rekonstruiere den Spieler
		Player p = (isRealPlayer == "true") ? Player(name, budget, playerID, true) : Player(name, budget, playerID, false);

		p.setPosition(position);
		p.setPrisonCount(prisonCount);
		if (gameOver == "true") { p.setGameOver(); }

		// Karten parsen
		stringstream kss(karten);
		string karte;
		while (getline(kss, karte, '|')) {
			if (!karte.empty())
				p.addKarte(karte);
		}
		parsedPlayers.push_back(p);
	}
	logFile.close();

	/****************************************** Neue Dateil save  ************************************************************************/
	ofstream saveFile(savePath);
	if (!saveFile.is_open()) {
		cout << "Speicherdatei konnte nicht geöffnet werden." << endl;
		return;
	}

	saveFile << "# SPIELZUSTAND SPEICHERUNG" << endl;
	saveFile << "round = " << maxRound << endl << endl;
	for (int i = parsedPlayers.size() - wieVieleSpieler; i < parsedPlayers.size(); i++) {	//lese die letze zeile von log-Datei
		saveFile << "# Spieler " << parsedPlayers[i].getID() + 1 << endl;
		saveFile << "name = " << parsedPlayers[i].getName() << endl;
		saveFile << "playerID = " << parsedPlayers[i].getID() << endl;
		saveFile << "budget = " << parsedPlayers[i].getMoney() << endl;
		saveFile << "position = " << parsedPlayers[i].getPosition() << endl;
		saveFile << "prisonCount = " << parsedPlayers[i].getPrisonCount() << endl;
		saveFile << "gameOver = " << (parsedPlayers[i].getGameOver() ? "true" : "false") << endl;;
		saveFile << "isRealPlayer = " << (parsedPlayers[i].isRealPlayer() ? "true" : "false") << endl;;
		saveFile << "karten = ";
		vector<string> karten = parsedPlayers[i].getKarten();
		for (size_t i = 0; i < karten.size(); ++i) {
			saveFile << karten[i];
			if (i != karten.size() - 1) saveFile << "|";
		}
		saveFile << endl;
		saveFile << "grundstuecke = " << parsedGrundstuecke[i] << endl << endl;

	}
	//current spieler
	parsedPlayers[parsedPlayers.size() - 5].getID() == 3 ? naechsteSpielerID = 0 : naechsteSpielerID = parsedPlayers[parsedPlayers.size() - 5].getID() + 1;	//naechster Spieler
	saveFile << "naechsteSpielerID = " << naechsteSpielerID << endl;

	saveFile.close();
}

GameFunctionManager Configuration::loadGame() {
	GameFunctionManager manager;
	ifstream saveFile(savePath);	//offen

	if (!saveFile.is_open()) {		//Ob nicht offen
		cout << "Save-Datei konnte nicht geoeffnet werden" << endl;
		return manager;	//leer
	}

	string zeile;
	Player tempPlayer("", 0, 0, true); int round = 0, naechsteSpieler = 0; string name = "", gameOver = "", isRealPlayer = "";
	while (getline(saveFile, zeile)) {
		// Leere Zeilen oder Kommentare überspringen
		if (zeile.empty() || zeile[0] == '#') { continue; }

		//Such '='
		size_t pos = zeile.find('=');	//size_t = unsigned integer
		if (pos == string::npos) { continue; }	//wobei npos = nicht gefunden

		string key = zeile.substr(0, pos);
		string value = zeile.substr(pos + 1);

		// Entferne Leerzeichen
		key.erase(remove(key.begin(), key.end(), ' '), key.end());
		value.erase(remove(value.begin(), value.end(), ' '), value.end());

		// Zuordnen
		if (key == "round") round = stoi(value);
		else if (key == "name") name = value;
		else if (key == "playerID") tempPlayer = Player(name, 0, stoi(value));
		else if (key == "isRealPlayer") isRealPlayer = value; if (isRealPlayer == "false") { tempPlayer = Player(name, 0, stoi(value), false); }
		else if (key == "budget") tempPlayer.addMoney(stoi(value));
		else if (key == "position") tempPlayer.setPosition(stoi(value));
		else if (key == "prisonCount") tempPlayer.setPrisonCount(stoi(value));
		else if (key == "gameOver") gameOver = value; if (value == "true") { tempPlayer.setGameOver(); }
		else if (key == "karten") {
			stringstream ss(value);
			string karte;
			while (getline(ss, karte, '|')) {
				tempPlayer.addKarte(karte);
			}
			manager.addPlayer(tempPlayer);
		}
		else if (key == "grundstuecke") {
			stringstream ss(value);
			string grundstueck;
			while (getline(ss, grundstueck, '|')) {
				// Neu: Name/Mite/Hausanzahl speichern und einlesen!
				size_t trenner1 = grundstueck.find('/');
				if (trenner1 == string::npos) continue;
				size_t trenner2 = grundstueck.find('/', trenner1 + 1);
				if (trenner2 == string::npos) continue;

				string propertyName = grundstueck.substr(0, trenner1);
				int rent = stoi(grundstueck.substr(trenner1 + 1, trenner2 - trenner1 - 1));
				int buildingLevel = stoi(grundstueck.substr(trenner2 + 1));

				for (int i = 0; i < 40; ++i) {
					shared_ptr<Tile> t = manager.getMap().getTile(i);
					PropertyTile* pt = dynamic_cast<PropertyTile*>(t.get());
					if (pt && pt->getName() == propertyName) {
						pt->setOwner(tempPlayer.getID());
						pt->setRent(rent);
						pt->setBuildingLevel(buildingLevel);
					}
				}
			}
		}
		else if (key == "naechsteSpielerID") naechsteSpieler = stoi(value);
	}
	manager.setCurrentRound(round);
	manager.setCurrentPlayer(naechsteSpieler);
	saveFile.close();
	return manager;
}

void Configuration::printLoadGame(GameFunctionManager g) {
	cout << "Aktuelle Runde: " << g.getCurrentRound() << endl;
	cout << "Aktuelle Spieler: " << g.getCurrentPlayer() << endl;

	for (const Player& p : g.getPlayers()) {
		cout << "-----------------------------" << endl;
		cout << "Name: " << p.getName() << endl;
		cout << "ID: " << p.getID() << endl;
		cout << "Budget: " << p.getMoney() << endl;
		cout << "Position: " << p.getPosition() << endl;
		cout << "Game Over? " << (p.getGameOver() ? "Ja" : "Nein") << endl;
		cout << "isRealPlayer? " << (p.isRealPlayer() ? "Ja" : "Nein") << endl;
		cout << "Gefaengnis-Runden: " << p.getPrisonCount() << endl;
		cout << "Karten: ";
		vector<string> karten = p.getKarten();
		for (const string& k : karten) {
			cout << k << " ";
		}
		cout << endl;
		cout << "Grundstuecke: ";
		bool first = true;
		for (int i = 0; i < 40; ++i) {
			PropertyTile* pt = dynamic_cast<PropertyTile*>(g.getMap().getTile(i).get());
			if (pt && pt->getOwnerId() == p.getID()) {
				if (!first) cout << " | ";
				cout << pt->getName() << "/" << pt->getRent() << "/" << pt->getBuildingLevel();
				first = false;
			}
		}
		cout << endl;
	}
}

void Configuration::sammlungHighscore(vector<Player> players) {
	ofstream highscoreFile("highscore.txt", ios::app);	//append für speichern alle Ergebnisse
	if (!highscoreFile.is_open()) {
		cout << "Fehler beim Oeffnen der Highscore-Datei" << endl;
		return;
	}

	//write
	for (int i = 0; i < players.size(); i++) {
		highscoreFile << players[i].getName() << "=" << players[i].getMoney() << endl;
	}

	highscoreFile.close();
}

vector<Player> Configuration::sortedHighscore() {
	vector<Player> sortedPlayers;

	ifstream file(highscorePath);	//offen
	if (!file.is_open()) {		//Ob nicht offen
		cout << "Datei-Highscore konnte nicht geoeffnet werden" << endl;
		return sortedPlayers;
	}

	string zeile;
	while (getline(file, zeile)) {
		// Leere Zeilen oder Kommentare überspringen und Leerzeichen nicht nutzlich hier. Ich habe die Highscore-Dateil selbst geschrieben
		//Such '='
		size_t pos = zeile.find('=');	//size_t = unsigned integer
		if (pos == string::npos) { continue; }	//wobei npos = nicht gefunden

		string name = zeile.substr(0, pos);
		string score = zeile.substr(pos + 1);

		//push
		Player p(name, stoi(score), 0);
		sortedPlayers.push_back(p);
	}
	file.close();

	//sortier highscore
	sort(sortedPlayers.begin(), sortedPlayers.end(), [](Player& a, Player& b) { return a.getMoney() > b.getMoney(); });


	return sortedPlayers;
}

void Configuration::showHighscore(vector<Player> p) {
	for (int i = 0; i < p.size(); i++) {
		cout << p[i].getName() << " " << p[i].getMoney() << endl;
	}
}

