#ifndef TRANSACTION_H
#define TRANSACTION_H

#include <string>
#include <ctime>

using namespace std;

class Transaction {
private:
    time_t timestamp;
    int userId;
    double amount;
    string type;
    int recipientId;

public:
    Transaction();
    Transaction(const string& transactionString);
    Transaction(const string& date, int userId, double amount, const string& type);
    Transaction(const string& date, int userId, double amount, const string& type, int recipientId);
    Transaction(time_t timestamp, int userId, double amount, const string& type);
    Transaction(time_t timestamp, int userId, double amount, const string& type, int recipientId);
    
    string getDate() const;
    string getFormattedDate() const;
    int getUserId() const;
    double getAmount() const;
    string getType() const;
    int getRecipientId() const;
    bool isValid() const;
    string toString() const;
};

#endif 