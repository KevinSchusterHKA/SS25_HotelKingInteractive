/*
File : Menu.hpp 
Description :  Header file for Menu class

Author : Sami El Aidi 
Date : 2025-05-26

Version : 1.0

Notes: 
 - 
*/

#ifndef MENU_HPP
#define MENU_HPP

#include <iostream>
#include <vector>
#include <string>

#include "GameFunctionManager.hpp"

using namespace std;

class Menu {
    private:
        int layer;              // Layer of the menu, used for navigation
        int current_position;  // Current selected position in the menu
        vector<string> menuitems; // Menuentrys
        bool has_submenu; // Show if the menu has a submenu
        string header;
        vector<Menu> submenus; // Submenus for this menu

        void clear_screen();
        void addMenuItem(string item);
        void state_logic(int& current_menu_loc, int dir);

    public:
        Menu();
        Menu(int layer, string header, const vector<string>& items, bool has_submenu);
        void displayMainMenu();
        void displayIngameMenu(GameFunctionManager gameFunctionManager);

        //Getter
        int getCurrentPosition();
        int getLayer();
        bool hasSubmenu();
        vector<Menu>& getSubmenus();
        void addSubmenu(Menu submenu);
        vector<string>& getMenuItems();
        void setCurrentPosition(int position);
        string getHeader();

};
#endif