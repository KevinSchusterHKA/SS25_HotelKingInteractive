//
// Created by Hanno Ole Meyer on 27.05.25.
//
// Player.cpp

#include "Player.hpp"  // Eigene Headerdatei einbinden

// Konstruktor-Implementierung
Player::Player(const std::string& name, int startMoney, int playerID)
    : name(name), money(startMoney), currentField(0), prisonCount(0), gameover(false), playerID(playerID) {}


std::string Player::getName() const {
    return name;
}

int Player::getMoney() const{
    return money;
}

int Player::getPosition() const {
    return currentField;
}

int Player::getPrisonCount() const {
    return prisonCount;
}

bool Player::getGameOver() const {
    return gameover;
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

void Player::setPrisonCount(int neuPrisonCount) {
    prisonCount = neuPrisonCount;
}

void Player::setGameOver() {
    gameover = true;
}

void Player::deductPrisonTime() {
    if (prisonCount > 0){
        --prisonCount;
    }
}

void Player::addKarte(const std::string& karte) {
    
}

//Noch Nicht gemacht: debug(), pay(), addkarten(), removekarten()







