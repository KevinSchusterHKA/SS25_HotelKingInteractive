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
	GameSettings settings;
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
	void fuehreFeldAktionAus(GameFunctionManager& manager, int id, int tile, bool bonusAufLosErlaubt = true);
	void GefaengnisCheck(GameFunctionManager& gamefunc);
	void Paschwerfen(GameFunctionManager& manager);
	void naechsterSpieler(GameFunctionManager& manager);
	void Ende();
	int pot;
	void addToPot(int betrag);
	void clearPot();
	int maxMoney;
	int winnerID;
	int winnerID2;
	int winnerID3;


public:
	Server(vector<Player>& playersRef);
	void SpielStarten();
	void SpielLaden();
	MenuManager& getMenuManager();
	Configuration getConfiguration();
	void setMenuManager(MenuManager& manager);
	void Spielzug(GameFunctionManager& manager);
	void Wuerfeln(GameFunctionManager& manager);
	void SpielstandSpeichern(GameFunctionManager& manager);
	int getPot() const;
	void handleTrade(GameFunctionManager& manager, int currentPlayerId);
};