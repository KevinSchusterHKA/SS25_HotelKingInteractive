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
    std::vector<std::string> karten;

public:
    // Konstruktor
    Player(const std::string& name, int startMoney, int playerID);

    // Getter
    std::string getName() const;
    int getMoney() const;
    int getPosition() const;
    bool inPrison() const;
    int getPrisonCount() const;
    int getID() const;
    std::vector<std::string> getKarten() const;

    // Setter / Aktionen
    void addMoney(int amount);
    void setPosition(int field);
    void move(int steps);
    void pay(Player* recipient, int amount);//für mich nicht klar, ob in dem Ojekt Player einzuornen ist
    void setPrison();
    void deductPrisonTime();
    void addKarte(const std::string& karte);
    void removeKarte(const std::string& karte);

    // Debug
    void debug() const;
};

#endif // PLAYER_H
