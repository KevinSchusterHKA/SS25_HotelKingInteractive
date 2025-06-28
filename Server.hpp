#include <iostream>
#include <vector>
#include <string>

#include "Player.hpp"
#include "Configuration.h"
#include "GameFunctionManager.hpp"
#include "MenuManager.hpp"
#include "Menu.hpp"

using namespace std;

/**
* @brief Server class that manages the game state and player interactions.
*/
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
	int bauen;
	int freiparkenpot;
	MenuManager* menumanager;
	Configuration config;
	GameSettings settings;
	void fuehreFeldAktionAus(GameFunctionManager& manager, int id, int tile);
	void GefaengnisCheck(GameFunctionManager& gamefunc);
	void Paschwerfen(GameFunctionManager& manager);
	void naechsterSpieler(GameFunctionManager& manager);
	void Ende();
	int pot = 0;
	void addToPot(int betrag);
	int getPot() const;
	void clearPot();


public:
	Server(vector<Player>& playersRef);
	void SpielStarten();
	void SpielLaden();
	MenuManager& getMenuManager();
	Configuration getConfiguration();
	void setMenuManager(MenuManager& manager);
	void Spielzug(GameFunctionManager& manager);
	void Wuerfeln(GameFunctionManager& manager);
	void SpielstandSpeichern();
};