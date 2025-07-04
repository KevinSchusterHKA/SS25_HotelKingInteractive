#ifndef CONFIGURATION_HPP
#define CONFIGURATION_HPP

#include <string>
#include <vector>
#include "GameSettings.h"
#include "GameFunctionManager.hpp"

using namespace std;

class Configuration {
private:
	GameSettings settings;
	string configPath = "../Config/config.txt";
	string logPath = "game.log";
	string savePath = "save.txt";
	string highscorePath = "highscore.txt";

public:
	GameSettings getSettings();

	//config
	bool loadConfig();
	void printSettings();

	//save+load
	void writeLog(GameFunctionManager info);
	void clearLog();
	void saveGame();
	GameFunctionManager loadGame();
	void printLoadGame(GameFunctionManager g);

	//highscore
	void sammlungHighscore(vector<Player> players);
	vector<Player> sortedHighscore();
	void showHighscore(vector<Player> p);
};
#endif
