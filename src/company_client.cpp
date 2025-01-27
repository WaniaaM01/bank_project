#include "../include/company_client.h"
#include "../include/file_handler.h"
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>

CompanyClient::CompanyClient() : User() {
    taxNumber = 0;
    balance = 0;
}

CompanyClient::CompanyClient(int id, const string& name, const string& addr, int tax, const string& pass) 
    : User(id, name, addr, pass) {
    taxNumber = tax;
    balance = 0;
}

bool CompanyClient::deposit(double amount) {
    FileHandler fh;
    if (amount <= 0) return false;
    
    double currentBalance = fh.getBalance(userId);
    if (fh.updateBalance(userId, currentBalance + amount)) {
        Transaction trans;
        trans.userId = userId;
        trans.amount = amount;
        trans.type = "deposit";
        return fh.logTransaction(trans);
    }
    return false;
}

bool CompanyClient::withdraw(double amount) {
    FileHandler fh;
    if (amount <= 0) return false;
    
    double currentBalance = fh.getBalance(userId);
    if (currentBalance < amount) return false;
    
    if (!fh.validateDailyLimit(userId, amount)) return false;
    
    if (fh.updateBalance(userId, currentBalance - amount)) {
        Transaction trans;
        trans.userId = userId;
        trans.amount = amount;
        trans.type = "withdraw";
        return fh.logTransaction(trans);
    }
    return false;
}

bool CompanyClient::transfer(int targetId, double amount) {
    FileHandler fh;
    if (amount <= 0 || !fh.userExists(targetId)) return false;
    
    double currentBalance = fh.getBalance(userId);
    if (currentBalance < amount) return false;
    
    if (!fh.validateDailyLimit(userId, amount)) return false;
    
    if (fh.updateBalance(userId, currentBalance - amount)) {
        double targetBalance = fh.getBalance(targetId);
        if (fh.updateBalance(targetId, targetBalance + amount)) {
            Transaction trans;
            trans.userId = userId;
            trans.amount = amount;
            trans.type = "transfer";
            trans.targetUserId = targetId;
            return fh.logTransaction(trans);
        }
        fh.updateBalance(userId, currentBalance);
    }
    return false;
}

bool CompanyClient::requestLoan(double amount, const string& reason) {
    FileHandler fh;
    return fh.addLoanRequest(userId, amount, reason);
}

bool CompanyClient::addEmployee(int employeeId) {
    FileHandler fh;
    if (!fh.userExists(employeeId)) return false;
    return fh.addCompanyEmployee(userId, employeeId);
}

bool CompanyClient::removeEmployee(int employeeId) {
    FileHandler fh;
    return fh.removeCompanyEmployee(userId, employeeId);
}

vector<int> CompanyClient::getEmployees() {
    FileHandler fh;
    return fh.getCompanyEmployees(userId);
}

double CompanyClient::getBalance() {
    FileHandler fh;
    return fh.getBalance(userId);
}

vector<Transaction> CompanyClient::getTransactionHistory() {
    FileHandler fh;
    return fh.getTransactionHistory(userId);
}

vector<string> CompanyClient::getCards() {
    FileHandler fh;
    return fh.getUserCards(userId);
}

bool CompanyClient::addCard(const string& cardNumber, const string& pin) {
    FileHandler fh;
    return fh.addCard(userId, cardNumber, pin);
}

bool CompanyClient::validateCard(const string& cardNumber, const string& pin) {
    FileHandler fh;
    return fh.validateCard(cardNumber, pin);
}

void CompanyClient::setTaxNumber(int tax) {
    taxNumber = tax;
}

int CompanyClient::getTaxNumber() const {
    return taxNumber;
}

bool CompanyClient::save() {
    FileHandler fh;
    ofstream file("data/companies.txt", ios::app);
    if (!file.is_open()) return false;
    
    file << userId << "," << name << "," << address << "," 
         << taxNumber << "," << password << "," << balance << endl;
    
    file.close();
    return true;
}

bool CompanyClient::load(int id) {
    ifstream file("data/companies.txt");
    if (!file.is_open()) return false;
    
    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        string field;
        vector<string> fields;
        
        while (getline(ss, field, ',')) {
            fields.push_back(field);
        }
        
        if (fields.size() == 6 && stoi(fields[0]) == id) {
            userId = id;
            name = fields[1];
            address = fields[2];
            taxNumber = stoi(fields[3]);
            password = fields[4];
            balance = stod(fields[5]);
            file.close();
            return true;
        }
    }
    
    file.close();
    return false;
}

bool CompanyClient::update() {
    ifstream inFile("data/companies.txt");
    if (!inFile.is_open()) return false;
    
    ofstream outFile("data/companies_temp.txt");
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
        
        if (fields.size() == 6 && stoi(fields[0]) == userId) {
            outFile << userId << "," << name << "," << address << "," 
                   << taxNumber << "," << password << "," << balance << endl;
            found = true;
        } else {
            outFile << line << endl;
        }
    }
    
    inFile.close();
    outFile.close();
    
    if (found) {
        remove("data/companies.txt");
        rename("data/companies_temp.txt", "data/companies.txt");
        return true;
    }
    
    remove("data/companies_temp.txt");
    return false;
}

bool CompanyClient::deleteAccount() {
    ifstream inFile("data/companies.txt");
    if (!inFile.is_open()) return false;
    
    ofstream outFile("data/companies_temp.txt");
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
        
        if (fields.size() == 6 && stoi(fields[0]) == userId) {
            found = true;
        } else {
            outFile << line << endl;
        }
    }
    
    inFile.close();
    outFile.close();
    
    if (found) {
        remove("data/companies.txt");
        rename("data/companies_temp.txt", "data/companies.txt");
        return true;
    }
    
    remove("data/companies_temp.txt");
    return false;
}
