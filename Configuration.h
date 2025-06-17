#pragma once
#include <string>
#include <vector>
#include "GameSettings.h"
#include "Score.h"
#include "GameFunctionManager.hpp"

using namespace std;

class Configuration {
private:
	GameSettings settings;

public:
	GameSettings getSettings();

	//config
	bool loadConfig(string path);
	void printSettings();

	//save+load
	void writeLog(GameFunctionManager info);
	void saveGame(string logPath, int wieVieleSpieler);
	GameFunctionManager loadGame(string savePath);

	//highscore
	void sammlungHighscore(vector<Player> players);
	vector<Player> sortedHighscore(string path);
	void showHighscore(vector<Player> p);

};
