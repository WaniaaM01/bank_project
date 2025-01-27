#include "../include/user.h"
#include <fstream>
#include <iostream>

User::User() : id(0), fullName(""), address(""), password("") {}

User::User(int id, const string& fullName, const string& address, const string& password)
    : id(id), fullName(fullName), address(address), password(password) {}

int User::getId() const {
    return id;
}

string User::getFullName() const {
    return fullName;
}

string User::getAddress() const {
    return address;
}

bool User::validatePassword(const string& password) {
    // Password must be at least 6 characters
    if (password.length() < 6) return false;
    
    bool hasLetter = false;
    bool hasNumber = false;
    
    for (char c : password) {
        if (isalpha(c)) hasLetter = true;
        if (isdigit(c)) hasNumber = true;
    }
    
    return hasLetter && hasNumber;
} 