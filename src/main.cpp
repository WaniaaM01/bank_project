#include <iostream>
#include <string>
#include <limits>
#include <algorithm>
#include "../include/file_handler.h"
#include "../include/bank_employee.h"
#include "../include/user_client.h"
#include "../include/company_client.h"

using namespace std;

// Function prototypes
void clearScreen();
void displayMainMenu();
void handleBankEmployeeMenu();
void handleUserClientMenu(int userId);
void handleCompanyClientMenu(int companyId);
void handleNewAccount();

int main() {
    FileHandler fileHandler;
    int choice;
    
    while (true) {
        clearScreen();
        displayMainMenu();
        
        cout << "\nEnter choice: ";
        cin >> choice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        
        switch (choice) {
            case 1: {
                int employeeId;
                string password;
                
                cout << "Enter Employee ID: ";
                cin >> employeeId;
                cout << "Enter Password: ";
                cin >> password;
                
                if (fileHandler.verifyBankEmployee(employeeId, password)) {
                    handleBankEmployeeMenu();
                } else {
                    cout << "Invalid credentials!" << endl;
                }
                break;
            }
            
            case 2: {
                int userId;
                string password;
                
                cout << "Enter User ID: ";
                cin >> userId;
                cout << "Enter Password: ";
                cin >> password;
                
                if (fileHandler.verifyUserClient(userId, password)) {
                    handleUserClientMenu(userId);
                } else {
                    cout << "Invalid credentials!" << endl;
                }
                break;
            }
            
            case 3: {
                int companyId;
                string password;
                
                cout << "Enter Company ID: ";
                cin >> companyId;
                cout << "Enter Password: ";
                cin >> password;
                
                if (fileHandler.verifyCompanyClient(companyId, password)) {
                    handleCompanyClientMenu(companyId);
                } else {
                    cout << "Invalid credentials!" << endl;
                }
                break;
            }
            
            case 4: {
                handleNewAccount();
                break;
            }
            
            case 5: {
                cout << "Thank you for using our Banking System!" << endl;
                return 0;
            }
            
            default:
                cout << "Invalid choice!" << endl;
        }
        
        cout << "\nPress Enter to continue...";
        cin.get();
    }
    
    return 0;
}

void clearScreen() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

void displayMainMenu() {
    cout << "=== Bank Management System ===" << endl;
    cout << "1. Login as Bank Employee" << endl;
    cout << "2. Login as User Client" << endl;
    cout << "3. Login as Company Client" << endl;
    cout << "4. Create New Account" << endl;
    cout << "5. Exit" << endl;
}

void handleBankEmployeeMenu() {
    FileHandler fileHandler;
    int choice;
    
    while (true) {
        clearScreen();
        cout << "=== Bank Employee Menu ===" << endl;
        cout << "1. View All Accounts" << endl;
        cout << "2. Approve/Reject Applications" << endl;
        cout << "3. Manage Loans" << endl;
        cout << "4. View Transaction History" << endl;
        cout << "5. Freeze/Unfreeze Accounts" << endl;
        cout << "6. Logout" << endl;
        
        cout << "\nEnter choice: ";
        cin >> choice;
        
        switch (choice) {
            case 1:
                fileHandler.displayAllAccounts();
                break;
            case 2: {
                fileHandler.displayPendingApplications();
                int clientId;
                char decision;
                cout << "Enter client ID: ";
                cin >> clientId;
                cout << "Approve (A) or Reject (R)? ";
                cin >> decision;
                
                if (toupper(decision) == 'A') {
                    fileHandler.approveAccount(clientId);
                } else if (toupper(decision) == 'R') {
                    fileHandler.rejectAccount(clientId);
                }
                break;
            }
            case 3: {
                fileHandler.displayPendingLoans();
                int loanId, companyId;
                char decision;
                cout << "Enter loan ID: ";
                cin >> loanId;
                cout << "Enter company ID: ";
                cin >> companyId;
                cout << "Approve (A) or Reject (R)? ";
                cin >> decision;
                
                if (toupper(decision) == 'A') {
                    fileHandler.approveLoan(loanId, companyId);
                } else if (toupper(decision) == 'R') {
                    fileHandler.rejectLoan(loanId, companyId);
                }
                break;
            }
            case 4: {
                int clientId;
                cout << "Enter client ID: ";
                cin >> clientId;
                fileHandler.displayTransactionHistory(clientId);
                break;
            }
            case 5: {
                int clientId;
                char action;
                cout << "Enter client ID: ";
                cin >> clientId;
                cout << "Freeze (F) or Unfreeze (U)? ";
                cin >> action;
                
                if (toupper(action) == 'F') {
                    fileHandler.freezeAccount(clientId);
                } else if (toupper(action) == 'U') {
                    fileHandler.unfreezeAccount(clientId);
                }
                break;
            }
            case 6:
                return;
            default:
                cout << "Invalid choice!" << endl;
        }
        
        cout << "\nPress Enter to continue...";
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cin.get();
    }
}

void handleUserClientMenu(int userId) {
    FileHandler fileHandler;
    int choice;
    
    while (true) {
        clearScreen();
        cout << "=== User Client Menu ===" << endl;
        cout << "1. View Balance" << endl;
        cout << "2. Deposit" << endl;
        cout << "3. Withdraw" << endl;
        cout << "4. Transfer Money" << endl;
        cout << "5. View Transaction History" << endl;
        cout << "6. Manage Cards" << endl;
        cout << "7. Logout" << endl;
        
        cout << "\nEnter choice: ";
        cin >> choice;
        
        switch (choice) {
            case 1: {
                double balance = fileHandler.getBalance(userId);
                cout << "Current Balance: PKR " << balance << endl;
                break;
            }
            case 2: {
                double amount;
                cout << "Enter amount to deposit: ";
                cin >> amount;
                
                if (amount > 0) {
                    double currentBalance = fileHandler.getBalance(userId);
                    if (fileHandler.updateBalance(userId, currentBalance + amount)) {
                        Transaction trans;
                        trans.userId = userId;
                        trans.amount = amount;
                        trans.type = "deposit";
                        fileHandler.logTransaction(trans);
                        cout << "Deposit successful!" << endl;
                    }
                }
                break;
            }
            case 3: {
                double amount;
                cout << "Enter amount to withdraw: ";
                cin >> amount;
                
                if (amount > 0) {
                    double currentBalance = fileHandler.getBalance(userId);
                    if (currentBalance >= amount && fileHandler.validateDailyLimit(userId, amount)) {
                        if (fileHandler.updateBalance(userId, currentBalance - amount)) {
                            Transaction trans;
                            trans.userId = userId;
                            trans.amount = amount;
                            trans.type = "withdraw";
                            fileHandler.logTransaction(trans);
                            cout << "Withdrawal successful!" << endl;
                        }
                    } else {
                        cout << "Insufficient funds or daily limit exceeded!" << endl;
                    }
                }
                break;
            }
            case 4: {
                int targetId;
                double amount;
                cout << "Enter recipient User ID: ";
                cin >> targetId;
                cout << "Enter amount to transfer: ";
                cin >> amount;
                
                if (amount > 0 && fileHandler.userExists(targetId)) {
                    double currentBalance = fileHandler.getBalance(userId);
                    if (currentBalance >= amount && fileHandler.validateDailyLimit(userId, amount)) {
                        if (fileHandler.updateBalance(userId, currentBalance - amount)) {
                            double targetBalance = fileHandler.getBalance(targetId);
                            fileHandler.updateBalance(targetId, targetBalance + amount);
                            
                            Transaction trans;
                            trans.userId = userId;
                            trans.amount = amount;
                            trans.type = "transfer";
                            trans.targetUserId = targetId;
                            fileHandler.logTransaction(trans);
                            cout << "Transfer successful!" << endl;
                        }
                    } else {
                        cout << "Insufficient funds or daily limit exceeded!" << endl;
                    }
                }
                break;
            }
            case 5:
                fileHandler.displayTransactionHistory(userId);
                break;
            case 6: {
                vector<string> cards = fileHandler.getUserCards(userId);
                cout << "Your cards:" << endl;
                for (const auto& card : cards) {
                    cout << card << endl;
                }
                break;
            }
            case 7:
                return;
            default:
                cout << "Invalid choice!" << endl;
        }
        
        cout << "\nPress Enter to continue...";
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cin.get();
    }
}

void handleCompanyClientMenu(int companyId) {
    FileHandler fileHandler;
    int choice;
    
    while (true) {
        clearScreen();
        cout << "=== Company Client Menu ===" << endl;
        cout << "1. View Balance" << endl;
        cout << "2. Deposit" << endl;
        cout << "3. Withdraw" << endl;
        cout << "4. Transfer Money" << endl;
        cout << "5. View Transaction History" << endl;
        cout << "6. Manage Cards" << endl;
        cout << "7. Request Loan" << endl;
        cout << "8. Manage Employees" << endl;
        cout << "9. Logout" << endl;
        
        cout << "\nEnter choice: ";
        cin >> choice;
        
        switch (choice) {
            case 1: {
                double balance = fileHandler.getBalance(companyId);
                cout << "Current Balance: PKR " << balance << endl;
                break;
            }
            case 2: {
                double amount;
                cout << "Enter amount to deposit: ";
                cin >> amount;
                
                if (amount > 0) {
                    double currentBalance = fileHandler.getBalance(companyId);
                    if (fileHandler.updateBalance(companyId, currentBalance + amount)) {
                        Transaction trans;
                        trans.userId = companyId;
                        trans.amount = amount;
                        trans.type = "deposit";
                        fileHandler.logTransaction(trans);
                        cout << "Deposit successful!" << endl;
                    }
                }
                break;
            }
            case 3: {
                double amount;
                cout << "Enter amount to withdraw: ";
                cin >> amount;
                
                if (amount > 0) {
                    double currentBalance = fileHandler.getBalance(companyId);
                    if (currentBalance >= amount && fileHandler.validateDailyLimit(companyId, amount)) {
                        if (fileHandler.updateBalance(companyId, currentBalance - amount)) {
                            Transaction trans;
                            trans.userId = companyId;
                            trans.amount = amount;
                            trans.type = "withdraw";
                            fileHandler.logTransaction(trans);
                            cout << "Withdrawal successful!" << endl;
                        }
                    } else {
                        cout << "Insufficient funds or daily limit exceeded!" << endl;
                    }
                }
                break;
            }
            case 4: {
                int targetId;
                double amount;
                cout << "Enter recipient User ID: ";
                cin >> targetId;
                cout << "Enter amount to transfer: ";
                cin >> amount;
                
                if (amount > 0 && fileHandler.userExists(targetId)) {
                    double currentBalance = fileHandler.getBalance(companyId);
                    if (currentBalance >= amount && fileHandler.validateDailyLimit(companyId, amount)) {
                        if (fileHandler.updateBalance(companyId, currentBalance - amount)) {
                            double targetBalance = fileHandler.getBalance(targetId);
                            fileHandler.updateBalance(targetId, targetBalance + amount);
                            
                            Transaction trans;
                            trans.userId = companyId;
                            trans.amount = amount;
                            trans.type = "transfer";
                            trans.targetUserId = targetId;
                            fileHandler.logTransaction(trans);
                            cout << "Transfer successful!" << endl;
                        }
                    } else {
                        cout << "Insufficient funds or daily limit exceeded!" << endl;
                    }
                }
                break;
            }
            case 5:
                fileHandler.displayTransactionHistory(companyId);
                break;
            case 6: {
                vector<string> cards = fileHandler.getUserCards(companyId);
                cout << "Company cards:" << endl;
                for (const auto& card : cards) {
                    cout << card << endl;
                }
                break;
            }
            case 7: {
                double amount;
                string reason;
                cout << "Enter loan amount: ";
                cin >> amount;
                cin.ignore();
                cout << "Enter loan reason: ";
                getline(cin, reason);
                
                if (fileHandler.addLoanRequest(companyId, amount, reason)) {
                    cout << "Loan request submitted successfully!" << endl;
                }
                break;
            }
            case 8: {
                int empChoice;
                cout << "1. Add Employee\n2. Remove Employee\n3. View Employees\nChoice: ";
                cin >> empChoice;
                
                if (empChoice == 1) {
                    int empId;
                    cout << "Enter employee User ID: ";
                    cin >> empId;
                    if (fileHandler.addCompanyEmployee(companyId, empId)) {
                        cout << "Employee added successfully!" << endl;
                    }
                } else if (empChoice == 2) {
                    int empId;
                    cout << "Enter employee User ID: ";
                    cin >> empId;
                    if (fileHandler.removeCompanyEmployee(companyId, empId)) {
                        cout << "Employee removed successfully!" << endl;
                    }
                } else if (empChoice == 3) {
                    vector<int> employees = fileHandler.getCompanyEmployees(companyId);
                    cout << "Company employees:" << endl;
                    for (int empId : employees) {
                        cout << empId << endl;
                    }
                }
                break;
            }
            case 9:
                return;
            default:
                cout << "Invalid choice!" << endl;
        }
        
        cout << "\nPress Enter to continue...";
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cin.get();
    }
}

void handleNewAccount() {
    int accountType;
    cout << "Select account type:\n1. User Client\n2. Company Client\nEnter choice: ";
    cin >> accountType;
    cin.ignore();
    
    FileHandler fileHandler;
    
    if (accountType == 1) {
        string name, address, cnic, password;
        
        cout << "Enter Full Name: ";
        getline(cin, name);
        cout << "Enter Address: ";
        getline(cin, address);
        cout << "Enter CNIC (13 digits): ";
        getline(cin, cnic);
        
        while (cnic.length() != 13 || !all_of(cnic.begin(), cnic.end(), ::isdigit)) {
            cout << "Invalid CNIC! Please enter 13 digits: ";
            getline(cin, cnic);
        }
        
        cout << "Enter Password (min 8 characters): ";
        getline(cin, password);
        
        while (password.length() < 8) {
            cout << "Password too short! Enter at least 8 characters: ";
            getline(cin, password);
        }
        
        if (fileHandler.addUser(name, address, cnic, password)) {
            cout << "Account created successfully! Waiting for approval." << endl;
        } else {
            cout << "Error creating account!" << endl;
        }
    }
    else if (accountType == 2) {
        string name, address, password;
        int taxNumber;
        
        cout << "Enter Company Name: ";
        getline(cin, name);
        cout << "Enter Address: ";
        getline(cin, address);
        cout << "Enter Tax Number: ";
        cin >> taxNumber;
        cin.ignore();
        
        cout << "Enter Password (min 8 characters): ";
        getline(cin, password);
        
        while (password.length() < 8) {
            cout << "Password too short! Enter at least 8 characters: ";
            getline(cin, password);
        }
        
        if (fileHandler.addCompany(name, address, taxNumber, password)) {
            cout << "Company account created successfully! Waiting for approval." << endl;
        } else {
            cout << "Error creating account!" << endl;
        }
    }
}
