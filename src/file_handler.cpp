#include "../include/file_handler.h"
#include "../include/error_logger.h"
#include "../include/bronze_account.h"
#include "../include/gold_account.h"
#include "../include/business_account.h"
#include <random>
#include <fstream>
#include <sstream>
#include <iostream>

// Initialize static members
int FileHandler::lastUserId = 1100;
int FileHandler::lastCompanyId = 5000;
int FileHandler::lastLoanId = 1000;

// File paths
const string FileHandler::ADMIN_FILE = "data/admin.txt";
const string FileHandler::USER_FILE = "data/userclient.txt";
const string FileHandler::COMPANY_FILE = "data/companies.txt";
const string FileHandler::CARD_FILE = "data/cards.txt";
const string FileHandler::TRANSACTION_FILE = "data/transactions.txt";
const string FileHandler::LOAN_FILE = "data/loans.txt";
const string FileHandler::COMPANY_EMPLOYEE_FILE = "data/company_employees.txt";
const string FileHandler::PIN_ATTEMPT_FILE = "data/pin_attempts.txt";

// Authentication operations
bool FileHandler::verifyBankEmployee(int id, const string& password) {
    vector<string> lines = readFromFile(ADMIN_FILE);
    for (const string& line : lines) {
        istringstream iss(line);
        int adminId;
        string adminPass;
        iss >> adminId >> adminPass;
        
        if (adminId == id && adminPass == password) {
            resetLoginAttempts(id);
            return true;
        }
    }
    
    int attempts = getLoginAttempts(id) + 1;
    updateLoginAttempts(id, attempts);
    return false;
}

bool FileHandler::verifyUserClient(int id, const string& password) {
    vector<string> lines = readFromFile(USER_FILE);
    for (const string& line : lines) {
        istringstream iss(line);
        int userId;
        string name, address, cnic, pass, balance, status;
        iss >> userId >> name >> address >> cnic >> pass >> balance >> status;
        
        if (userId == id && pass == password && status == "approved") {
            resetLoginAttempts(id);
            return true;
        }
    }
    
    int attempts = getLoginAttempts(id) + 1;
    updateLoginAttempts(id, attempts);
    return false;
}

bool FileHandler::verifyCompanyClient(int id, const string& password) {
    vector<string> lines = readFromFile(COMPANY_FILE);
    for (const string& line : lines) {
        istringstream iss(line);
        int companyId;
        string name, address, taxNum, pass, balance, status;
        iss >> companyId >> name >> address >> taxNum >> pass >> balance >> status;
        
        if (companyId == id && pass == password && status == "approved") {
            resetLoginAttempts(id);
            return true;
        }
    }
    
    int attempts = getLoginAttempts(id) + 1;
    updateLoginAttempts(id, attempts);
    return false;
}

// Account creation operations
int FileHandler::addUser(const string& name, const string& address, const string& cnic, const string& password) {
    lastUserId++;
    ostringstream oss;
    oss << lastUserId << " " << name << " " << address << " " << cnic << " " 
        << password << " 0.00 pending";
    
    if (writeToFile(USER_FILE, oss.str())) {
        return lastUserId;
    }
    return -1;
}

int FileHandler::addCompany(const string& name, const string& address, int taxNumber, const string& password) {
    lastCompanyId++;
    ostringstream oss;
    oss << lastCompanyId << " " << name << " " << address << " " << taxNumber << " " 
        << password << " 0.00 pending";
    
    if (writeToFile(COMPANY_FILE, oss.str())) {
        return lastCompanyId;
    }
    return -1;
}

// Account management operations
bool FileHandler::approveAccount(int clientId) {
    string filename = userExists(clientId) ? USER_FILE : COMPANY_FILE;
    vector<string> lines = readFromFile(filename);
    
    for (size_t i = 0; i < lines.size(); i++) {
        istringstream iss(lines[i]);
        int id;
        string rest;
        iss >> id;
        getline(iss, rest);
        
        if (id == clientId && rest.find("pending") != string::npos) {
            size_t pos = rest.rfind("pending");
            lines[i] = to_string(id) + rest.substr(0, pos) + "approved";
            return updateFile(filename, lines);
        }
    }
    return false;
}

bool FileHandler::rejectAccount(int clientId) {
    string filename = userExists(clientId) ? USER_FILE : COMPANY_FILE;
    vector<string> lines = readFromFile(filename);
    
    for (size_t i = 0; i < lines.size(); i++) {
        istringstream iss(lines[i]);
        int id;
        string rest;
        iss >> id;
        getline(iss, rest);
        
        if (id == clientId && rest.find("pending") != string::npos) {
            size_t pos = rest.rfind("pending");
            lines[i] = to_string(id) + rest.substr(0, pos) + "rejected";
            return updateFile(filename, lines);
        }
    }
    return false;
}

bool FileHandler::freezeAccount(int clientId) {
    string filename = userExists(clientId) ? USER_FILE : COMPANY_FILE;
    vector<string> lines = readFromFile(filename);
    
    for (size_t i = 0; i < lines.size(); i++) {
        istringstream iss(lines[i]);
        int id;
        string rest;
        iss >> id;
        getline(iss, rest);
        
        if (id == clientId && rest.find("approved") != string::npos) {
            size_t pos = rest.rfind("approved");
            lines[i] = to_string(id) + rest.substr(0, pos) + "frozen";
            return updateFile(filename, lines);
        }
    }
    return false;
}

bool FileHandler::unfreezeAccount(int clientId) {
    string filename = userExists(clientId) ? USER_FILE : COMPANY_FILE;
    vector<string> lines = readFromFile(filename);
    
    for (size_t i = 0; i < lines.size(); i++) {
        istringstream iss(lines[i]);
        int id;
        string rest;
        iss >> id;
        getline(iss, rest);
        
        if (id == clientId && rest.find("frozen") != string::npos) {
            size_t pos = rest.rfind("frozen");
            lines[i] = to_string(id) + rest.substr(0, pos) + "approved";
            return updateFile(filename, lines);
        }
    }
    return false;
}

bool FileHandler::validateTransactionLimit(int userId, double amount) {
    if (!validateAmount(amount)) return false;
    
    // Get current date
    time_t now = time(nullptr);
    char dateStr[11];
    strftime(dateStr, sizeof(dateStr), "%Y-%m-%d", localtime(&now));
    
    // Check daily limits
    vector<string> lines = readFromFile(TRANSACTION_FILE);
    double dailyTotal = 0.0;
    
    for (const string& line : lines) {
        Transaction trans(line);
        if (trans.getUserId() == userId && trans.getFormattedDate() == string(dateStr)) {
            if (trans.getType() == "withdraw" || trans.getType() == "transfer") {
                dailyTotal += trans.getAmount();
            }
        }
    }
    
    // Apply account type limits
    if (companyExists(userId)) {
        BusinessAccount account;
        return account.validateWithdrawal(dailyTotal + amount, true);
    } else {
        // Default to Bronze account for regular users
        BronzeAccount account;
        return account.validateWithdrawal(dailyTotal + amount, false);
    }
}

// Add these private file operation functions at the beginning of the implementation file
bool FileHandler::openFile(fstream& file, const string& filename, ios_base::openmode mode) {
    file.open(filename, mode);
    if (!file.is_open()) {
        ErrorLogger::logFileError(filename, "open");
        return false;
    }
    return true;
}

bool FileHandler::writeToFile(const string& filename, const string& content, bool append) {
    fstream file;
    ios_base::openmode mode = ios::out;
    if (append) {
        mode |= ios::app;
    }
    
    if (!openFile(file, filename, mode)) {
        return false;
    }
    
    file << content << endl;
    file.close();
    return true;
}

bool FileHandler::updateFile(const string& filename, const vector<string>& lines) {
    fstream file;
    if (!openFile(file, filename, ios::out | ios::trunc)) {
        return false;
    }
    
    for (const string& line : lines) {
        file << line << endl;
    }
    
    file.close();
    return true;
}

vector<string> FileHandler::readFromFile(const string& filename) {
    vector<string> lines;
    fstream file;
    
    if (!openFile(file, filename, ios::in)) {
        return lines;
    }
    
    string line;
    while (getline(file, line)) {
        if (!line.empty()) {
            lines.push_back(line);
        }
    }
    
    file.close();
    return lines;
}

// Card operations
bool FileHandler::addCard(int userId, const string& cardNumber, const string& pin) {
    if (!userExists(userId)) return false;
    
    ostringstream oss;
    oss << userId << " " << cardNumber << " " << pin << " active";
    return writeToFile(CARD_FILE, oss.str());
}

bool FileHandler::validateCard(const string& cardNumber, const string& pin) {
    vector<string> lines = readFromFile(CARD_FILE);
    for (const string& line : lines) {
        istringstream iss(line);
        int userId;
        string card, storedPin, status;
        iss >> userId >> card >> storedPin >> status;
        
        if (card == cardNumber && storedPin == pin && status == "active") {
            return true;
        }
    }
    return false;
}

bool FileHandler::deactivateCard(const string& cardNumber) {
    vector<string> lines = readFromFile(CARD_FILE);
    bool found = false;
    
    for (size_t i = 0; i < lines.size(); i++) {
        istringstream iss(lines[i]);
        int userId;
        string card, pin, status;
        iss >> userId >> card >> pin >> status;
        
        if (card == cardNumber && status == "active") {
            ostringstream oss;
            oss << userId << " " << card << " " << pin << " deactivated";
            lines[i] = oss.str();
            found = true;
            break;
        }
    }
    
    return found && updateFile(CARD_FILE, lines);
}

vector<string> FileHandler::getUserCards(int userId) {
    vector<string> cards;
    vector<string> lines = readFromFile(CARD_FILE);
    
    for (const string& line : lines) {
        istringstream iss(line);
        int id;
        string cardNumber, pin, status;
        iss >> id >> cardNumber >> pin >> status;
        
        if (id == userId && status == "active") {
            cards.push_back(cardNumber);
        }
    }
    
    return cards;
}

// Loan operations
bool FileHandler::approveLoan(int loanId, int companyId) {
    vector<string> lines = readFromFile(LOAN_FILE);
    bool found = false;
    
    for (size_t i = 0; i < lines.size(); i++) {
        istringstream iss(lines[i]);
        int lid, cid;
        double amount;
        string reason, status;
        iss >> lid >> cid >> amount >> reason >> status;
        
        if (lid == loanId && cid == companyId && status == "pending") {
            ostringstream oss;
            oss << lid << " " << cid << " " << amount << " " << reason << " approved";
            lines[i] = oss.str();
            found = true;
            
            // Update company balance
            double currentBalance = getBalance(companyId);
            updateBalance(companyId, currentBalance + amount);
            break;
        }
    }
    
    return found && updateFile(LOAN_FILE, lines);
}

bool FileHandler::rejectLoan(int loanId, int companyId) {
    vector<string> lines = readFromFile(LOAN_FILE);
    bool found = false;
    
    for (size_t i = 0; i < lines.size(); i++) {
        istringstream iss(lines[i]);
        int lid, cid;
        double amount;
        string reason, status;
        iss >> lid >> cid >> amount >> reason >> status;
        
        if (lid == loanId && cid == companyId && status == "pending") {
            ostringstream oss;
            oss << lid << " " << cid << " " << amount << " " << reason << " rejected";
            lines[i] = oss.str();
            found = true;
            break;
        }
    }
    
    return found && updateFile(LOAN_FILE, lines);
}

bool FileHandler::addLoanRequest(int companyId, double amount, const string& reason) {
    if (!companyExists(companyId) || !validateAmount(amount)) return false;
    
    lastLoanId++;
    ostringstream oss;
    oss << lastLoanId << " " << companyId << " " << fixed << setprecision(2) 
        << amount << " " << reason << " pending";
    
    return writeToFile(LOAN_FILE, oss.str());
}

vector<string> FileHandler::getPendingLoans() {
    vector<string> pendingLoans;
    vector<string> lines = readFromFile(LOAN_FILE);
    
    for (const string& line : lines) {
        if (line.find("pending") != string::npos) {
            pendingLoans.push_back(line);
        }
    }
    
    return pendingLoans;
}

// Company Employee operations
bool FileHandler::addCompanyEmployee(int companyId, int employeeId) {
    if (!companyExists(companyId) || !userExists(employeeId)) return false;
    
    ostringstream oss;
    oss << companyId << " " << employeeId;
    return writeToFile(COMPANY_EMPLOYEE_FILE, oss.str());
}

bool FileHandler::removeCompanyEmployee(int companyId, int employeeId) {
    vector<string> lines = readFromFile(COMPANY_EMPLOYEE_FILE);
    vector<string> newLines;
    bool found = false;
    
    for (const string& line : lines) {
        istringstream iss(line);
        int cid, eid;
        iss >> cid >> eid;
        
        if (cid == companyId && eid == employeeId) {
            found = true;
        } else {
            newLines.push_back(line);
        }
    }
    
    return found && updateFile(COMPANY_EMPLOYEE_FILE, newLines);
}

bool FileHandler::isEmployeeOfCompany(int companyId, int employeeId) {
    vector<string> lines = readFromFile(COMPANY_EMPLOYEE_FILE);
    
    for (const string& line : lines) {
        istringstream iss(line);
        int cid, eid;
        iss >> cid >> eid;
        
        if (cid == companyId && eid == employeeId) {
            return true;
        }
    }
    
    return false;
}

vector<int> FileHandler::getCompanyEmployees(int companyId) {
    vector<int> employees;
    vector<string> lines = readFromFile(COMPANY_EMPLOYEE_FILE);
    
    for (const string& line : lines) {
        istringstream iss(line);
        int cid, eid;
        iss >> cid >> eid;
        
        if (cid == companyId) {
            employees.push_back(eid);
        }
    }
    
    return employees;
}

// Display operations
void FileHandler::displayAllAccounts() {
    cout << "\n=== User Accounts ===\n";
    vector<string> users = readFromFile(USER_FILE);
    for (const string& line : users) {
        istringstream iss(line);
        int id;
        string name, address, cnic, pass, balance, status;
        iss >> id >> name >> address >> cnic >> pass >> balance >> status;
        cout << "ID: " << id << " | Name: " << name << " | Status: " << status << endl;
    }
    
    cout << "\n=== Company Accounts ===\n";
    vector<string> companies = readFromFile(COMPANY_FILE);
    for (const string& line : companies) {
        istringstream iss(line);
        int id;
        string name, address, taxNum, pass, balance, status;
        iss >> id >> name >> address >> taxNum >> pass >> balance >> status;
        cout << "ID: " << id << " | Name: " << name << " | Status: " << status << endl;
    }
}

void FileHandler::displayTransactionHistory(int clientId) {
    vector<Transaction> transactions = getTransactionHistory(clientId);
    cout << "\n=== Transaction History ===\n";
    cout << "Date       | Type     | Amount    | Details\n";
    cout << "----------------------------------------\n";
    
    for (const Transaction& trans : transactions) {
        cout << trans.getFormattedDate() << " | "
             << setw(8) << left << trans.getType() << " | "
             << "$" << fixed << setprecision(2) << setw(8) << right << trans.getAmount() << " | ";
        
        if (trans.getType() == "transfer") {
            cout << "To: " << trans.getRecipientId();
        }
        cout << endl;
    }
}

void FileHandler::displayPendingApplications() {
    cout << "\n=== Pending User Applications ===\n";
    vector<string> users = readFromFile(USER_FILE);
    for (const string& line : users) {
        if (line.find("pending") != string::npos) {
            istringstream iss(line);
            int id;
            string name, address, cnic, pass, balance, status;
            iss >> id >> name >> address >> cnic >> pass >> balance >> status;
            cout << "ID: " << id << " | Name: " << name << " | CNIC: " << cnic << endl;
        }
    }
    
    cout << "\n=== Pending Company Applications ===\n";
    vector<string> companies = readFromFile(COMPANY_FILE);
    for (const string& line : companies) {
        if (line.find("pending") != string::npos) {
            istringstream iss(line);
            int id;
            string name, address, taxNum, pass, balance, status;
            iss >> id >> name >> address >> taxNum >> pass >> balance >> status;
            cout << "ID: " << id << " | Name: " << name << " | Tax Number: " << taxNum << endl;
        }
    }
}

void FileHandler::displayPendingLoans() {
    cout << "\n=== Pending Loan Requests ===\n";
    vector<string> loans = getPendingLoans();
    for (const string& loan : loans) {
        istringstream iss(loan);
        int loanId, companyId;
        double amount;
        string reason, status;
        iss >> loanId >> companyId >> amount >> reason >> status;
        cout << "Loan ID: " << loanId << " | Company ID: " << companyId 
             << " | Amount: $" << fixed << setprecision(2) << amount 
             << " | Reason: " << reason << endl;
    }
}

// Login attempt operations
int FileHandler::getLoginAttempts(int userId) {
    vector<string> lines = readFromFile(PIN_ATTEMPT_FILE);
    for (const string& line : lines) {
        istringstream iss(line);
        int id;
        string cardNumber;
        int attempts;
        time_t lastAttempt;
        iss >> id >> cardNumber >> attempts >> lastAttempt;
        
        if (id == userId) {
            // Reset attempts if more than 24 hours have passed
            time_t now = time(nullptr);
            if (now - lastAttempt > 86400) {
                updateLoginAttempts(userId, 0);
                return 0;
            }
            return attempts;
        }
    }
    return 0;
}

bool FileHandler::updateLoginAttempts(int userId, int attempts) {
    vector<string> lines = readFromFile(PIN_ATTEMPT_FILE);
    bool found = false;
    time_t now = time(nullptr);
    
    for (size_t i = 0; i < lines.size(); i++) {
        istringstream iss(lines[i]);
        int id;
        string cardNumber;
        int oldAttempts;
        time_t lastAttempt;
        iss >> id >> cardNumber >> oldAttempts >> lastAttempt;
        
        if (id == userId) {
            ostringstream oss;
            oss << id << " " << cardNumber << " " << attempts << " " << now;
            lines[i] = oss.str();
            found = true;
            break;
        }
    }
    
    if (!found) {
        ostringstream oss;
        oss << userId << " none " << attempts << " " << now;
        lines.push_back(oss.str());
    }
    
    return updateFile(PIN_ATTEMPT_FILE, lines);
}

bool FileHandler::resetLoginAttempts(int userId) {
    return updateLoginAttempts(userId, 0);
}

// Utility operations
bool FileHandler::userExists(int userId) {
    vector<string> lines = readFromFile(USER_FILE);
    for (const string& line : lines) {
        istringstream iss(line);
        int id;
        iss >> id;
        if (id == userId) return true;
    }
    return false;
}

bool FileHandler::companyExists(int companyId) {
    vector<string> lines = readFromFile(COMPANY_FILE);
    for (const string& line : lines) {
        istringstream iss(line);
        int id;
        iss >> id;
        if (id == companyId) return true;
    }
    return false;
}