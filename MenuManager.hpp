/*
File : MenuManager.hpp 
Description :  Header file for MenuManager class

Author : Sami El Aidi 
Date : 2025-05-26

Version : 1.0

Notes: 
 - 
*/
#ifndef MENUMANAGER_HPP
#define MENUMANAGER_HPP

#include <iostream>
#include <vector>
#include <string>
#include <fstream>

#include "Menu.hpp"
#include "GameFunctionManager.hpp"


using namespace std;

class MenuManager { 

    private:
        int current_Layer;
        vector<Menu> menus;
        Menu* current_menu;
        GameFunctionManager* gameFunctionManager;
        ofstream MenuLog;
        bool inGame; 

        //Menu System and Logic Functions
        void clear_screen();
        void enableVirtualTerminal();
        void enableVirtualTerminal();
        void initMenus();
        bool escapeCheck();
        void addMenu(Menu& menu);

        //Menu Functions
        void doOperation(char input);
        void showHighScores();

    public:
        MenuManager();
        
        void handleMenus();

        
        //Setter
        void setGameFunctionManager(GameFunctionManager& gameFunctionManager);
        void setCurrentLayer(int layer);
        void setCurrentMenu(Menu& menu);
        void setInGame(bool inGame);
        
        //Getter
        int getCurrentLayer();
        Menu& getCurrentMenu();
        ofstream& getMenulog();
        bool isInGame();
        GameFunctionManager& getGameFunctionManager();
        vector<Menu>& getMenus();

};
#endif
