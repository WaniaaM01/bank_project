#ifndef CARD_H
#define CARD_H


#include <iostream>
#include <string>
using namespace std;

class Card {
private:
    int userid;
    string cardnumber;
    string pin;
    bool isActive;
public:
    Card(int uid);
    string generateCardNumber();
    bool setPin(const string& cardPin);
    bool validatePin(const string& cardPin);
    string getCardNumber() const;
};

#endif