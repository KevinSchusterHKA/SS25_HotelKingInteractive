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

