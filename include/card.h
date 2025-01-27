#ifndef CARD_H
#define CARD_H

#include <string>
using namespace std;

class Card {
private:
    int userId;
    string cardNumber;
    string pin;
    string status;  // active, frozen, or cancelled

public:
    Card();
    Card(int userId, const string& cardNumber, const string& pin, const string& status = "active");
    
    // Getters
    int getUserId() const;
    string getCardNumber() const;
    string getStatus() const;
    
    // Card operations
    bool validatePin(const string& inputPin) const;
    bool isActive() const;
    static string generateCardNumber();
    static bool validateCardNumber(const string& number);
    
    // Utility functions
    string toString() const;
    static Card fromString(const string& str);
};

#endif 