// HKI.cpp : Diese Datei enthält die Funktion "main". Hier beginnt und endet die Ausführung des Programms.
//

#include <iostream>
#include "Unittests.hpp"

using namespace std;

#define GAMEVERSION "00.02"

//#define ACTIVATE_UNIT_TEST_CONTROL
//#define ACTIVATE_UNIT_TEST_CONFIG
//#define ACTIVATE_UNIT_TEST_MAP
//#define ACTIVATE_UNIT_TEST_PLAYER
#define ACTIVATE_UNIT_TEST_SERVER


int main()
{
    cout << "Hotel King Interactive T" << GAMEVERSION << endl;
#ifdef ACTIVATE_UNIT_TEST_CONTROL
    cout << "Testing Engine Control!\n";
    control_main();
#endif

#ifdef ACTIVATE_UNIT_TEST_CONFIG
    cout << "Testing Engine Config!\n";
    config_main();
#endif

#ifdef ACTIVATE_UNIT_TEST_MAP
    cout << "Testing Engine Map!\n";
    map_main();
#endif

#ifdef ACTIVATE_UNIT_TEST_PLAYER
    cout << "Testing Engine Player!\n";
    player_main();
#endif

#ifdef ACTIVATE_UNIT_TEST_SERVER
    cout << "Testing Engine Server!\n";
    server_main();
#endif


    //initial game
    //start game
    //check game results
    //check winner


    cout << "Thanks for playing Hotel King " << GAMEVERSION << endl;
    //end game
}

