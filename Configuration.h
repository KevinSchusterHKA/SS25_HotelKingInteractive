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
	vector<Score> highscores;

public:
	bool loadConfig(string path);
	void printSettings();
	void writeLog(GameFunctionManager info);

	void saveGame(string logPath, string savePath, int wieVieleSpieler);
	bool loadGame(string path, vector<InfoGame>& playersInfo);

	GameSettings getSettings();
	
};
