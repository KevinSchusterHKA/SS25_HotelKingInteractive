/*
File : GameFunctionManager.cpp 
Description : Manager for game functions including dice rolling, buying and managing players.

Author : Sami El Aidi 
Date : 2025-05-26

Version : 1.1

Changes: 
 - Slight changes to the rollDice()-Method:
    - Now Prints the Result in the Terminal

Notes: 
 -  The class handles game-related functions such as rolling dice, checking for a pasch, and managing players.
 -  Doesnt need to be called directly, but is used by the MenuManager to handle game logic.
*/

#include <conio.h>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>
#include <stdlib.h>
#include <vector>
#include <thread>
#include <chrono>
#include <random>
#include <ctime>

#include "GameFunctionManager.hpp"
#include "Player.hpp"

using namespace std;

/*
Clears the console screen.
*/ 
void GameFunctionManager::clear_screen() { cout << "\x1B[2J\x1B[H";}

/*
Private function to generate a random number between 1 and 6.
@return A random number between 1 and 6.
*/
int GameFunctionManager::randomNumber() {
    static mt19937 generator(static_cast<unsigned int>(time(nullptr))); 
    static uniform_int_distribution<int> distribution(1, 6); 
    return distribution(generator);
}

/*
Generator funktion to create a new GameFunctionManager object.
@param current_player The index of the current player. (Normal is 0 at start)
*/
GameFunctionManager::GameFunctionManager() {
    this->current_player = current_player;
    this->pasch_counter = 0; 
    this->current_player = 0; 
    vector<Player> players; 
}

/*
Function to add a player to the game.
Note: Needs a check if the player already exists in the vector.
@param player The Player object to be added.
*/
void GameFunctionManager::addPlayer(Player player) {
    players.push_back(player);
}



/**
*Displays all of the information of the current player.
*@param player The Player object whose information is to be displayed.
*/
void GameFunctionManager::showPlayerInformation(Player player) {
    clear_screen();
    cout << "Player " << (getCurrentPlayer() + 1) << ": " << getPlayers()[getCurrentPlayer()].getName() << endl;
    cout << "Position: " << getPlayers()[getCurrentPlayer()].getPosition() << endl;
    cout << "Budget: " << getPlayers()[getCurrentPlayer()].getMoney() << endl;
    cout << "----------------------------" << endl;
    cout << "Ereigniskarten: ";
    int counter = 1;
    for (auto item : player.getKarten()) {
        cout << counter << ") " << item << endl;
        counter++;
    }
    cout << " " << endl;
    cout << "----------------------------" << endl;
    cout << "Grundstücke: " << endl;
    for (int i = 0; i < 40; i++) {
        PropertyTile* propertyTile = dynamic_cast<PropertyTile*>(getMap().getTile(i).get());
        if (propertyTile && propertyTile->getOwnerId() == player.getID()) {
            cout << propertyTile->getName() << " - Miete: " << propertyTile->getRent() << endl;
        }
    }
}

/**
 * Displays information about the tile at the given index.
 * @param tile The index of the tile to display information for.
 * This function clears the screen and prints the name, owner, price, and rent levels of the tile.
 */
void GameFunctionManager::showTileInfomation(int tile) {
    clear_screen();
    auto& sTile = getMap().tiles[tile];

    if (auto propTile = dynamic_cast<const PropertyTile*>(sTile.get())) {
        cout << "---------" << propTile->getName() << "---------" << endl;
        cout << "Besitzer: " << ((propTile->getOwnerId() == -1) ? "Kein Besitzer" : getPlayers()[propTile->getOwnerId() + 1].getName()) << endl;
        cout << " Preis : " << propTile->getPrice() << endl;
        cout << " Miete: " << endl;
        int counter = 1;
        for (int rent : propTile->getRentLevels()) {
            cout << "       Stufe " << counter << ": " << rent << endl;
            counter++;
        }
    }
    else if (auto specialTile = dynamic_cast<const SpecialTile*>(sTile.get())) {
        std::cout << "  Name: " << specialTile->getName() << "\n"
            << "  Category: " << specialTile->getTypeString() << "\n";
    }
}

/*
Prints the current map of the game in the console.
*/
void GameFunctionManager::showMap() {
    clear_screen();
    cout << "+--------------+-----------+----------+----------+----------+----------+------------+--------------+-----------+--------------+---------------+" << endl;
    cout << "| Gefängnis    | Amaliens. |  E-Werk  | Hirschs. | Kriegss. | West-Bhf | Fastplatz  | Gemeinschaft | Kaiser A. | Durlacher A. | Frei Parken   |" << endl;
    cout << "+--------------+-----------+----------+----------+----------+----------+------------+--------------+-----------+--------------+---------------+" << endl;
    cout << "| Ettlingers.  |                                                                                                              |   Zirkel      |" << endl;
    cout << "+--------------+                                                                                                              +---------------+" << endl;
    cout << "| Rüppurers.   |                                                                                                              | Gemeinschaft  |" << endl;
    cout << "+--------------+                                                                                                              +---------------+" << endl;
    cout << "| Ereignis     |                                                                                                              |    Karlss.    |" << endl;
    cout << "+--------------+                                                                                                              +---------------+" << endl;
    cout << "| Eberts.      |                                                                                                              |   Brauers.    |" << endl;
    cout << "+--------------+                                                                                                              +---------------+" << endl;
    cout << "| Haupt-Bhf    |                                                                                                              |   Ost-Bhf     |" << endl;
    cout << "+--------------+                                                                                                              +---------------+" << endl;
    cout << "|   Steuer     |                                                                                                              | Hildaprom.    |" << endl;
    cout << "+--------------+                                                                                                              +---------------+" << endl;
    cout << "|   Adlers.    |                                                                                                              |   Moltkes.    |" << endl;
    cout << "+--------------+                                                                                                              +---------------+" << endl;
    cout << "| Gemeinschaft |                                                                                                              |    W-Werk     |" << endl;
    cout << "+--------------+                                                                                                              +---------------+" << endl;
    cout << "|   Kronens.   |                                                                                                              | Karl-Friedr.  |" << endl;
    cout << "+--------------+-----------+----------+----------+----------+----------+------------+--------------+-----------+--------------+---------------|" << endl;
    cout << "|    LOS!      | Schlossp. |  Steuer  | Kaisers. | Ereignis | Helipad  | Erbprinzs. | Gemeinschaft |   Walds.  |   Herrens.   |  Ins Gefängis |" << endl;
    cout << "+--------------+-----------+----------+----------+----------+----------+------------+--------------+-----------+--------------+---------------+" << endl;


}

/*
Function to roll the dice and return the result.
The function simulates a dice roll by generating random numbers between 1 and 6.
@return A vector containing the results of the two dice rolls.
*/
vector<int> GameFunctionManager::rollDice() {
    vector<int> dice(2);
    for (int i = 0; i < (randomNumber() * 3); i++) {
        clear_screen();
        int random1 = randomNumber();
        int random2 = randomNumber();

        cout << " ---       --- " << endl;
        cout << "| " << random1 << " |  " << "   | " << random2 << " |" << endl;
        cout << " ---       --- " << endl;
        
        if (i = (rand() & 6 + 10) - 1) {
            dice[0] = random1;
            dice[1] = random2;
        }
    
        this_thread::sleep_for(chrono::milliseconds(500));
    }
    cout << "Du hast eine " << dice[0] << " und eine " << dice[1] << " gewürfelt!" << endl;
    if (checkPasch(dice)) {
        cout << "Du hast einen Pasch gewürfelt! Päsche geworfen: " << pasch_counter << endl;
    } else {
        cout << " " << endl;
    }
    this_thread::sleep_for(chrono::milliseconds(3000));
    getPlayers()[getCurrentPlayer()].move(dice[0] + dice[1]);
    
    return dice;
}

/*
Checks if the rolled dice are a pasch (both dice show the same number).
@param dice A vector containing the results of the two dice rolls.
@return True if the dice are a pasch, false otherwise.
*/
bool GameFunctionManager::checkPasch(vector<int> dice) {
    return (dice[0] == dice[1]);
}

/*
Getter function for the current player.
@return The index of the current player.
*/
int GameFunctionManager::getCurrentPlayer() { return current_player; }

/*
… @param counter The new value for the pasch counter.
 */
void GameFunctionManager::setPaschCounter(int counter) { pasch_counter = counter; }

int  GameFunctionManager::getCurrentRound() { return current_round; }

void GameFunctionManager::setCurrentRound(int round) { current_round = round; }

GameMap& GameFunctionManager::getMap() { return map; }