/*
File : GameFunctionManager.hpp 
Description : Header file for GameFunctionManager class

Author : Sami El Aidi 
Date : 2025-05-26

Version : 1.0

Notes: 
 -  This file contains the declaration of the GameFunctionManager class, which manages game functions such as player management, dice rolling, and game state.
 -  The class includes methods for adding players, showing the game map, rolling dice, and managing game rounds.
*/
#pragma once

#include <iostream>
#include <vector>
#include <string>

#include "Player.hpp"

using namespace std;

class GameFunctionManager {
    private:
        int current_player;
        int current_round;
        int pasch_counter;
        vector<Player> players;

        void clear_screen();
        int randomNumber();

    public:
        GameFunctionManager();
        
        void addPlayer(Player player);
        void showMap();
        void showPlayerInventory();
        vector<int> rollDice();
        void buy();
        bool checkPasch(vector<int> dice);


        //Getter
        int getCurrentPlayer();
        vector<Player>& getPlayers();
        int getPaschCounter();
        int getCurrentRound();

        //Setter
        void setCurrentPlayer(int player);
        void setPaschCounter(int counter);
        void setCurrentRound(int round); 
};