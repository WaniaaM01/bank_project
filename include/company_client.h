#ifndef COMPANY_CLIENT_H
#define COMPANY_CLIENT_H

#include <string>
#include <vector>
#include <algorithm>  // for remove
#include "user.h"
#include "file_handler.h"
#include "transaction.h"
#include "business_account.h"

class CompanyClient : public User {
private:
    int taxNumber;
    vector<string> cards;
    void handleDeposit();
    void handleWithdrawal();
    void handleTransfer();
    void handleEmployees();
    void handleLoanOperations();
    void viewTransactions();
    void viewBalance();
    void manageCards();
    bool addCard(const string& pin = "");
    void removeCard();
    void viewCards();
    static bool validatePassword(const string& password);

public:
    CompanyClient();
    CompanyClient(int id, const string& name, const string& address, int taxNumber, const string& password);
    
    bool login(int id, const string& password) override;
    void displayMenu() override;
    void createAccount() override;
    
    // Company-specific operations
    bool requestLoan(double amount, const string& reason);
    bool addEmployee(int employeeId);
    bool removeEmployee(int employeeId);
    vector<int> getEmployees() const;
    
    // Getters
    int getTaxNumber() const { return taxNumber; }
};

#endif 