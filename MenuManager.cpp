/*
File : MenuManager.cpp
Description : Implementation file for MenuManager class

Author : Sami El Aidi 
Date : 2025-05-26

Version : 1.0

Notes: 
 -  This file contains the implementation of the MenuManager class, which manages the menu system for the Hotel King Interactive game.
 -  The class includes methods for initializing menus, handling menu navigation, and managing game functions.
 -  To use this class, create an instance of MenuManager and call the handleMenus() method to start the menu system.
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

#include "MenuManager.hpp"
#include "Player.hpp"

using namespace std;

/*
Constructor for the MenuManager class.
Initializes the menu system and opens the log file for menu operations.
*/
MenuManager::MenuManager() : current_Layer(0), current_menu(nullptr), MenuLog() {
    this->MenuLog.open("MenuLog.txt", ios::trunc);
    initMenus();
    this->inGame = false;
    this->gameFunctionManager = new GameFunctionManager();
}

/*
Clears the console screen.
*/ 
void MenuManager::clear_screen() { cout << "\x1B[2J\x1B[H";}

/*
Initializes the menu system with a start menu and submenus.
*/
void MenuManager::initMenus() {

    //Main Menu
    Menu startMenu(0, "Hotel King Interactiv", {"Neues Spiel", "Spiel laden", "Highscores", "Exit"}, true);
    Menu newGameMenu(1, "Neues Spiel starten", {"Anzahl Spieler", "Regeln einstellen", "Spiel starten", "Zurück"}, true);
    Menu loadGameMenu(1, "Spiel laden",  {"Spiel laden", "Zurück"}, true);
    Menu highScoresMenu(1, "Highscores - Top 5", {"Zurück"}, false);
    
    //Ingame Overlay Menu
    Menu inGameMenu(0, "Ingame", {"Würfeln", "Handeln und Tauschen", "Inventar anzeigen", "Feld anzeigen", "Spielbrett anzeigen", "Spielstand speichern", "Exit"}, true);

    menus.push_back(startMenu);
    menus.push_back(inGameMenu);

    if (!menus.empty()) current_menu = &menus[0]; 

    getCurrentMenu().addSubmenu(newGameMenu);
    getCurrentMenu().addSubmenu(loadGameMenu);
    getCurrentMenu().addSubmenu(highScoresMenu);

    getMenulog() << "Menu: " << menus[0].getHeader() << " initialized with " << menus[0].getMenuItems().size() << " items." << endl;
    getMenulog() << "Menu: " << menus[1].getHeader() << " initialized with " << menus[1].getMenuItems().size() << " items." << endl;
}

/**
 * Check if the current menu has a submenu and if the current position is not the first one
 * @return true if the last menu item is "Exit" (quit application on Layer 0) if in submenu, the return to the main menu.
 */
bool MenuManager::escapeCheck() {;
    if (getCurrentLayer() == 0 && getCurrentMenu().getMenuItems()[getCurrentMenu().getCurrentPosition()] == "Exit") {
        getMenulog() << "Enter: END" << endl;
        exit(0); // Exit the application
    }  

    if (getCurrentMenu().getCurrentPosition() == getCurrentMenu().getMenuItems().size() - 1) {           
        getMenulog() << "Esc: " << " Current Position: " << getCurrentMenu().getCurrentPosition() << ", Menu Items Size: " << (getCurrentMenu().getMenuItems().size() - 1) << ", Submenu Status: " << getCurrentMenu().hasSubmenu() << endl;
        setCurrentLayer(0);
        setCurrentMenu(menus[0]); 
        return true;
    }
   return false;         
}

/*
Adds a menu to the MenuManager's list of menus.
@param menu The Menu object to be added.
*/
void MenuManager::addMenu(Menu& menu) {menus.push_back(menu);}

/**
 * Controls the input and navigation through the before game (Startmenu, Settings, etc.) menu system. 
 * Simple use of arrow keys for navigation and enter key to select options. 
 * For use, only initialize the MenuManager and call the handleMainMenu() method.
 */
void MenuManager::handleMenus() {
        while (true) {
        getMenulog() << "Ingame: " << (isInGame() ? "Yes" : "No") << " - Current Menu: " << getCurrentMenu().getHeader() << endl;
        if (isInGame()) {
            getCurrentMenu().displayIngameMenu(*gameFunctionManager);
        } else {
            getCurrentMenu().displayMainMenu();
        }
        char input = _getch(); // Get user input
        if (input == 13) { // Entered key
            switch (getCurrentMenu().getCurrentPosition()) {
                /**
                 * Opens the submenu based on the current position, if there is a submenu available.
                 * The order of the cases corresponds to the position in the menu and the position of the submenu in the Submenu vector.
                 * Currently, it only supports up to 3 submenus. Extencion via adding more cases.
                 */
                case 0: 
                    if (escapeCheck()) break;
                    if (getCurrentMenu().hasSubmenu() && getCurrentMenu().getSubmenus().size() > 0) {
                        setCurrentMenu(getCurrentMenu().getSubmenus()[0]); 
                        getCurrentMenu().setCurrentPosition(0); 
                        setCurrentLayer(getCurrentMenu().getLayer() + 1); 
                        getMenulog() << "Enter: " << "Current Menu: " << getCurrentMenu().getHeader() << ", Current Position: " << getCurrentMenu().getCurrentPosition() << ", Current Layer: " << getCurrentLayer() << endl;
                    }
                    break;
                case 1:
                    if (escapeCheck()) break;
                    if (getCurrentMenu().hasSubmenu() && getCurrentMenu().getSubmenus().size() > 1) {
                        setCurrentMenu(getCurrentMenu().getSubmenus()[1]); 
                        getCurrentMenu().setCurrentPosition(0); 
                        setCurrentLayer(getCurrentMenu().getLayer() + 1); 
                        getMenulog() << "Enter: " << "Current Menu: " << getCurrentMenu().getHeader() << ", Current Position: " << getCurrentMenu().getCurrentPosition() << ", Current Layer: " << getCurrentLayer() << endl;

                    }
                    break;
                case 2: 
                    if (escapeCheck()) break;
                    if (getCurrentMenu().hasSubmenu() && getCurrentMenu().getSubmenus().size() > 2) {
                        setCurrentMenu(getCurrentMenu().getSubmenus()[2]);
                        getCurrentMenu().setCurrentPosition(0); 
                        setCurrentLayer(getCurrentMenu().getLayer() + 1); 
                        getMenulog() << "Enter: " << "Current Menu: " << getCurrentMenu().getHeader() << ", Current Position: " << getCurrentMenu().getCurrentPosition() << ", Current Layer: " << getCurrentLayer()<< endl;
                    }
                    break;
                case 3:  
                    if (escapeCheck()) break;
                    if (getCurrentMenu().hasSubmenu()  && getCurrentMenu().getSubmenus().size() > 3) {
                        setCurrentMenu(getCurrentMenu().getSubmenus()[3]); 
                        getCurrentMenu().setCurrentPosition(0); 
                        setCurrentLayer(getCurrentMenu().getLayer() + 1); 
                        getMenulog() << "Enter: " << "Current Menu: " << getCurrentMenu().getHeader() << ", Current Position: " << getCurrentMenu().getCurrentPosition() << ", Current Layer: " << getCurrentLayer() << endl;
                    } 
                    break; 
                case 4:  
                    if (escapeCheck()) break;
                    if (getCurrentMenu().hasSubmenu()  && getCurrentMenu().getSubmenus().size() > 4) {
                        setCurrentMenu(getCurrentMenu().getSubmenus()[4]); 
                        getCurrentMenu().setCurrentPosition(0); 
                        setCurrentLayer(getCurrentMenu().getLayer() + 1); 
                        getMenulog() << "Enter: " << "Current Menu: " << getCurrentMenu().getHeader() << ", Current Position: " << getCurrentMenu().getCurrentPosition() << ", Current Layer: " << getCurrentLayer() << endl;
                    } 
                    break; 
                case 5:  
                    if (escapeCheck()) break;
                    if (getCurrentMenu().hasSubmenu()  && getCurrentMenu().getSubmenus().size() > 5) {
                        setCurrentMenu(getCurrentMenu().getSubmenus()[5]); 
                        getCurrentMenu().setCurrentPosition(0); 
                        setCurrentLayer(getCurrentMenu().getLayer() + 1); 
                        getMenulog() << "Enter: " << "Current Menu: " << getCurrentMenu().getHeader() << ", Current Position: " << getCurrentMenu().getCurrentPosition() << ", Current Layer: " << getCurrentLayer() << endl;
                    } 
                    break; 
                case 6:  
                    if (escapeCheck()) break;
                    if (getCurrentMenu().hasSubmenu()  && getCurrentMenu().getSubmenus().size() > 6) {
                        setCurrentMenu(getCurrentMenu().getSubmenus()[6]); 
                        getCurrentMenu().setCurrentPosition(0); 
                        setCurrentLayer(getCurrentMenu().getLayer() + 1); 
                        getMenulog() << "Enter: " << "Current Menu: " << getCurrentMenu().getHeader() << ", Current Position: " << getCurrentMenu().getCurrentPosition() << ", Current Layer: " << getCurrentLayer() << endl;
                    } 
                    break;         
                default:
                    if (escapeCheck()) break;
                    getMenulog() << "Invalid selection: " << input << ", Current Menu: " << getCurrentMenu().getHeader() << endl;
                    break;
            }
           
        } else if (input == 72 && getCurrentMenu().getCurrentPosition() > 0) { // Up arrow key
            getMenulog() << "Up: " << input << ", Current Menu: " << getCurrentMenu().getHeader() << ", Current Position: " << getCurrentMenu().getCurrentPosition() << endl;

            getCurrentMenu().setCurrentPosition(getCurrentMenu().getCurrentPosition() - 1);

            if (isInGame()) {
                getCurrentMenu().displayIngameMenu(*gameFunctionManager);
            } else {
                getCurrentMenu().displayMainMenu();
            }
            
        } else if (input == 80 && getCurrentMenu().getCurrentPosition() < (getCurrentMenu().getMenuItems().size() - 1)) { // Down arrow key
            getMenulog() << "Down: " << input << ", Current Menu: " << getCurrentMenu().getHeader() << ", Current Position: " << getCurrentMenu().getCurrentPosition() << endl;

            getCurrentMenu().setCurrentPosition(getCurrentMenu().getCurrentPosition() + 1);
            
            if (isInGame()) {
                getCurrentMenu().displayIngameMenu(*gameFunctionManager);
            } else {
                getCurrentMenu().displayMainMenu();
            }
        }
    }	
} 

/*
Gets the GameFunctionManager instance managed by the MenuManager.
@return A reference to the GameFunctionManager instance.
*/
GameFunctionManager& MenuManager::getGameFunctionManager() {return *gameFunctionManager;}

/*
Gets the current layer of the menu.
@return The current layer of the menu.
*/
Menu& MenuManager::getCurrentMenu() {return *current_menu;}

/*
Sets the current layer of the menu.
@param layer The new layer to be set.
*/
void MenuManager::setCurrentMenu(Menu& menu) {current_menu = &menu;}

/*
Returns the log for the menu Operations.
@return The ofstream object for the menu log.
*/
ofstream& MenuManager::getMenulog() { return MenuLog; }

/*
Sets the current layer of the menu.
@param layer The new layer to be set.
*/
void MenuManager::setCurrentLayer(int layer) {current_Layer = layer;}

/*
Returns the current layer of the menu.
@return The current layer of the menu.
*/
int MenuManager::getCurrentLayer() {return current_Layer;}

/*
@return true if the game is currently in progress, false otherwise.
*/
bool MenuManager::isInGame() {return inGame;}

/*
Sets the in-game status of the menu manager.
@param inGame The new in-game status to be set.
*/
void MenuManager::setInGame(bool inGame) {this->inGame = inGame;}

/*
Gets the vector of menus managed by the MenuManager.
@return A reference to the vector of Menu objects.
*/
vector<Menu>& MenuManager::getMenus() {return menus;}

