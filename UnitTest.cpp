/*
File : UnitTest.cpp 
Description : Test for parts of the Controll-Engine

Author : Sami El Aidi 
Date : 2025-05-26

Version : 1.0a

Notes: 
 - Caution: You need to download files form other branches to run:
    1. feature/player: Player.hpp
    2. feature/player: Player.cpp
 - This file contains test functions for the menu system and game functionality.
 - To run the tests, uncomment the desired test function calls in the main() function.
 - The tests include:
   1. Ingame menu test
   2. Main menu test
   3. Dice rolling test
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
#include <string>

#include "Unittests.hpp"
#include "GameFunctionManager.hpp"
#include "MenuManager.hpp"
#include "Menu.hpp"
#include "Player.hpp"
#include "Map.hpp"
#include "Graph.hpp"
#include "SpecialTile.hpp"
#include "PropertyTile.hpp"


using namespace std;


void IngameMenu_test() {
    MenuManager menumanager = MenuManager();

    menumanager.setInGame(true);
    menumanager.getGameFunctionManager().addPlayer(Player("Tim", 1500, 0));
    menumanager.getGameFunctionManager().addPlayer(Player("Tom", 1500, 1));
    menumanager.getGameFunctionManager().addPlayer(Player("Timmy", 1500, 2));
    menumanager.getGameFunctionManager().addPlayer(Player("Tommy", 1500, 3));
    menumanager.getGameFunctionManager().setCurrentPlayer(0);
    menumanager.getGameFunctionManager().setCurrentRound(6);

    menumanager.setCurrentMenu(menumanager.getMenus()[1]); 

    menumanager.handleMenus();
}
#
/* Test function for the dice rolling functionality
Demonstrates rolling two dice and checking for a pasch (both dice showing the same number). */
void diceroll_test() {
    GameFunctionManager gameFunctionManager = GameFunctionManager();

    vector<int> dice = gameFunctionManager.rollDice();
    cout << "Dice rolled: " << dice[0] << ", " << dice[1] << endl;
    bool pasch = gameFunctionManager.checkPasch(dice);
    cout << "Pasch? " << pasch << endl;

    dice.clear();
    this_thread::sleep_for(chrono::milliseconds(3000));

    dice = gameFunctionManager.rollDice();
    cout << "2) Dice rolled: " << dice[0] << ", " << dice[1] << endl;
    pasch = gameFunctionManager.checkPasch(dice);
    cout << "Pasch? " << pasch << endl;

}

/* Test function for the working main menu
Demonstrates the main menu functionality, allowing navigation through the menu options.
Submenu functionality needs to be implemented */ 
void Menu_test() {
    MenuManager menumanager = MenuManager();
    menumanager.handleMenus();

}

int control_main() {

    // Menu tests

    IngameMenu_test();
    //Menu_test();
    
    //Functionality tests

    //diceroll_test();

    
    return 0;
}


/* First test of the menu system (out of date, but works) 
   Demonstrates the basic functionality of a menu system with navigation.
   Uncomment the code below if you want to test the old menu system.
   Note: Call the menu_test() function in main() to run it.

void state_logic(int& current_menu_loc, int dir) {
    switch(current_menu_loc) {
        case 0:
            current_menu_loc = dir == 1 ? 1 : 5;
            break;
        case 1:
            current_menu_loc = dir == 1 ? 2 : 0;
            break;
        case 2:
            current_menu_loc = dir == 1 ? 3 : 1;
            break;
        case 3:
            current_menu_loc = dir == 1 ? 4 : 2;
            break;
        case 4:
            current_menu_loc = dir == 1 ? 5 : 3;
            break;
        case 5:
            current_menu_loc = dir == 1 ? 0 : 4;
            break;
    }

}
void clear_screen() {cout << "\x1B[2J\x1B[H";}
void print_menu(const vector<string>& menu, int& current_menu_loc) {
    (void)clear_screen();
    for (int i = 0; i < menu.size(); ++i) {
        if (i == current_menu_loc) {
            cout << "> " << menu[i] << endl;
        } else{
            cout << "  " << menu[i] << endl;
        }
    }
    cout << " " << endl;

}
int menu_test() {
    ofstream log("log.txt", ios::trunc);

    vector<string> menu;
    menu.push_back("Test 1");
    menu.push_back("Test 2");
    menu.push_back("Test 3");
    menu.push_back("Test 4");
    menu.push_back("Test 5");
    menu.push_back("Test 6");

    vector<string> submenu;
    submenu.push_back("Test 1.1");
    submenu.push_back("Test 1.2");
    submenu.push_back("Test 1.3");
    submenu.push_back("Test 1.4");
    submenu.push_back("Test 1.5");
    submenu.push_back("Test 1.6");

    log << "Menusize: " << menu.size() << endl;

    int c = 0;
    int current_menu_loc = 0;
    int current_submenu_loc = 0;
    bool submenu_active = false;

    print_menu(menu, current_menu_loc);
    log << "Start: current_menu_loc: "<< current_menu_loc << endl;

    while (true) {
        c = 0;
        switch (c = getch()) {
            case 72: // Pfeil nach oben
                if (submenu_active) {
                    state_logic(current_submenu_loc, 0);
                    print_menu(submenu, current_submenu_loc);
                    log << "Arrow up (" << c << "): submenu_active: " << submenu_active << ", current_submenu_loc: " << current_submenu_loc << endl;
                } else {
                    state_logic(current_menu_loc, 0);
                    print_menu(menu, current_menu_loc);
                    log << "Arrow up (" << c << "): current_menu_loc: "<< current_menu_loc << endl;
                }
                
                break;
            case 80: // Pfeil nach unten
                if (submenu_active) {
                    state_logic(current_submenu_loc, 1);
                    print_menu(submenu, current_submenu_loc);
                    log << "Arrow down (" << c << "): submenu_active: " << submenu_active << ", current_submenu_loc: " << current_submenu_loc << endl;
                } else {
                    state_logic(current_menu_loc, 1);
                    print_menu(menu, current_menu_loc);
                    log << "Arrow down (" << c << "): current_menu_loc: "<< current_menu_loc<< endl;
                }
                
                break;
            case 75: // Pfeil nach links
                if (submenu_active) {
                    submenu_active = false;
                    state_logic(current_menu_loc, 1);
                    print_menu(menu, current_menu_loc);
                    log << "Arrow right (" << c << "): submenu_active: " << submenu_active << ", current_menu_loc: " << current_menu_loc << endl;
                } else {
                    cout << "Bereits im Hauptmenü" << endl;
                    log << "Arrow left (" << c << "): submenu_active: " << submenu_active << ", current_submenu_loc: " << current_submenu_loc << endl;
                }

                log << "Arrow left (" << c << ")" << endl;
                break;
            case 77: // Pfeil nach rechts
                if (!submenu_active) {
                    submenu_active = true;
                    state_logic(current_submenu_loc, 1);
                    print_menu(submenu, current_submenu_loc);
                    log << "Arrow right (" << c << "): submenu_active: " << submenu_active << ", current_submenu_loc: " << current_submenu_loc << endl;
                } else {
                    cout << "Bereits im Submenü" << endl;
                    log << "Arrow right (" << c << "): submenu_active: " << submenu_active << ", current_submenu_loc: " << current_submenu_loc << endl;
                }
                break;
            default:
                if (c == 'q') {
                    cout << "Programm beendet." << endl;
                    return 0;
                } 
                break;
        }   	
    }
    return 0;
}
*/
