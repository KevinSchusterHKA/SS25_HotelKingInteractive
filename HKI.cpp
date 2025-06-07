#include <iostream>
using namespace std;


int main() {
    bool serverRunning = true;

    cout << "======================================\n";
    cout << "        Welcome to MONOPOLY           \n";
    cout << "======================================\n";
    cout << "This program acts as the game server.\n";
    cout << "Use the menu to start or exit.\n";
    cout << "======================================\n";

    while (serverRunning) {
        // === START MENU ===
        // int startSelection = MainMenu();  // (Menu)

        // --- PLACEHOLDER for testing without Menu ---
        cout << "\n[TEST] Enter 1 to start game, 0 to exit program: ";
        int startSelection;
        cin >> startSelection;

        if (startSelection == 1) {
            cout << "\n--- Starting game setup ---\n";

            //Game Menu//

           MainMenu()                       //Funktion von Menu

            cout << "--- Starting game. ---\n";

            // === GAME LOOP ===
            bool gameRunning = true;

            while (gameRunning) {
                // int gameSelection = inGameMenu();  // (Menu)

                // --- PLACEHOLDER for testing without Menu ---
                cout << "\n[TEST] Enter 1 to run next round, 0 to end game: ";
                int gameSelection;
                cin >> gameSelection;

                if (gameSelection == 1) {

                    int round_count = round_count + 1;

                    cout << "[TEST] Simulated game round executed. Current Round: " << round_count <<"\n";

                }
                else if (gameSelection == 0) {
                    cout << "Game manually terminated by user.\n";
                    gameRunning = false;
                }
                else {
                    cout << "Invalid input in game menu.\n";
                }
            }

        }
        else if (startSelection == 0) {
            cout << "Exiting program...\n";
            serverRunning = false;
        }
        else {
            cout << "Invalid input in start menu.\n";
        }
    }

    return 0;
}