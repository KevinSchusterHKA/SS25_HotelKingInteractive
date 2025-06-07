/*
File : Menu.cpp 
Description : Header file for Menu class

Author : Sami El Aidi 
Date : 2025-05-26

Version : 1.0

Notes: 
 -  This file contains the implementation of the Menu class, which is managed by Menumanager.
 -  The class includes methods for displaying the main menu or ingame menu and managing submenus.
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

#include "Menu.hpp"

using namespace std;;


/*
Clears the console screen.
*/ 
void Menu::clear_screen() { cout << "\x1B[2J\x1B[H";}

Menu::Menu() = default;

/*
Generator funktion to create a new Menu object.
@param layer, the layer where the menu is located (ex.: Startmenu = 1, Submenu = 2);
@param items, the menu items to be displayed;
@param has_submenu, indicates if the menu has a submenu;
*/
Menu::Menu(int layer, string header, const vector<string>& items, bool has_submenu) {
    this -> header = header;
    this -> layer = layer;
    this -> menuitems = items;
    this -> has_submenu = has_submenu;
    vector<Menu> submenus;
}

/*
Display the menu in the console.
This function handles the user input for navigating through the menu.
*/
void Menu::displayMainMenu() {
    clear_screen();
    cout << getHeader() << endl; // Display the header of the menu
    cout << "Zum navigieren im Menü die Pfeiltasten und Enter verwenden." << endl;
    cout << "------------------" << endl;

    for (int i = 0; i < menuitems.size(); i++) {
        if (i == current_position) {
        cout << "> " << getMenuItems()[i] << " <" << endl; // Highlight current position
        } else {
            cout << "  " << getMenuItems()[i] << endl;
        }
    }	 
}

void Menu::displayIngameMenu(GameFunctionManager gameFunctionManager) {
    clear_screen();
    cout << "--------- Runde " << gameFunctionManager.getCurrentRound() << " ----------" << endl;
    cout << "       Player " << (gameFunctionManager.getCurrentPlayer() + 1) << ": " << gameFunctionManager.getPlayers()[gameFunctionManager.getCurrentPlayer()].getName() << endl; 
    cout << "Position: " << gameFunctionManager.getPlayers()[gameFunctionManager.getCurrentPlayer()].getPosition() << "  -  Budget: " << gameFunctionManager.getPlayers()[gameFunctionManager.getCurrentPlayer()].getMoney() << endl;
    cout << "----------------------------" << endl;
    for (int i = 0; i < menuitems.size(); i++) {
        if (i == current_position) {
            cout << "> " << getMenuItems()[i] << " <" << endl; // Highlight current position
        } else {
            cout << "  " << getMenuItems()[i] << endl;
        }
    }
}

/*
Getter function for Menuitems.
@return Vector of all the menuitems.
*/
vector<string>& Menu::getMenuItems() { return menuitems; }

/*
Getter function for the current position.
@return The index of the current position.
*/
int Menu::getCurrentPosition() { return current_position;}

/*
Getter function for the position as a layer.
@return the position as a layer.
*/
int Menu::getLayer() {return layer;}

/*
Returns if there is a submenu.
@return true if there is a submenu.
*/
bool Menu::hasSubmenu() {return has_submenu;}

/*
Getter function for submenu.
@return pointer of the submenu.
*/
vector<Menu>& Menu::getSubmenus() {return submenus;}

/*
Setter for the current position.
@param position, the new position to be set.
*/
void Menu::setCurrentPosition(int position) { this->current_position = position; }

/*
Setter for the submenu.
@param submenu, the new submenu to be set.
*/
void Menu::addSubmenu(Menu submenu) {
    this->submenus.push_back(submenu);
}

string Menu::getHeader() {return header;}



