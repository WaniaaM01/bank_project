#ifndef USER_CLIENT_H
#define USER_CLIENT_H

#include "user.h"
#include "file_handler.h"
#include "transaction.h"
#include <vector>

class UserClient : public User {
private:
    string cnic;
    vector<string> cards;
    double balance;
    string status;
    static const string USER_FILE;
    
    void handleDeposit();
    void handleWithdrawal();
    void handleTransfer();
    void handleCardOperations();
    void viewTransactions();
    void viewBalance();
    
    // Helper functions for card operations
    string generateCardNumber() const;
    string generatePin() const;
    bool validateCNIC(const string& cnic) const;
    bool addCard(const string& pin);

public:
    UserClient();
    UserClient(int id, const string& name, const string& address, const string& cnic, const string& password);
    
    // Override virtual functions from User
    bool login(int id, const string& password) override;
    void displayMenu() override;
    void createAccount() override;
    
    // Getters
    string getCNIC() const { return cnic; }
    const vector<string>& getCards() const { return cards; }
    double getBalance() const { return balance; }
    [[nodiscard]] string getStatus() const { return status; }
    
    // Account management
    bool deposit(double amount);
    bool withdraw(double amount);
    bool transfer(int recipientId, double amount);
    
    // View functions
    void viewTransactionHistory();
    
    // Card management
    bool requestCard();
    bool reportCard(const string& cardNumber);
    
    // Utility functions
    static bool validatePassword(const string& password);
};

#endif 