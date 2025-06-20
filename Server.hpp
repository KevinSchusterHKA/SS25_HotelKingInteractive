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
	//vector<Player> players;
	vector<Player>& players;
	int numberOfPlayer;
	int numberOfCPUPlayer;
	string playerName;
	string rulesPath = "../Config/config.txt";
	bool buyfield;
	int price;
	int rent;
	int ownerid;
	string specialtile;
	int wunschfeldid;
	int positionofothers;
	bool transport;
	MenuManager* menumanager;


public:
	Server(vector<Player>& playersRef);
	void SpielStarten();
	void SpielLaden();
	MenuManager& getMenuManager();
	void setMenuManager(MenuManager& manager);
	void showIngameDialog();
	void Spielzug(GameFunctionManager& manager);



};