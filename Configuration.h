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
	//string logFile;
public:
	bool loadConfig(string path);
	void printSettings();
	void writeLog(InfoGame info);

	void  saveGame(string logPath, string savePath, int wieVieleSpieler);
	//void loadGame(string path);
	GameSettings getSettings();
	
};
