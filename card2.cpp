#include "card.h"
#include <iostream>
#include <string>
#include <random>
#include <sstream>
using namespace std;

Card::Card(int uid) : userid(uid), isActive(true) {
    cardnumber = generateCardNumber();
}

string Card::generateCardNumber() {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(0, 9);

    stringstream ss;
    for (int i = 0; i < 16; ++i) {
        ss << dis(gen);
    }
    return ss.str();
}

bool Card::setPin(const string& cardPin) {
    if(cardPin.length() != 4) return false;
    pin = cardPin;
    return true;
}

bool Card::validatePin(const string& cardPin) {
    return isActive && pin == cardPin;
}

string Card::getCardNumber() const {
    return cardnumber;
}