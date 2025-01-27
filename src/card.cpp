#include "../include/card.h"
#include <sstream>
#include <random>
#include <chrono>

Card::Card() : userId(0), status("active") {}

Card::Card(int userId, const string& cardNumber, const string& pin, const string& status)
    : userId(userId), cardNumber(cardNumber), pin(pin), status(status) {}

int Card::getUserId() const { return userId; }
string Card::getCardNumber() const { return cardNumber; }
string Card::getStatus() const { return status; }

bool Card::validatePin(const string& inputPin) const {
    return pin == inputPin;
}

bool Card::isActive() const {
    return status == "active";
}

string Card::generateCardNumber() {
    // Seed with current time
    unsigned seed = chrono::system_clock::now().time_since_epoch().count();
    mt19937 generator(seed);
    uniform_int_distribution<int> distribution(0, 9);
    
    // Generate 16-digit card number starting with "4532"
    string number = "4532";
    for (int i = 0; i < 12; i++) {
        number += to_string(distribution(generator));
    }
    
    return number;
}

bool Card::validateCardNumber(const string& number) {
    // Check length
    if (number.length() != 16) return false;
    
    // Check if starts with "4532"
    if (number.substr(0, 4) != "4532") return false;
    
    // Check if all digits
    for (char c : number) {
        if (!isdigit(c)) return false;
    }
    
    return true;
}

string Card::toString() const {
    ostringstream oss;
    oss << userId << " " << cardNumber << " " << pin << " " << status;
    return oss.str();
}

Card Card::fromString(const string& str) {
    istringstream iss(str);
    int userId;
    string cardNumber, pin, status;
    
    iss >> userId >> cardNumber >> pin >> status;
    return Card(userId, cardNumber, pin, status);
} 