#include <iostream>
#include <vector>
#include <string>

#include "Menu.hpp"
#include "GameFunctionManager.hpp"

using namespace std;

class MenuManager { 

    private:
        int current_Layer;
        vector<Menu> menus;
        Menu* current_menu;
        GameFunctionManager gameFunctionManager;

        void clear_screen();

    public:
        MenuManager(int current_Layer, const vector<Menu>& menus);
        void addMenu(const Menu& menu);
        void state_logic(int& current_menu_loc, int dir);
        void displayMenu();
        
        //Setter
        void setCurrentLayer(int layer);
        
        //Getter
        int getCurrentLayer();
        Menu* getCurrentMenu();


};