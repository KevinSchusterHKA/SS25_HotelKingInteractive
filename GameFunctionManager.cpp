/*
File : GameFunctionManager.cpp 
Description : Manager for game functions including dice rolling, buying and managing players.

Author : Sami El Aidi 
Date : 2025-05-26

Version : 1.2

Changes: 
 - Slight changes to the rollDice()-Method:
    - Now Prints the Result in the Terminal

Notes: 
 -  The class handles game-related functions such as rolling dice, checking for a pasch, and managing players.
 -  Doesnt need to be called directly, but is used by the MenuManager to handle game logic.
*/

#include <Windows.h>
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
#include "Map.hpp"

using namespace std;

#define AE "\xC3\xA4"  // ä
#define OE "\xC3\xB6"  // ö
#define UE "\xC3\xBC"  // ü
#define SZ "\xC3\x9F"  // ß

#define BLACK "\x1B[0;30m"
#define RED "\x1B[0;31m"
#define GREEN "\x1B[0;32m"
#define YELLOW "\x1B[0;33m"
#define BLUE "\x1B[0;34m"
#define MAGENTA "\x1B[0;35m"
#define CYAN "\x1B[0;36m"
#define WHITE "\x1B[0;37m"
#define BROWN "\033[33m"
#define PINK "\033[35m"
#define RESET "\x1B[0m"

const string LIGHT_ORANGE = "\033[38;5;214m";

/*
Clears the console screen.
*/ 
void GameFunctionManager::clear_screen() { cout << "\x1B[2J\x1B[H";}

/*
Virtuelle Konsole für Farbausgabe
*/
void GameFunctionManager::enableVirtualTerminal() {
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD dwMode = 0;
    GetConsoleMode(hOut, &dwMode);
    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    SetConsoleMode(hOut, dwMode);
}


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
    current_round;
    pasch_counter; 
    current_player; 
    vector<Player> players; 
    GameMap map; 
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
    cout << "Grundst" + string(UE) + "cke: " << endl;
    for (int i = 0; i < 40; i++) {
        PropertyTile* propertyTile = dynamic_cast<PropertyTile*>(getMap().getTile(i).get());
        if (propertyTile && propertyTile->getOwnerId() == player.getID()) {
            cout << propertyTile->getName() << " - Miete: " <<propertyTile->getRent() << endl;
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
        cout << "Besitzer: " << ((propTile->getOwnerId() == -1) ? "Kein Besitzer" : getPlayers()[propTile->getOwnerId()].getName()) << endl;
        cout << " Preis : " << propTile->getPrice() << endl;
        cout << " Miete: " << propTile->getRentLevels()[0] << endl;
        // Auskommentieren für alle rent levels
		/*int counter = 1;  
        for (int rent : propTile->getRentLevels()) {
            cout << "       Stufe " << counter << ": " << propTile->getRentLevels()[0] << endl;
            counter++;
        }*/
    } else if (auto specialTile = dynamic_cast<const SpecialTile*>(sTile.get())) {
        std::cout << "  Name: " << specialTile->getName() << "\n"
                  << "  Category: " << specialTile->getTypeString() << "\n";
    }
}

/*
Prints the current map of the game in the console.
*/
void GameFunctionManager::showMap() {
	 clear_screen();
	 SetConsoleOutputCP(CP_UTF8);
	enableVirtualTerminal();
	   vector<string> tileNames;
	   tileNames.push_back("        LOS!         ");
	   tileNames.push_back(string(BROWN) + "Kronenstra" + string(SZ) + "e  " + string(WHITE));
	   tileNames.push_back(string(WHITE) + "  Aktionsfeld " + string(WHITE));
	   tileNames.push_back(string(BROWN) + " Adlerstra" + string(SZ) + "e  " + string(WHITE));
	   //tileNames.push_back(string(WHITE) + " Aktionsfeld  " + string(WHITE));
	   tileNames.push_back(string(WHITE) + "   Steuer     " + string(WHITE));
	   tileNames.push_back(string(WHITE) + " Hauptbahnhof " + string(WHITE));
	   tileNames.push_back(string(CYAN) + "  Ebertstra" + string(SZ) + "e " + string(WHITE));
	   tileNames.push_back(string(WHITE) + "  Aktionsfeld " + string(WHITE));
	   tileNames.push_back(string(CYAN) + "R" + string(UE) + "ppurerstra" + string(SZ) + "e" + string(WHITE));
	   tileNames.push_back(string(CYAN) + "Ettlingerstra" + string(SZ) + "e" + string(WHITE));
	   tileNames.push_back(string(WHITE) + "Nur zu Besuch/Gef" + string(AE) + "nis" + string(WHITE));
	   tileNames.push_back(string(PINK) + "  Amalienstra" + string(SZ) + "e " + string(WHITE));
	   tileNames.push_back(string(WHITE) + "E-Werk" + string(WHITE));
	   tileNames.push_back(string(PINK) + "Hirschstra" + string(SZ) + "e" + string(WHITE));
	   tileNames.push_back(string(PINK) + "Kriegstra" + string(SZ) + "e" + string(WHITE));
	   tileNames.push_back(string(WHITE) + "Westbahnhof" + string(WHITE));
	   tileNames.push_back(string(LIGHT_ORANGE) + "  Fastplatz   " + string(WHITE));
	   tileNames.push_back(string(WHITE) + "Aktionsfeld" + string(WHITE));
	   tileNames.push_back(string(LIGHT_ORANGE) + "Kaiserallee" + string(WHITE));
	   tileNames.push_back(string(LIGHT_ORANGE) + "Durlacher Allee" + string(WHITE));
	   tileNames.push_back(string(WHITE) + "Frei Parken" + string(WHITE));
	   tileNames.push_back(string(RED) + "Zirkel" + string(WHITE));
	   tileNames.push_back(string(WHITE) + "Aktionsfeld" + string(WHITE));
	   tileNames.push_back(string(RED) + "Karlsstra" + string(SZ) + "e" + string(WHITE));
	   tileNames.push_back(string(RED) + "Brauersstra" + string(SZ) + "e" + string(WHITE));
	   tileNames.push_back(string(WHITE) + "Ostbahnhof" + string(WHITE));
	   tileNames.push_back(string(YELLOW) + "Hildapromenade" + string(WHITE));
	   tileNames.push_back(string(YELLOW) + "Moltkestra" + string(SZ) + "e" + string(WHITE));
	   //tileNames.push_back(string(WHITE) + "Aktionsfeld" + string(WHITE));
	   tileNames.push_back(string(WHITE) + "Wasserwerk" + string(WHITE));
	   tileNames.push_back(string(YELLOW) + "Karl-F-Stra" + string(SZ) + "e" + string(WHITE));
	   tileNames.push_back(string(WHITE) + "Ins Gef" + string(AE) + "nis" + string(WHITE));
	   tileNames.push_back(string(GREEN) + "  Herrenstra" + string(SZ) + "e " + string(WHITE));
	   tileNames.push_back(string(GREEN) + "Waldstra" + string(SZ) + "e " + string(WHITE));
	   tileNames.push_back(string(WHITE) + "Aktionsfeld" + string(WHITE));
	   tileNames.push_back(string(GREEN) + "Erbprinzstra" + string(SZ) + "e" + string(WHITE));
	   tileNames.push_back(string(WHITE) + "  Helipad  " + string(WHITE));
	   tileNames.push_back(string(WHITE) + "Aktionsfeld" + string(WHITE));
	   tileNames.push_back(string(BLUE) + "Kaiserstra" + string(SZ) + "e" + string(WHITE));
	   tileNames.push_back(string(WHITE) + "Steuer" + string(WHITE));
	   tileNames.push_back(string(BLUE) + "Schlossplatz" + string(WHITE));
	
	   cout << "+-----------------------+------------------+--------+--------------+-------------+-------------+----------------+-------------+-------------+-----------------+--------------+" << endl;
	   cout << "| " << tileNames[10] << " | " << tileNames[11] << " | " << tileNames[12] << " | " << tileNames[13] << " | " << tileNames[14] << " | " << tileNames[15] << " | " << tileNames[16] << " | " << tileNames[17] << " | " << tileNames[18] << " | " << tileNames[19] << " | " << tileNames[20] << "  | " << endl;
	   cout << "+-----------------------+------------------+--------+--------------+-------------+-------------+----------------+-------------+-------------+-----------------+--------------+" << endl;
	   cout << "|    " << tileNames[9] << "    |                                                                                                                                     |    " << tileNames[21] << "    |" << endl;
	   cout << "+-----------------------+                                                                                                                                     +--------------+" << endl;
	   cout << "|    " << tileNames[8] << "     |                                                                                                                                     | " << tileNames[22] << "  |" << endl;
	   cout << "+-----------------------+                                                                                                                                     +--------------+" << endl;
	   cout << "|    " << tileNames[7] << "     |                                                                                                                                     | " << tileNames[23] << "  |" << endl;
	   cout << "+-----------------------+                                                                                                                                     +--------------+" << endl;
	   cout << "|    " << tileNames[6] << "     |                                                                                                                                     |" << tileNames[24] << " |" << endl;
	   cout << "+-----------------------+                                                                                                                                     +--------------+" << endl;
	   cout << "|    " << tileNames[5] << "     |                                                                                                                                     |  " << tileNames[25] << "  |" << endl;
	   cout << "+-----------------------+                                                                                                                                     +--------------+" << endl;
	   cout << "|    " << tileNames[4] << "     |                                                                                                                                     |" << tileNames[26] << "|" << endl;
	   cout << "+-----------------------+                                                                                                                                     +--------------+" << endl;
	   cout << "|    " << tileNames[3] << "     |                                                                                                                                     | " << tileNames[27] << " |" << endl;
	   cout << "+-----------------------+                                                                                                                                     +--------------+" << endl;
	   cout << "|    " << tileNames[2] << "     |                                                                                                                                     | " << tileNames[28] << "   |" << endl;
	   cout << "+-----------------------+                                                                                                                                     +--------------+" << endl;
	   cout << "|    " << tileNames[1] << "     |                                                                                                                                     | " << tileNames[29] << "|" << endl;
	   cout << "+-----------------------+------------------+--------+--------------+-------------+-------------+----------------+-------------+-------------+-----------------+--------------+" << endl;
	   cout << "| " << tileNames[0] << " | " << tileNames[39] << " | " << tileNames[38] << " | " << tileNames[37] << " | " << tileNames[36] << " | " << tileNames[35] << " | " << tileNames[34] << " | " << tileNames[33] << " | " << tileNames[32] << " | " << tileNames[31] << " | " << tileNames[30] << "  |" << endl;
	   cout << "+-----------------------+------------------+--------+--------------+-------------+-------------+----------------+-------------+-------------+-----------------+--------------+" << endl;
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
   // getPlayers()[getCurrentPlayer()].move(dice[0] + dice[1]);

    //showTileInfomation(getPlayers()[getCurrentPlayer()].getPosition());
    cout << "----------------------------" << endl;
    cout << "Du hast eine " << dice[0] << " und eine " << dice[1] << " gew" + string(UE) + "rfelt!" << endl;
    if (checkPasch(dice)) {
        cout << "Du hast einen Pasch gew" + string(UE) + "rfelt! P" + string(AE) + "sche geworfen: " << pasch_counter +1 << endl;
        setPaschCounter(getPaschCounter() + 1);
    } else {
        setPaschCounter(0);
    }
    this_thread::sleep_for(chrono::milliseconds(2000));
    
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
int GameFunctionManager::getCurrentPlayer() {return current_player;}

/*
Getter function for the players vector.abort
@return A pointer to the vector of Player objects.
Note: This function returns a pointer to the vector, allowing direct access to the vector.
*/
vector<Player>& GameFunctionManager::getPlayers() {return players; }

/*
Getter function for the pasch counter.
@return The current value of the pasch counter.
*/
int GameFunctionManager::getPaschCounter() {return pasch_counter;}

/*
 Setter function for the current player.
 @param player The index of the new current player. 
 */
void GameFunctionManager::setCurrentPlayer(int player) {this->current_player = player;}

/*
 Setter function for the pasch counter.
 @param counter The new value for the pasch counter. 
 */
void GameFunctionManager::setPaschCounter(int counter) {pasch_counter = counter;}

int  GameFunctionManager::getCurrentRound() {return current_round;}

void GameFunctionManager::setCurrentRound(int round) {current_round = round;}

GameMap& GameFunctionManager::getMap() {return map;}

void GameFunctionManager::setPlayers(vector<Player> players) { this->players = players; }



