//
// Created by Hanno Ole Meyer on 22.05.25.
//

#ifndef PLAYER_H
#define PLAYER_H

#include <string>
#include <iostream>
#include <vector>

class Player {
private:
    std::string name;
    int money;
    int currentField;
    bool prison;
    int prisonCount;
    int playerID;
	bool gameover;
    std::vector<std::string> karten;
    bool realPlayer;

public:
    // Konstruktor
    Player(const std::string& name, int startMoney, int playerID, bool realPlayer = true);

    // Getter
    std::string getName() const;
    int getMoney() const;
    int getPosition() const;
    int getPrisonCount() const;
    int getID() const;
    std::vector<std::string> getKarten() const;
    bool getGameOver() const;
    bool isRealPlayer() const;


    // Setter / Aktionen
    void addMoney(int amount);
    void setPosition(int field);
    void move(int steps);
    void pay(Player* recipient, int amount);//für mich nicht klar, ob in dem Ojekt Player einzuornen ist
    void deductPrisonTime();
    void setPrisonCount(int neuPrisonCount);
	void setGameOver();
    void addKarte(const std::string& karte);
    void removeKarte(const std::string& karte);

    // Debug
    void debug() const;
};

#endif // PLAYER_H
