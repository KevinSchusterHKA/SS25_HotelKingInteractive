#ifndef GAMEFUNCTIONMANAGER_HPP
#define GAMEFUNCTIONMANAGER_HPP


#include <iostream>
#include <vector>
#include <string>

#include "Player.hpp"
#include "Map.hpp"

using namespace std;

class GameFunctionManager {
private:
    int current_player;
    int current_round;
    int pasch_counter;
    vector<Player> players;
    GameMap map;

    void clear_screen();
    void enableVirtualTerminal();

public:
    GameFunctionManager();
    int randomNumber();

    void addPlayer(Player player);

    void showPlayerInformation(Player player);
    void showMap();
    void showTileInfomation(int tile);

    vector<int> rollDice();
    void buy();
    bool checkPasch(vector<int> dice);


    //Getter
    int getCurrentPlayer();
    vector<Player>& getPlayers();
    int getPaschCounter();
    int getCurrentRound();
    GameMap& getMap();

    //Setter
    void setPlayers(vector<Player> players);
    void setCurrentPlayer(int player);
    void setPaschCounter(int counter);
    void setCurrentRound(int round);
};
#endif