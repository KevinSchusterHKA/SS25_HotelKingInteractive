#include <iostream>
#include <vector>
#include <string>

using namespace std;

class Menu {
    private:
        int layer;              // Gibt die Ebene an, in der sich das Menü befindet
        int current_position;  // Momentan ausgewählter Menüpunkt
        vector<string> menuitems; // Vektor für die Menüeinträge
        bool has_submenu; // Gibt an, ob ein Untermenü vorhanden ist
        Menu* submenu; // Zeiger auf das Untermenü, falls vorhanden 

        void clear_screen();

    public:
        Menu(int layer, const vector<string>& items, bool has_submenu);
        void displayMenu();
        void addMenuItem(string item);

        //Getter
        int getCurrentPosition();
        int getLayer();
        bool hasSubmenu();
        Menu* getSubmenu();
        vector<string>& getMenuItems();

        //Setter
        void setCurrentPosition(int position);
        void setSubmenu(Menu* submenu);
        void setHasSubmenu(bool has_submenu);
};  