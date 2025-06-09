#pragma once
#include <string>
using namespace std;

enum class GameMode { RUNDENBASIERT, ZEITBASIERT, BUDGETBASIERT};

struct GameSettings {
    int playerCount = 0;
    int cpuCount = 0;
    int startBudget = 0;
    int roundLimit = 0;
    int timeLimit = 0;
    int budgetLimit = 0;
    string cpuDifficulty;
    GameMode gameMode;
};

struct InfoGame {
    int round;
    int playerID;
    int sup_budget;
    string sup_ownship;
    int position;
};
