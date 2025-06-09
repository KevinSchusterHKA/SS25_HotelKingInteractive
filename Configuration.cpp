#include <iostream>
#include <fstream>
#include <sstream>
#include "Configuration.h"
using namespace std;

GameSettings Configuration::getSettings() {
	return settings;
}

bool Configuration::loadConfig(string path) {
	ifstream file(path);	//offen

	if (!file.is_open()) {		//Ob nicht offen
		cout << "Datei konnte nicht geöffnet werden" << endl; 
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
		if (key == "playerCount") { settings.playerCount = stoi(value); }
		else if (key == "cpuCount") { settings.cpuCount = stoi(value); }
		else if (key == "startBudget") { settings.startBudget = stoi(value); }
		else if (key == "roundLimit") { settings.roundLimit = stoi(value); }
		else if (key == "timeLimit") { settings.timeLimit = stoi(value); }
		else if (key == "budgetLimit") { settings.budgetLimit = stoi(value); }
		else if (key == "cpuDifficulty") { settings.cpuDifficulty = value; }
		else if (key == "gameMode") {
			if (value == "RUNDENBASIERT") settings.gameMode = GameMode::RUNDENBASIERT;
			else if (value == "ZEITBASIERT") settings.gameMode = GameMode::ZEITBASIERT;
			else if (value == "BUDGETBASIERT") settings.gameMode = GameMode::BUDGETBASIERT;
		}
	}
	file.close();
	return true;
}

void Configuration::printSettings() {
	string mode;
	switch (settings.gameMode) {
	case GameMode::RUNDENBASIERT: mode = "RUNDENBASIERT"; break;
	case GameMode::ZEITBASIERT: mode = "ZEITBASIERT"; break;
	case GameMode::BUDGETBASIERT: mode = "BUDGETBASIERT"; break;
	default: mode = "UNBEKANNT"; break;
	}

	cout << "playerCount: " << settings.playerCount << endl;
	cout << "cpuCount: " << settings.cpuCount << endl;
	cout << "startBudget: " << settings.startBudget << endl;
	cout << "roundLimit: " << settings.roundLimit << endl;
	cout << "timeLimit: " << settings.timeLimit << endl;
	cout << "budgetLimit: " << settings.budgetLimit << endl;
	cout << "cpuDifficulty: " << settings.cpuDifficulty << endl;
	cout << "gameMode: " << mode << endl;
}

void Configuration::writeLog(InfoGame info) {
	ofstream logFile("game.log", ios::app);		//append

	if (!logFile.is_open()) {
		cout << "Fehler beim Öffnen der Log-Datei." << endl;
		return;
	}

	//write
	logFile << "Round = " << info.round
			<< ", playerID  =" << info.playerID
			<< ", Budget = " << info.budget
			<< ", ownship = " << info.ownship
			<< ", position = " << info.position << endl;

	logFile.close();
}

void Configuration::saveGame(string logPath, string savePath, int wieVieleSpieler) {
/***************************   lexicalische analyse für log-Dateil  **********************************************************/
	vector<InfoGame> parsedLog;
	int maxRound = 0;
	int budget = settings.startBudget;
	vector<string> ownship;
	
	ifstream logFile(logPath);
	if (!logFile.is_open()) {
		cout << "Fehler beim Öffnen der Log-Datei." << endl;
		return;
	}

	string zeile;
	while (getline(logFile, zeile)) {
		if (zeile.empty()) { continue; }

		InfoGame info;
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

			if (key == "Round") info.round = stoi(value);
			else if (key == "playerID") info.playerID = stoi(value);
			else if (key == "Budget") info.budget = stoi(value);
			else if (key == "ownship") info.ownship = value;
			else if (key == "position") info.position = stoi(value);
		}
		parsedLog.push_back(info);
		if (info.round > maxRound) { maxRound = info.round; }	//update round
		
	}
	logFile.close();

/****************************************** Neue Dateil save  ************************************************************************/
	ofstream saveFile(savePath);
	if (!saveFile.is_open()) {
		cout << "Speicherdatei konnte nicht geöffnet werden." << endl;
		return;
	}
	int ind = wieVieleSpieler;
	saveFile << "# SPIELZUSTAND SPEICHERUNG" << endl;
	saveFile << "round = " << maxRound << endl << endl;
	for (int i = parsedLog.size()-1; i >= parsedLog.size() - wieVieleSpieler; i--) {	//lese die letze zeile von log-Datei
		saveFile << "# Spieler " << ind-- << endl;
		saveFile << "playerID = " << parsedLog[i].playerID << endl;
		saveFile << "budget = " << parsedLog[i].budget << endl;
		saveFile << "position = " << parsedLog[i].position << endl;
		saveFile << "besitz = " << parsedLog[i].ownship << endl << endl;
	}
	saveFile.close();
}

