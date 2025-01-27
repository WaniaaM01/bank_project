#include "../include/user_client.h"
#include "../include/file_handler.h"
#include <iostream>
#include <iomanip>

UserClient::UserClient() : User() {}

UserClient::UserClient(int id, const string& name, const string& address, 
                     const string& cnic, const string& password)
    : User(id, name, address, password), cnic(cnic) {}

bool UserClient::login(int id, const string& password) {
    return FileHandler::verifyUserClient(id, password);
}

void UserClient::createAccount() {
    string name, address, cnic, password;
    cout << "\n=== Create User Account ===\n";
    
    cout << "Enter Name: ";
    cin.ignore();
    getline(cin, name);
    
    cout << "Enter Address: ";
    getline(cin, address);
    
    cout << "Enter CNIC: ";
    getline(cin, cnic);
    
    do {
        cout << "Enter Password (min 6 characters): ";
        cin >> password;
    } while (!validatePassword(password));
    
    int newId = FileHandler::addUser(name, address, cnic, password);
    if (newId != -1) {
        cout << "\nAccount created successfully!\n";
        cout << "Your User ID is: " << newId << "\n";
        cout << "Please wait for bank employee approval before logging in.\n";
    } else {
        cout << "\nFailed to create account. Please try again.\n";
    }
}

void UserClient::displayMenu() {
    int choice;
    do {
        cout << "\n=== User Client Menu ===\n";
        cout << "1. View Balance\n";
        cout << "2. Deposit Money\n";
        cout << "3. Withdraw Money\n";
        cout << "4. Transfer Money\n";
        cout << "5. Card Operations\n";
        cout << "6. View Transactions\n";
        cout << "7. Logout\n";
        cout << "Enter your choice: ";
        cin >> choice;
        
        switch (choice) {
            case 1:
                viewBalance();
                break;
            case 2:
                handleDeposit();
                break;
            case 3:
                handleWithdrawal();
                break;
            case 4:
                handleTransfer();
                break;
            case 5:
                handleCardOperations();
                break;
            case 6:
                viewTransactionHistory();
                break;
            case 7:
                cout << "\nLogging out...\n";
                break;
            default:
                cout << "\nInvalid choice. Please try again.\n";
        }
    } while (choice != 7);
}

void UserClient::handleDeposit() {
    double amount;
    cout << "\nEnter amount to deposit: $";
    cin >> amount;
    
    if (!FileHandler::validateAmount(amount)) {
        cout << "Invalid amount.\n";
        return;
    }
    
    double currentBalance = FileHandler::getBalance(getId());
    if (FileHandler::updateBalance(getId(), currentBalance + amount)) {
        Transaction trans(time(0), getId(), amount, "deposit");
        FileHandler::logTransaction(trans);
        cout << "Deposit successful.\n";
    } else {
        cout << "Deposit failed.\n";
    }
}

void UserClient::handleWithdrawal() {
    if (FileHandler::isAccountFrozen(getId())) {
        cout << "\nYour account is frozen. Please contact bank support.\n";
        return;
    }

    double amount;
    cout << "\nEnter amount to withdraw: $";
    cin >> amount;
    
    if (!FileHandler::validateAmount(amount)) {
        cout << "Invalid amount.\n";
        return;
    }
    
    if (!FileHandler::validateWithdrawalLimit(getId(), amount)) {
        cout << "Amount exceeds daily withdrawal limit.\n";
        return;
    }
    
    double currentBalance = FileHandler::getBalance(getId());
    if (currentBalance < amount) {
        cout << "Insufficient balance.\n";
        return;
    }
    
    if (FileHandler::updateBalance(getId(), currentBalance - amount)) {
        Transaction trans(time(0), getId(), amount, "withdraw");
        FileHandler::logTransaction(trans);
        cout << "Withdrawal successful.\n";
    } else {
        cout << "Withdrawal failed.\n";
    }
}

void UserClient::handleTransfer() {
    if (FileHandler::isAccountFrozen(getId())) {
        cout << "\nYour account is frozen. Please contact bank support.\n";
        return;
    }

    int recipientId;
    double amount;
    
    cout << "\nEnter recipient ID: ";
    cin >> recipientId;
    
    if (!FileHandler::userExists(recipientId) && !FileHandler::companyExists(recipientId)) {
        cout << "Recipient not found.\n";
        return;
    }
    
    cout << "Enter amount to transfer: $";
    cin >> amount;
    
    if (!FileHandler::validateAmount(amount)) {
        cout << "Invalid amount.\n";
        return;
    }
    
    if (!FileHandler::validateWithdrawalLimit(getId(), amount)) {
        cout << "Amount exceeds daily transfer limit.\n";
        return;
    }
    
    double currentBalance = FileHandler::getBalance(getId());
    if (currentBalance < amount) {
        cout << "Insufficient balance.\n";
        return;
    }
    
    double recipientBalance = FileHandler::getBalance(recipientId);
    
    if (FileHandler::updateBalance(getId(), currentBalance - amount) &&
        FileHandler::updateBalance(recipientId, recipientBalance + amount)) {
        Transaction trans(time(0), getId(), amount, "transfer", recipientId);
        FileHandler::logTransaction(trans);
        cout << "Transfer successful.\n";
    } else {
        cout << "Transfer failed.\n";
    }
}

void UserClient::handleCardOperations() {
    int choice;
    do {
        cout << "\n=== Card Operations ===\n";
        cout << "1. Request New Card\n";
        cout << "2. View Cards\n";
        cout << "3. Report Lost Card\n";
        cout << "4. Back to Main Menu\n";
        cout << "Enter your choice: ";
        cin >> choice;
        
        switch (choice) {
            case 1:
                if (requestCard()) {
                    cout << "Card request submitted successfully.\n";
                } else {
                    cout << "Failed to request card.\n";
                }
                break;
            case 2:
                viewCards();
                break;
            case 3: {
                string cardNumber;
                cout << "Enter card number to report: ";
                cin >> cardNumber;
                if (reportCard(cardNumber)) {
                    cout << "Card reported successfully.\n";
                } else {
                    cout << "Failed to report card.\n";
                }
                break;
            }
            case 4:
                break;
            default:
                cout << "Invalid choice.\n";
        }
    } while (choice != 4);
}

void UserClient::viewBalance() {
    double balance = FileHandler::getBalance(getId());
    cout << "\nCurrent Balance: $" << fixed << setprecision(2) << balance << endl;
}

void UserClient::viewTransactionHistory() {
    FileHandler::displayTransactionHistory(getId());
}

void UserClient::viewCards() {
    vector<string> cards = FileHandler::getUserCards(getId());
    if (cards.empty()) {
        cout << "No active cards found.\n";
        return;
    }
    
    cout << "\n=== Your Cards ===\n";
    for (const string& card : cards) {
        cout << "Card Number: " << card << endl;
    }
}

bool UserClient::requestCard() {
    return FileHandler::addCard(getId());
}

bool UserClient::reportCard(const string& cardNumber) {
    return FileHandler::reportCard(getId(), cardNumber);
}

bool UserClient::validatePassword(const string& password) {
    if (password.length() < 6) {
        cout << "Password must be at least 6 characters long.\n";
        return false;
    }
    return true;
}
