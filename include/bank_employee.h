#ifndef BANK_EMPLOYEE_H
#define BANK_EMPLOYEE_H

#include <string>
#include "user.h"
#include "file_handler.h"

class BankEmployee : public User {
private:
    static const string EMPLOYEE_FILE;
    void handleAccountApprovals();
    void handleLoanRequests();
    void handleAccountFreezing();
    void viewTransactions();
    void viewAllAccounts();

public:
    BankEmployee();
    BankEmployee(int id, const string& name, const string& address, const string& password);
    
    bool login(int id, const string& password) override;
    void displayMenu() override;
    void createAccount() override;
    
    // Bank employee specific operations
    bool approveUserAccount(int userId);
    bool rejectUserAccount(int userId);
    bool freezeUserAccount(int userId);
    bool unfreezeUserAccount(int userId);
    bool approveLoanRequest(int loanId, int companyId);
    bool rejectLoanRequest(int loanId, int companyId);
};

#endif 