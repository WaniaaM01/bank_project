// Include necessary headers
#include <iostream>     // For input/output operations
#include <string>      // For string handling
#include <vector>      // For dynamic arrays (cards, employees)
#include <algorithm>   // For vector operations (remove, find)
#include "card.h"      // For card management
#include "account_type.cpp"  // For account types (Bronze, Gold, Business)
#include "file_handler.cpp"  // For data persistence
using namespace std;

// Get singleton instance of FileHandler for database operations
FileHandler* fh = FileHandler::getInstance();

// Abstract base class for all user types (Regular Users, Companies, Bank Employees)
class User {
    protected:
        // Protected members accessible to derived classes
        int userid;           // Unique identifier for each user
        string password;      // User's password
        string accountType;   // Account type (Bronze/Gold/Business)
        bool isAccountFrozen; // Account status flag

    public:
        // Constructor - Initialize a new user with default values
        User(){
            userid = 0;               // Default ID
            isAccountFrozen = false;  // Account starts unfrozen
            accountType = "";         // No account type initially
        }
        
        // Virtual destructor for proper cleanup of derived classes
        virtual ~User(){};

        // Basic password validation (can be overridden by derived classes)
        virtual bool validatePassword(const string& pwd) const{
            return pwd.length() >= 8;  // Minimum 8 characters
        }
        
        // Pure virtual function - all derived classes must implement login
        virtual bool login(int id, const string& pwd) = 0;
        
        // Getter for user ID
        int getuserid() const {
            return userid;
        }
};

// Regular User Client Class
class UserClient : public User {
    private:
        double balance;            // Current account balance
        vector<Card*> cards;       // User's bank cards
        bool isApproved;          // Account approval status
        string fullName;          // User's full name
        string address;           // User's address
        string CNIC;              // National ID number
        double currentwithdrawal; // Amount withdrawn today (for limit checking)

        // Validate CNIC format
        bool validateCNIC(const string& cnic) const {
            // CNIC must be exactly 13 digits
            if(cnic.length() != 13) return false;
            
            // Each character must be a digit
            for (char c : cnic){
                if(!isdigit(c)) return false;
            }
            return true;
        }

        // Enhanced password validation
        bool validatePassword(const string& pwd) const {
            if(pwd.length() < 8) return false;  // Minimum length
            
            // Password must contain:
            bool hasUpper = false;  // Uppercase letter
            bool hasLower = false;  // Lowercase letter
            bool hasDigit = false;  // Number
            
            for (char c : pwd){
                if(isupper(c)) hasUpper = true;
                if(islower(c)) hasLower = true;
                if(isdigit(c)) hasDigit = true;
            }
            
            // All requirements must be met
            return hasUpper && hasLower && hasDigit;
        }

    public:
        // Constructor - Initialize a new user client
        UserClient(){
            balance = 0.0;
            isApproved = false;
            isAccountFrozen = false;
            currentwithdrawal = 0.0;
        }

        // Destructor - Clean up allocated card objects
        ~UserClient(){
            for(auto& card : cards){
                delete card;
            }
        }
        // user_type.cpp - Complete implementation of banking system user types


    // Create new account
    bool createAccount(const string& fullName, const string& address, 
                      const string& CNIC, const string& password, 
                      const string& accountType) {
        // Validate all inputs
        if(!validatePassword(password) || !validateCNIC(CNIC)) {
            cout << "Invalid password or CNIC format\n";
            return false;
        }

        // Store user details
        this->fullName = fullName;
        this->address = address;
        this->CNIC = CNIC;
        this->password = password;
        this->accountType = accountType;

        // Add to system through FileHandler
        return fh->addUser(fullName, address, CNIC, password, accountType);
    }

    // Login implementation
    bool login(int id, const string& pwd) override {
        if(!fh->verifyUserClient(id, pwd)) {
            cout << "Invalid credentials\n";
            return false;
        }

        // Load user data
        userid = id;
        password = pwd;
        balance = fh->getUserBalance(id);
        currentwithdrawal = fh->getUserCurrentWithdrawal(id);
        accountType = fh->getUserAccountType(id);
        isAccountFrozen = fh->isUserAccountFrozen(id);
        cards = fh->getUserCards(id);

        return true;
    }

    // Transaction Methods
    bool deposit(double amount) {
        if(!isApproved || isAccountFrozen || amount <= 0) {
            cout << "Invalid deposit attempt\n";
            return false;
        }

        balance += amount;
        if(!fh->updateBalance(userid, balance)) {
            cout << "Deposit failed\n";
            return false;
        }

        // Log transaction
        Transaction trans(userid, amount, "deposit");
        return fh->logTransaction(trans);
    }

    bool withdraw(double amount, const string& cardPin) {
        // Validate withdrawal conditions
        if(!isApproved || isAccountFrozen || amount <= 0 || amount > balance) {
            cout << "Invalid withdrawal attempt\n";
            return false;
        }
        if(!validateCardPin(cardPin)) {
            cout << "Invalid PIN\n";
            return false;
        }

        // Check withdrawal limits based on account type
        bool withinLimit = false;
        if(accountType == "Gold") {
            Gold gold;
            withinLimit = gold.canwithdraw(amount + currentwithdrawal);
        } else if(accountType == "Bronze") {
            Bronze bronze;
            withinLimit = bronze.canwithdraw(amount + currentwithdrawal);
        }

        if(!withinLimit) {
            cout << "Exceeds daily withdrawal limit\n";
            return false;
        }

        // Process withdrawal
        currentwithdrawal += amount;
        balance -= amount;
        
        if(!fh->updateBalance(userid, balance)) {
            cout << "Withdrawal failed\n";
            return false;
        }

        // Log transaction
        Transaction trans(userid, amount, "withdraw");
        return fh->logTransaction(trans);
    }

        // Continuing UserClient class...

    bool transfer(double amount, const string& cardPin, int receiverId) {
        // Validate transfer conditions
        if(!isApproved || isAccountFrozen) {
            cout << "Account not available for transfers\n";
            return false;
        }
        if(amount <= 0 || amount > balance) {
            cout << "Invalid transfer amount\n";
            return false;
        }
        if(!validateCardPin(cardPin)) {
            cout << "Invalid PIN\n";
            return false;
        }
        if(!fh->userExists(receiverId)) {
            cout << "Recipient account not found\n";
            return false;
        }

        // Process transfer
        balance -= amount;
        if(!fh->updateBalance(userid, balance)) {
            cout << "Transfer failed - sender update\n";
            return false;
        }
        if(!fh->addToBalance(receiverId, amount)) {
            // Rollback sender's balance if receiver update fails
            balance += amount;
            fh->updateBalance(userid, balance);
            cout << "Transfer failed - receiver update\n";
            return false;
        }

        // Log transaction
        Transaction trans(userid, amount, "transfer", receiverId);
        return fh->logTransaction(trans);
    }

    // Card Management
    bool requestCard() {
        if(!isApproved || isAccountFrozen) {
            cout << "Cannot request card for inactive account\n";
            return false;
        }
        if(cards.size() >= 3) {
            cout << "Maximum card limit reached\n";
            return false;
        }

        Card* newCard = new Card(userid);
        cards.push_back(newCard);
        return fh->addCard(userid, newCard->getCardNumber(), "");
    }

    bool setCardPin(const string& cardNumber, const string& newPin) {
        for(auto& card : cards) {
            if(card->getCardNumber() == cardNumber) {
                return card->setPin(newPin) && fh->updateCardPin(cardNumber, newPin);
            }
        }
        cout << "Card not found\n";
        return false;
    }

    bool validateCardPin(const string& cardPin) const {
        for(const auto& card : cards) {
            if(card->validatePin(cardPin)) return true;
        }
        return false;
    }

    // Display Methods
    void displayBalance() const {
        cout << fixed << setprecision(2);
        cout << "Current Balance: $" << balance << endl;
        cout << "Daily Withdrawal: $" << currentwithdrawal << endl;
    }

    void displayCards() const {
        if(cards.empty()) {
            cout << "No cards found\n";
            return;
        }
        cout << "\nYour Cards:\n";
        cout << string(30, '-') << endl;
        for(const auto& card : cards) {
            cout << "Card Number: " << card->getCardNumber() << endl;
        }
    }
};

// CompanyClient Class - Handles business accounts with extended features
class CompanyClient : public User {
private:
    string companyName;        // Company's registered name
    string address;           // Company's business address
    int taxNumber;           // Unique tax identification number
    double balance;          // Current account balance
    vector<Card*> cards;     // Company's business cards
    vector<int> employees;   // List of employee IDs linked to company
    bool isApproved;         // Account approval status
    double currentwithdrawal; // Today's total withdrawals

    // Validate tax number format and range
    bool validateTaxNumber(int taxNum) const {
        // Tax number must be positive and within reasonable range
        return taxNum > 0 && taxNum < 1000000000;
    }

public:
    // Constructor: Initialize company account with default values
    CompanyClient() : balance(0.0), isApproved(false), 
                     currentwithdrawal(0.0), taxNumber(0) {}
    
    // Destructor: Clean up dynamically allocated card objects
    ~CompanyClient() {
        for(auto& card : cards) {
            delete card;
        }
    }

    // Create new company account
    bool createAccount(const string& companyName, const string& address, 
                      int taxNumber, const string& password) {
        // Validate credentials before proceeding
        if(!validatePassword(password) || !validateTaxNumber(taxNumber)) {
            cout << "Invalid credentials format\n";
            return false;
        }

        // Store company information
        this->companyName = companyName;
        this->address = address;
        this->taxNumber = taxNumber;
        this->password = password;
        // Companies automatically get Business account type
        this->accountType = "Business";

        // Add company to persistent storage
        return fh->addCompany(companyName, address, taxNumber, password);
    }

    // Authenticate and load company data
    bool login(int id, const string& pwd) override {
        // Verify company credentials
        if(!fh->verifyCompany(id, pwd)) {
            cout << "Invalid company credentials\n";
            return false;
        }

        // Load company data into memory
        userid = id;
        password = pwd;
        balance = fh->getCompanyBalance(id);
        currentwithdrawal = fh->getCompanyCurrentWithdrawal(id);
        isAccountFrozen = fh->isCompanyAccountFrozen(id);
        employees = fh->getCompanyEmployees(id);  // Load linked employees
        cards = fh->getCompanyCards(id);         // Load company cards

        return true;
    }

    // Employee Management Methods
    
    // Add new employee to company
    bool addEmployee(int employeeId) {
        // Check if account is active
        if(!isApproved || isAccountFrozen) {
            cout << "Account not active for employee management\n";
            return false;
        }
        // Check if employee already exists
        if(find(employees.begin(), employees.end(), employeeId) != employees.end()) {
            cout << "Employee already exists\n";
            return false;
        }

        // Add employee to memory and persistent storage
        employees.push_back(employeeId);
        return fh->addCompanyEmployee(userid, employeeId);
    }

    // Remove employee from company
    bool removeEmployee(int employeeId) {
        // Check if account is active
        if(!isApproved || isAccountFrozen) {
            cout << "Account not active for employee management\n";
            return false;
        }

        // Find employee in list
        auto it = find(employees.begin(), employees.end(), employeeId);
        if(it == employees.end()) {
            cout << "Employee not found\n";
            return false;
        }

        // Remove employee from memory and storage
        employees.erase(it);
        return fh->removeCompanyEmployee(userid, employeeId);
    }

    // Business Loan Management
    bool requestLoan(double amount, const string& reason) {
        // Validate account status and loan amount
        if(!isApproved || isAccountFrozen) {
            cout << "Account not eligible for loans\n";
            return false;
        }
        // Check if loan amount is within acceptable range
        if(amount <= 0 || amount > 10000000) { // Example limit of 10M
            cout << "Invalid loan amount\n";
            return false;
        }

        // Submit loan request to system
        return fh->addLoanRequest(userid, amount, reason);
    }
};

// BankEmployee Class - Handles administrative functions
class BankEmployee : public User {
public:
    // Authenticate bank employee
    bool login(int id, const string& pwd) override {
        // Verify employee credentials
        if(!fh->verifyBankEmployee(id, pwd)) {
            cout << "Invalid bank employee credentials\n";
            return false;
        }
        userid = id;
        password = pwd;
        return true;
    }

    // Account Management Functions
    
    // Approve pending account
    bool approveAccount(int clientId) {
        return fh->approveAccount(clientId);
    }

    // Freeze account (disable transactions)
    bool freezeAccount(int clientId) {
        return fh->freezeAccount(clientId);
    }

    // Unfreeze account (enable transactions)
    bool unfreezeAccount(int clientId) {
        return fh->unfreezeAccount(clientId);
    }

    // Loan Management
    bool approveLoan(int loanId, int clientId) {
        return fh->approveLoan(loanId, clientId);
    }

    // View Functions
    
    // Display all accounts in system
    void viewAllAccounts() const {
        fh->displayAllAccounts();
    }

    // Display pending account applications
    void viewPendingAccounts() const {
        fh->displayPendingApplications();
    }
};
