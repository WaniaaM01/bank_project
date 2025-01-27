#ifndef FILE_HANDLER_H
#define FILE_HANDLER_H

#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <ctime>
#include <iomanip>
#include <random>
#include <iostream>
#include "transaction.h"

using namespace std;

class Transaction;

class FileHandler {
private:
    static int lastUserId;
    static int lastCompanyId;
    static int lastLoanId;

    struct Loan {
        int loanId;
        int companyId;
        double amount;
        string reason;
        string status;
    };

    struct DailyLimit {
        int userId;
        string date;
        double totalWithdrawn;
    };

    struct PinAttempt {
        int userId;
        string cardNumber;
        int attempts;
        time_t lastAttemptTime;
    };

public:
    // Authentication operations
    static bool verifyBankEmployee(int id, const string& password);
    static bool verifyUserClient(int id, const string& password);
    static bool verifyCompanyClient(int id, const string& password);
    
    // Account creation operations
    static int addUser(const string& name, const string& address, const string& cnic, const string& password);
    static int addCompany(const string& name, const string& address, int taxNumber, const string& password);
    
    // Account management operations
    static bool approveAccount(int clientId);
    static bool rejectAccount(int clientId);
    static bool freezeAccount(int clientId);
    static bool unfreezeAccount(int clientId);
    static bool updateBalance(int userId, double newBalance);
    static double getBalance(int userId);
    static bool isAccountFrozen(int userId);
    
    // Card operations
    static bool addCard(int userId, const string& cardNumber, const string& pin);
    static bool validateCard(const string& cardNumber, const string& pin);
    static bool deactivateCard(const string& cardNumber);
    static vector<string> getUserCards(int userId);
    
    // Transaction operations
    static bool logTransaction(const Transaction& transaction);
    static vector<Transaction> getTransactionHistory(int userId);
    static bool validateDailyLimit(int userId, double amount);
    
    // Loan operations
    static bool approveLoan(int loanId, int companyId);
    static bool rejectLoan(int loanId, int companyId);
    static bool addLoanRequest(int companyId, double amount, const string& reason);
    static vector<string> getPendingLoans();
    
    // Company Employee operations
    static bool addCompanyEmployee(int companyId, int employeeId);
    static bool removeCompanyEmployee(int companyId, int employeeId);
    static bool isEmployeeOfCompany(int companyId, int employeeId);
    static vector<int> getCompanyEmployees(int companyId);
    
    // Display operations
    static void displayAllAccounts();
    static void displayTransactionHistory(int clientId);
    static void displayPendingApplications();
    static void displayPendingLoans();
    
    // Login attempt operations
    static int getLoginAttempts(int userId);
    static bool updateLoginAttempts(int userId, int attempts);
    static bool resetLoginAttempts(int userId);
    
    // Utility operations
    static bool userExists(int userId);
    static bool companyExists(int companyId);

private:
    // File Operations
    static bool openFile(fstream& file, const string& filename, ios_base::openmode mode);
    static bool writeToFile(const string& filename, const string& content, bool append = true);
    static vector<string> readFromFile(const string& filename);
    static bool updateFile(const string& filename, const vector<string>& lines);
    
    // Data Validation
    static bool validateAmount(double amount) { return amount > 0; }
    static bool validateTransactionLimit(int userId, double amount);
    
    // Error Handling
    static void logError(const string& error);
    static bool handleFileError(const string& filename);

    static const string ADMIN_FILE;
    static const string USER_FILE;
    static const string COMPANY_FILE;
    static const string CARD_FILE;
    static const string TRANSACTION_FILE;
    static const string LOAN_FILE;
    static const string COMPANY_EMPLOYEE_FILE;
    static const string PIN_ATTEMPT_FILE;
    
    static bool updateFile(const string& filename, const string& oldLine, const string& newLine);
    static string generateId(const string& filename, int startRange, int endRange);
};

#endif // FILE_HANDLER_H