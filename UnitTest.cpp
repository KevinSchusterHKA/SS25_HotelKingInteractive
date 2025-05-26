#include <conio.h>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>
#include <stdlib.h>
#include <vector>
#include <thread>
#include <chrono>

#include "GameFunctionManager.cpp"
#include "Menu.cpp"


using namespace std;

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

void clear_screen() {
    cout << "\x1B[2J\x1B[H";
}

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

void diceroll_test() {
    GameFunctionManager gameFunctionManager(0);

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

void menu_test_2() {
    Menu testmenu(0, {"Start", "Settings", "Exit"}, false);
    testmenu.displayMenu();
}
int main() {

    //diceroll_test();
    menu_test_2();
    
    return 0;
}