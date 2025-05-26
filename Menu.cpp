#include "Menu.hpp"

#include <conio.h>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>
#include <stdlib.h>
#include <vector>
#include <thread>
#include <chrono>

using namespace std;;


/*
Clears the console screen.
*/ 
void Menu::clear_screen() { cout << "\x1B[2J\x1B[H";}


/*
Generator funktion to create a new Menu object.
@param layer, the layer where the menu is located (ex.: Startmenu = 1, Submenu = 2);
@param items, the menu items to be displayed;
@param has_submenu, indicates if the menu has a submenu;
*/
Menu::Menu(int layer, const vector<string>& items, bool has_submenu) {
    this -> layer = layer;
    this -> menuitems = items;
    this -> has_submenu = has_submenu;
}

/*
Display the menu in the console.
This function handles the user input for navigating through the menu.
*/
void Menu::displayMenu() {
    clear_screen();
    cout << "Menu Layer: " << layer << endl;
    cout << "Use arrow keys to navigate, Enter to select, and Esc to exit." << endl;

    for (int i = 0; i < menuitems.size(); i++) {
        if (i == current_position) {
            cout << "> " << menuitems[i] << " <" << endl; // Highlight current position
        } else {
            cout << "  " << menuitems[i] << endl;
        }
    }

    while (true) {
        char input = _getch(); // Get user input

        if (input == 27) { // Escape key
            break; // Exit the menu
        } else if (input == 13) { // Enter key
            if (has_submenu && submenu != nullptr) {
                submenu->displayMenu(); // Display submenu if it exists
            } else {
                cout << "> " << menuitems[current_position] << endl;
                this_thread::sleep_for(chrono::seconds(1)); // Pause for a moment
            }
        } else if (input == 72 && current_position > 0) { // Up arrow key
            current_position--;
        } else if (input == 80 && current_position < menuitems.size() - 1) { // Down arrow key
            current_position++;
        }

        clear_screen();
        displayMenu(); // Redisplay the menu after each input
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
Menu* Menu::getSubmenu() {return submenu;}

/*
Setter for the current position.
@param position, the new position to be set.
*/
void Menu::setCurrentPosition(int position) { this->current_position = position; }

/*
Setter for the submenu.
@param submenu, the new submenu to be set.
*/
void Menu::setSubmenu(Menu* submenu) { this->submenu = submenu; }

/*
setter for has_submenu.
@param has_submenu, the new value to be set.
*/
void Menu::setHasSubmenu(bool has_submenu) { this->has_submenu = has_submenu; };


