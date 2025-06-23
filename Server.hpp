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
	vector<int> dice;
	int numberOfPlayer;
	int numberOfCPUPlayer;
	string playerName;
	string rulesPath = "../Config/config.txt";
	bool buyfield;
	bool freikaufen;
	int price;
	int rent;
	int ownerid;
	string specialtile;
	int wunschfeldid;
	int positionofothers;
	bool transport;
	MenuManager* menumanager;
	Configuration config;
	void Paschwerfen(GameFunctionManager& manager);
	void naechsterSpieler(GameFunctionManager& manager);


public:
	Server(vector<Player>& playersRef);
	void SpielStarten();
	void SpielLaden();
	MenuManager& getMenuManager();
	Configuration getConfiguration();
	void setMenuManager(MenuManager& manager);
	void showIngameDialog();
	void Spielzug(GameFunctionManager& manager);
	void Wuerfeln(GameFunctionManager& manager);
	void GefaengnisCheck(GameFunctionManager& gamefunc);



};