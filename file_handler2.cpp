#include <fstream>      // File operations
#include <ctime>        // Time operations
#include <sstream>      // String stream
#include <filesystem>   // File system operations
#include <iostream>     // Input/output
#include <iomanip>      // Output formatting
#include <string>       // String operations
#include <vector>       // Dynamic arrays
#include <algorithm>    // Algorithms like remove_if
#include "transaction.cpp"
#include "card.cpp"

class FileHandler {
    static FileHandler* instance;  // Singleton pattern instance

    // Constants for file names
    const string USERS_FILE = "users.txt";
    const string COMPANIES_FILE = "companies.txt";
    const string CARDS_FILE = "cards.txt";
    const string TRANSACTIONS_FILE = "transactions.txt";
    const string LOANS_FILE = "loans.txt";
    const string EMPLOYEES_FILE = "employees.txt";
    const string PENDING_ACCOUNTS_FILE = "pending_accounts.txt";
    const string BANK_EMPLOYEES_FILE = "bank_employees.txt";

public:
    // Constructor and destructor
    FileHandler() {};
    ~FileHandler();

    // Singleton pattern getter
    static FileHandler* getInstance();


    // Read all lines from a file
    vector<string> readFileLines(const string& filename) {
        vector<string> lines;
        ifstream file(filename);
        string line;
    
        if (file.is_open()) {
            while (getline(file, line)) {
                lines.push_back(line);
            }
        }
        file.close();
        return lines;
    }

    // Write lines to a file
    bool writeFileLines(const string& filename, const vector<string>& lines) {
        ofstream file(filename);
        if (!file.is_open()) return false;
    
        for (const auto& line : lines) {
            file << line << endl;
        }
        file.close();
        return true;
    }

    // Append a single line to a file
    bool appendFileLine(const string& filename, const string& line) {
        ofstream file(filename, ios::app);  // Open in append mode
        if (!file.is_open()) return false;
    
        file << line << endl;
        file.close();
        return true;
    }

    // Generate new ID for records
    int generateNewId(const string& filename) {
        auto lines = readFileLines(filename);
        int maxId = 0;
    
        for (const auto& line : lines) {
            istringstream iss(line);
            int id;
            if (iss >> id) {
                maxId = max(maxId, id);
            }
        }
        return maxId + 1;
    
    }
       // Add new user to system
    bool addUser(const string& name, const string& address, const string& cnic, 
                const string& password, const string& accountType) {
        int id = generateNewId(USERS_FILE);
        stringstream ss;
        // Format: ID|Name|Address|CNIC|Password|Balance|Status|AccountType|WithdrawalLimit
        ss << id << "|" << name << "|" << address << "|" << cnic << "|" 
           << password << "|0.0|pending|" << accountType << "|0.0";
    
        if (!appendFileLine(USERS_FILE, ss.str())) {
            return false;
        }
        return appendFileLine(PENDING_ACCOUNTS_FILE, to_string(id));
    }

    // Verify user credentials
    bool verifyUserClient(int id, const string& password) {
        auto lines = readFileLines(USERS_FILE);
        for (const auto& line : lines) {
            vector<string> fields;
            stringstream ss(line);
            string field;
            
            while (getline(ss, field, '|')) {
                fields.push_back(field);
            }
            
            if (fields.size() >= 5 && stoi(fields[0]) == id) {
                return fields[4] == password;
            }
        }
        return false;
    }
    // Update account balance
    bool updateBalance(int userId, double newBalance) {
        auto lines = readFileLines(USERS_FILE);
        bool found = false;
        
        for (auto& line : lines) {
            vector<string> fields;
            stringstream ss(line);
            string field;
            
            // Split line by '|' delimiter
            while (getline(ss, field, '|')) {
                fields.push_back(field);
            }
            
            // Update balance if user found
            if (fields.size() >= 6 && stoi(fields[0]) == userId) {
                fields[5] = to_string(newBalance);
                // Reconstruct the line with updated balance
                stringstream newLine;
                for (size_t i = 0; i < fields.size(); ++i) {
                    newLine << fields[i];
                    if (i < fields.size() - 1) newLine << "|";
                }
                line = newLine.str();
                found = true;
                break;
            }
        }
        return found && writeFileLines(USERS_FILE, lines);
    }

    // Approve pending account
    bool approveAccount(int clientId) {
        auto lines = readFileLines(USERS_FILE);
        bool found = false;
        
        // Update account status to active
        for (auto& line : lines) {
            vector<string> fields;
            stringstream ss(line);
            string field;
            
            while (getline(ss, field, '|')) {
                fields.push_back(field);
            }
            
            if (fields.size() >= 7 && stoi(fields[0]) == clientId) {
                fields[6] = "active";  // Change status to active
                stringstream newLine;
                for (size_t i = 0; i < fields.size(); ++i) {
                    newLine << fields[i];
                    if (i < fields.size() - 1) newLine << "|";
                }
                line = newLine.str();
                found = true;
                break;
            }
        }
        
        // Remove from pending accounts
        if (found) {
            auto pendingLines = readFileLines(PENDING_ACCOUNTS_FILE);
            // Remove client ID from pending list
            pendingLines.erase(
                remove_if(pendingLines.begin(), pendingLines.end(),
                    [clientId](const string& line) {
                        return stoi(line) == clientId;
                    }
                ),
                pendingLines.end()
            );
            
            // Update both files
            return writeFileLines(USERS_FILE, lines) && 
                   writeFileLines(PENDING_ACCOUNTS_FILE, pendingLines);
        }
        
        return false;
    }
bool freezeAccount(int userId) {
    auto lines = readFileLines(USERS_FILE);
    bool found = false;
    
    for (auto& line : lines) {
        istringstream iss(line);
        vector<string> fields;
        string field;
        
        while (getline(iss, field, '|')) {
            fields.push_back(field);
        }
        
        if (fields.size() >= 7 && stoi(fields[0]) == userId) {
            fields[6] = "frozen";
            stringstream newLine;
            for (size_t i = 0; i < fields.size(); ++i) {
                newLine << fields[i];
                if (i < fields.size() - 1) newLine << "|";
            }
            line = newLine.str();
            found = true;
            break;
        }
    }
    
    return found && writeFileLines(USERS_FILE, lines);
}
bool unfreezeAccount(int userId) {
    auto lines = readFileLines(USERS_FILE);
    bool found = false;
    
    for (auto& line : lines) {
        istringstream iss(line);
        vector<string> fields;
        string field;
        
        while (getline(iss, field, '|')) {
            fields.push_back(field);
        }
        
        if (fields.size() >= 7 && stoi(fields[0]) == userId) {
            fields[6] = "active";
            stringstream newLine;
            for (size_t i = 0; i < fields.size(); ++i) {
                newLine << fields[i];
                if (i < fields.size() - 1) newLine << "|";
            }
            line = newLine.str();
            found = true;
            break;
        }
    }
    
    return found && writeFileLines(USERS_FILE, lines);
}
    // Add new card to system
    bool addCard(int userId, const string& cardNumber, const string& pin) {
        stringstream ss;
        ss << userId << "|" << cardNumber << "|" << pin << "|active";
        return appendFileLine(CARDS_FILE, ss.str());
    }

    // Validate card credentials
    bool validateCard(const string& cardNumber, const string& pin) {
        auto lines = readFileLines(CARDS_FILE);
        
        for (const auto& line : lines) {
            vector<string> fields;
            stringstream ss(line);
            string field;
            
            while (getline(ss, field, '|')) {
                fields.push_back(field);
            }
            
            // Check if card exists and is active
            if (fields.size() >= 4 && 
                fields[1] == cardNumber && 
                fields[2] == pin && 
                fields[3] == "active") {
                return true;
            }
        }
        
        return false;
    }
    // Log new transaction
    bool logTransaction(const string& transactionDetails) {
        return appendFileLine(TRANSACTIONS_FILE, transactionDetails);
    }

    // Get transaction history for a user
    vector<Transaction> viewTransactionHistory(int userId) {
        vector<Transaction> transactions;
        auto lines = readFileLines(TRANSACTIONS_FILE);
        
        for (const auto& line : lines) {
            vector<string> fields;
            stringstream ss(line);
            string field;
            
            while (getline(ss, field, '|')) {
                fields.push_back(field);
            }
            
            // Create Transaction object if record belongs to user
            if (fields.size() > 5 && stoi(fields[1]) == userId) {
                Transaction trans(
                    stoi(fields[0]),    // transaction ID
                    stod(fields[1]),    // amount
                    fields[2],          // transaction type
                    stoi(fields[3])     // recipient ID (if transfer)
                );
                transactions.push_back(trans);
            }
        }
        
        return transactions;
    }
    // Add new loan request
    bool addLoanRequest(int companyId, double amount, const string& reason) {
        int loanId = generateNewId(LOANS_FILE);
        stringstream ss;
        // Format: LoanID|CompanyID|Amount|Reason|Status
        ss << loanId << "|" << companyId << "|" 
           << fixed << setprecision(2) << amount << "|"
           << reason << "|pending";
        
        return appendFileLine(LOANS_FILE, ss.str());
    }

    // Approve a pending loan
    bool approveLoan(int loanId, int companyId) {
        auto lines = readFileLines(LOANS_FILE);
        bool found = false;
        
        for (auto& line : lines) {
            vector<string> fields;
            stringstream ss(line);
            string field;
            
            while (getline(ss, field, '|')) {
                fields.push_back(field);
            }
            
            // Update loan status if found
            if (fields.size() >= 5 && 
                stoi(fields[0]) == loanId && 
                stoi(fields[1]) == companyId) {
                fields[4] = "approved";
                stringstream newLine;
                for (size_t i = 0; i < fields.size(); ++i) {
                    newLine << fields[i];
                    if (i < fields.size() - 1) newLine << "|";
                }
                line = newLine.str();
                found = true;
                break;
            }
        }
        
        return found && writeFileLines(LOANS_FILE, lines);
    }
bool rejectLoan(int loanId, int companyId) {
    auto lines = readFileLines(LOANS_FILE);
    bool found = false;
    
    for (auto& line : lines) {
        istringstream iss(line);
        vector<string> fields;
        string field;
        
        while (getline(iss, field, '|')) {
            fields.push_back(field);
        }
        
        if (fields.size() >= 5 && 
            stoi(fields[0]) == loanId && 
            stoi(fields[1]) == companyId) {
            fields[4] = "rejected";
            stringstream newLine;
            for (size_t i = 0; i < fields.size(); ++i) {
                newLine << fields[i];
                if (i < fields.size() - 1) newLine << "|";
            }
            line = newLine.str();
            found = true;
            break;
        }
    }
    
    return found && writeFileLines(LOANS_FILE, lines);
}
vector<string> getPendingLoans() {
    vector<string> pendingLoans;
    auto lines = readFileLines(LOANS_FILE);
    
    for (const auto& line : lines) {
        istringstream iss(line);
        vector<string> fields;
        string field;
        
        while (getline(iss, field, '|')) {
            fields.push_back(field);
        }
        
        if (fields.size() >= 5 && fields[4] == "pending") {
            pendingLoans.push_back(line);
        }
    }
    
    return pendingLoans;
}
    // Add employee to company
    bool addCompanyEmployee(int companyId, int employeeId) {
        stringstream ss;
        ss << companyId << "|" << employeeId;
        return appendFileLine(EMPLOYEES_FILE, ss.str());
    }

    // Remove employee from company
    bool removeCompanyEmployee(int companyId, int employeeId) {
        auto lines = readFileLines(EMPLOYEES_FILE);
        // Remove matching employee record
        lines.erase(
            remove_if(lines.begin(), lines.end(),
                [companyId, employeeId](const string& line) {
                    vector<string> fields;
                    stringstream ss(line);
                    string field;
                    while (getline(ss, field, '|')) {
                        fields.push_back(field);
                    }
                    return fields.size() >= 2 && 
                           stoi(fields[0]) == companyId && 
                           stoi(fields[1]) == employeeId;
                }
            ),
            lines.end()
        );
        
        return writeFileLines(EMPLOYEES_FILE, lines);
    }
bool isEmployeeOfCompany(int companyId, int employeeId) {
    auto lines = readFileLines(EMPLOYEES_FILE);
    
    for (const auto& line : lines) {
        istringstream iss(line);
        vector<string> fields;
        string field;
        
        while (getline(iss, field, '|')) {
            fields.push_back(field);
        }
        
        if (fields.size() >= 2 && stoi(fields[0]) == companyId && stoi(fields[1]) == employeeId) {
            return true;
        }
    }
    
    return false;
}
    // Display all accounts in system
    void displayAllAccounts() {
        auto lines = readFileLines(USERS_FILE);
        cout << "\nAll Accounts:\n";
        cout << setw(5) << "ID" << " | " 
             << setw(20) << "Name" << " | "
             << setw(10) << "Balance" << " | "
             << "Status\n";
        cout << string(50, '-') << "\n";
        
        for (const auto& line : lines) {
            vector<string> fields;
            stringstream ss(line);
            string field;
            
            while (getline(ss, field, '|')) {
                fields.push_back(field);
            }
            
            if (fields.size() >= 7) {
                cout << setw(5) << fields[0] << " | "    // ID
                     << setw(20) << fields[1] << " | "   // Name
                     << setw(10) << fields[5] << " | "   // Balance
                     << fields[6] << "\n";               // Status
            }
        }
    }

    // Display pending account applications
    void displayPendingApplications() {
        auto lines = readFileLines(PENDING_ACCOUNTS_FILE);
        cout << "\nPending Applications:\n";
        cout << "User ID\n";
        cout << string(20, '-') << "\n";
        
        for (const auto& line : lines) {
            cout << line << "\n";
        }
    }
bool addCompany(const string& name, const string& address, int taxNumber, const string& password, const string& accountType) {
    int id = generateNewId(COMPANIES_FILE);
    stringstream ss;
    ss << id << "|" << name << "|" << address << "|" 
       << taxNumber << "|" << password << "|0.0|pending" << accountType << "|0.0";
    
    if (!appendFileLine(COMPANIES_FILE, ss.str())) {
        return false;
    }
    
    // Add to pending accounts
    return appendFileLine(PENDING_ACCOUNTS_FILE, to_string(id));
}
double getUserBalance(int userId) {
    auto lines = readFileLines(USERS_FILE);
    for (const auto& line : lines) {
        istringstream iss(line);
        vector<string> fields;
        string field;
        
        while (getline(iss, field, '|')) {
            fields.push_back(field);
        }
        
        if (fields.size() >= 6 && stoi(fields[0]) == userId) {
            return stod(fields[5]);
        }
    }
    return 0.0;
}
double getUserCurrentWithdrawal(int userId) {
    auto lines = readFileLines(USERS_FILE);
    for (const auto& line : lines) {
        istringstream iss(line);
        vector<string> fields;
        string field;
        
        while (getline(iss, field, '|')) {
            fields.push_back(field);
        }
        
        if (fields.size() >= 9 && stoi(fields[0]) == userId) {
            return stod(fields[8]);
        }
    }
    return 0.0;
}
string getUserAccountType(int userId) {
    auto lines = readFileLines(USERS_FILE);
    for (const auto& line : lines) {
        istringstream iss(line);
        vector<string> fields;
        string field;
        
        while (getline(iss, field, '|')) {
            fields.push_back(field);
        }
        
        if (fields.size() >= 8 && stoi(fields[0]) == userId) {
            return fields[7];
        }
    }
    return "";
}
bool isUserAccountFrozen(int userId) {
    auto lines = readFileLines(USERS_FILE);
    for (const auto& line : lines) {
        istringstream iss(line);
        vector<string> fields;
        string field;
        
        while (getline(iss, field, '|')) {
            fields.push_back(field);
        }
        
        if (fields.size() >= 7 && stoi(fields[0]) == userId) {
            return fields[6]=="frozen";
        }
    }
    return false;
}
vector<int> getCompanyEmployees(int companyId) {
    vector<int> employees;
    auto lines = readFileLines(EMPLOYEES_FILE);
    
    for (const auto& line : lines) {
        istringstream iss(line);
        vector<string> fields;
        string field;
        
        while (getline(iss, field, '|')) {
            fields.push_back(field);
        }
        
        if (fields.size() >= 2 && stoi(fields[0]) == companyId) {
            employees.push_back(stoi(fields[1]));
        }
    }
    
    return employees;
}
vector<Card*> getUserCards(int userId) {
    vector<Card*> cards;
    auto lines = readFileLines(CARDS_FILE);
    
    for (const auto& line : lines) {
        istringstream iss(line);
        vector<string> fields;
        string field;
        
        while (getline(iss, field, '|')) {
            fields.push_back(field);
        }
        
        if (fields.size() >= 3 && stoi(fields[0]) == userId) {
            Card* card = new Card(userId);
            card->setPin(fields[2]);
            cards.push_back(card); // Only store the PIN
        }
    }
    
    return cards;
}
     // Add new bank employee
    bool addBankEmployee(const string& name, const string& password) {
        int id = generateNewId(BANK_EMPLOYEES_FILE);
        stringstream ss;
        ss << id << "|" << name << "|" << "BANK_EMPLOYEE" << "|" << password;
        return appendFileLine(BANK_EMPLOYEES_FILE, ss.str());
    }

    // Verify bank employee credentials
    bool verifyBankEmployee(int id, const string& password) {
        auto lines = readFileLines(BANK_EMPLOYEES_FILE);
        
        for (const auto& line : lines) {
            vector<string> fields;
            stringstream ss(line);
            string field;
            
            while (getline(ss, field, '|')) {
                fields.push_back(field);
            }
            
            if (fields.size() >= 4 && 
                stoi(fields[0]) == id && 
                fields[3] == password) {
                return true;
            }
        }
        return false;
    }
};

// Initialize the static instance pointer
FileHandler* FileHandler::instance = nullptr;

// Singleton pattern implementation
FileHandler* FileHandler::getInstance() {
    if (instance == nullptr) {
        instance = new FileHandler();
    }
    return instance;
}
