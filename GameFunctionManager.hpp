#include <iostream>
#include <vector>
#include <string>

#include "Player.hpp"

using namespace std;

class GameFunctionManager {
    private:
        int current_player;
        int pasch_counter;
        vector<Player> players;

        void clear_screen();
        int randomNumber();

    public:
        GameFunctionManager(int current_player);
        
        void addPlayer(Player player);
        void showMap();
        void showPlayerInventory();
        vector<int> rollDice();
        bool checkPasch(vector<int> dice);


        //Getter
        int getCurrentPlayer();
        vector<Player>* getPlayers();
        int getPaschCounter();

        //Setter
        void setCurrentPlayer(int player);
        void setPaschCounter(int counter);
};