//
// Created by Hanno Ole Meyer on 27.05.25.
//
// Player.cpp

#include "Player.hpp"  // Eigene Headerdatei einbinden

// Konstruktor-Implementierung
Player::Player(const std::string& name, int startMoney, int playerID)
    : name(name), money(startMoney), currentField(0), prison(false), prisonCount(0), playerID(playerID) {}


std::string Player::getName() const {
    return name;
}

int Player::getMoney() const{
    return money;
}

int Player::getPosition() const {
    return currentField;
}

bool Player::inPrison() const {
    return prison;
}

int Player::getPrisonCount() const {
    return prisonCount;
}

int Player::getID() const {
    return playerID;
}

std::vector<std::string> Player::getKarten() const {
    return karten;
}

void Player::addMoney(int amount) {
    money += amount;
}

void Player::setPosition(int position) {
    currentField = position;
}

void Player::move(int steps) {
    currentField += steps;
}

void Player::setPrison() {
    prison = true;
    prisonCount = 3;
}

void Player::setPrisonCount(int neuPrisonCount) {
    prisonCount = neuPrisonCount;
}

void Player::deductPrisonTime() {
    if (prisonCount > 0){
        --prisonCount;
    }
}

//Noch Nicht gemacht: debug(), pay(), addkarten(), removekarten()







