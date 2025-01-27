#include "../include/bank_employee.h"
#include "../include/file_handler.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

BankEmployee::BankEmployee() : User() {}

BankEmployee::BankEmployee(int id, const string& name, const string& addr, const string& pass)
    : User(id, name, addr, pass) {}

bool BankEmployee::login(int id, const string& pass) {
    FileHandler fh;
    return fh.verifyBankEmployee(id, pass);
}

vector<string> BankEmployee::viewAllAccounts() {
    vector<string> accounts;
    ifstream userFile("data/userclient.txt");
    ifstream companyFile("data/companies.txt");
    
    if (userFile.is_open()) {
        string line;
        while (getline(userFile, line)) {
            accounts.push_back("User: " + line);
        }
        userFile.close();
    }
    
    if (companyFile.is_open()) {
        string line;
        while (getline(companyFile, line)) {
            accounts.push_back("Company: " + line);
        }
        companyFile.close();
    }
    
    return accounts;
}

bool BankEmployee::approveAccount(int clientId, bool isCompany) {
    FileHandler fh;
    return fh.approveAccount(clientId);
}

bool BankEmployee::rejectAccount(int clientId, bool isCompany) {
    FileHandler fh;
    return fh.rejectAccount(clientId);
}

bool BankEmployee::freezeAccount(int clientId, bool isCompany) {
    FileHandler fh;
    return fh.freezeAccount(clientId);
}

bool BankEmployee::unfreezeAccount(int clientId, bool isCompany) {
    FileHandler fh;
    return fh.unfreezeAccount(clientId);
}

vector<Transaction> BankEmployee::viewTransactionHistory(int clientId) {
    FileHandler fh;
    return fh.getTransactionHistory(clientId);
}

vector<string> BankEmployee::getPendingApplications() {
    vector<string> pendingApps;
    ifstream userFile("data/pending_users.txt");
    ifstream companyFile("data/pending_companies.txt");
    
    if (userFile.is_open()) {
        string line;
        while (getline(userFile, line)) {
            pendingApps.push_back("User: " + line);
        }
        userFile.close();
    }
    
    if (companyFile.is_open()) {
        string line;
        while (getline(companyFile, line)) {
            pendingApps.push_back("Company: " + line);
        }
        companyFile.close();
    }
    
    return pendingApps;
}

vector<string> BankEmployee::getPendingLoans() {
    vector<string> loans;
    ifstream file("data/pending_loans.txt");
    
    if (file.is_open()) {
        string line;
        while (getline(file, line)) {
            loans.push_back(line);
        }
        file.close();
    }
    
    return loans;
}

bool BankEmployee::approveLoan(int loanId, int companyId) {
    FileHandler fh;
    return fh.approveLoan(loanId, companyId);
}

bool BankEmployee::rejectLoan(int loanId, int companyId) {
    FileHandler fh;
    return fh.rejectLoan(loanId, companyId);
}

bool BankEmployee::save() {
    ofstream file("data/admin.txt", ios::app);
    if (!file.is_open()) return false;
    
    file << userId << "," << name << "," << address << "," << password << endl;
    
    file.close();
    return true;
}

bool BankEmployee::load(int id) {
    ifstream file("data/admin.txt");
    if (!file.is_open()) return false;
    
    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        string field;
        vector<string> fields;
        
        while (getline(ss, field, ',')) {
            fields.push_back(field);
        }
        
        if (fields.size() == 4 && stoi(fields[0]) == id) {
            userId = id;
            name = fields[1];
            address = fields[2];
            password = fields[3];
            file.close();
            return true;
        }
    }
    
    file.close();
    return false;
}

bool BankEmployee::update() {
    ifstream inFile("data/admin.txt");
    if (!inFile.is_open()) return false;
    
    ofstream outFile("data/admin_temp.txt");
    if (!outFile.is_open()) {
        inFile.close();
        return false;
    }
    
    string line;
    bool found = false;
    while (getline(inFile, line)) {
        stringstream ss(line);
        string field;
        vector<string> fields;
        
        while (getline(ss, field, ',')) {
            fields.push_back(field);
        }
        
        if (fields.size() == 4 && stoi(fields[0]) == userId) {
            outFile << userId << "," << name << "," << address << "," << password << endl;
            found = true;
        } else {
            outFile << line << endl;
        }
    }
    
    inFile.close();
    outFile.close();
    
    if (found) {
        remove("data/admin.txt");
        rename("data/admin_temp.txt", "data/admin.txt");
        return true;
    }
    
    remove("data/admin_temp.txt");
    return false;
}

bool BankEmployee::deleteAccount() {
    ifstream inFile("data/admin.txt");
    if (!inFile.is_open()) return false;
    
    ofstream outFile("data/admin_temp.txt");
    if (!outFile.is_open()) {
        inFile.close();
        return false;
    }
    
    string line;
    bool found = false;
    while (getline(inFile, line)) {
        stringstream ss(line);
        string field;
        vector<string> fields;
        
        while (getline(ss, field, ',')) {
            fields.push_back(field);
        }
        
        if (fields.size() == 4 && stoi(fields[0]) == userId) {
            found = true;
        } else {
            outFile << line << endl;
        }
    }
    
    inFile.close();
    outFile.close();
    
    if (found) {
        remove("data/admin.txt");
        rename("data/admin_temp.txt", "data/admin.txt");
        return true;
    }
    
    remove("data/admin_temp.txt");
    return false;
}

bool BankEmployee::checkLoginAttempts(int clientId) {
    FileHandler fh;
    int attempts = fh.getLoginAttempts(clientId);
    return attempts < 3;
}

void BankEmployee::resetLoginAttempts(int clientId) {
    FileHandler fh;
    fh.resetLoginAttempts(clientId);
}

bool BankEmployee::isAccountFrozen(int clientId) {
    FileHandler fh;
    return fh.isAccountFrozen(clientId);
}

vector<string> BankEmployee::getAccountCards(int clientId) {
    FileHandler fh;
    return fh.getUserCards(clientId);
}

bool BankEmployee::deactivateCard(const string& cardNumber) {
    FileHandler fh;
    return fh.deactivateCard(cardNumber);
}

double BankEmployee::getAccountBalance(int clientId) {
    FileHandler fh;
    return fh.getBalance(clientId);
}

bool BankEmployee::updateAccountBalance(int clientId, double newBalance) {
    FileHandler fh;
    return fh.updateBalance(clientId, newBalance);
}
