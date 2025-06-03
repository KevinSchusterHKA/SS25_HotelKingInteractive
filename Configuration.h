#pragma once
#include <string>
#include <vector>
#include "GameSettings.h"
#include "Score.h"
using namespace std;

class Configuration {
private:
	GameSettings settings;
	vector<Score> highscores;
	string logFile;
public:
	bool loadConfig(string path);
	void printSettings();
	void writeLog(int round, int playerID, int sup_budget, string sup_ownship, int position);

	//naechste Sprint 
	//bool saveGame(string path);
	//bool loadGame(string path);
	GameSettings getSettings();
	
};
