#ifndef USER_H
#define USER_H

#include <string>
using namespace std;

class User {
protected:
    int id;
    string fullName;
    string address;
    string password;

public:
    User();
    User(int id, const string& fullName, const string& address, const string& password);
    virtual ~User() = default;

    // Getters
    int getId() const;
    string getFullName() const;
    string getAddress() const;
    
    // Virtual functions for derived classes
    virtual bool login(int userId, const string& password) = 0;
    virtual void displayMenu() = 0;
    virtual void createAccount() = 0;
    
    // Utility functions
    static bool validatePassword(const string& password);
};

#endif 