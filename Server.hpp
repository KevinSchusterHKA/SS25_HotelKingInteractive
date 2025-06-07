#include <iostream>
#include <vector>
#include <string>

#include "Player.hpp"
#include "Configuration.h"
#include "GameFunctionManager.hpp"
#include "MenuManager.hpp"
#include "Menu.hpp"

using namespace std;


class Server {
private:
	vector<Player> players;
	int numberOfPlayer;
	int numberOfCPUPlayer;
	string playerName;
	string rulesPath;


public:
	vector<Player> showMainDialog();
	void showIngameDialog();



};