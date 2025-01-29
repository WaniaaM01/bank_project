#include <iostream>   // For input/output
#include <string>     // For string operations
#include <sstream>    // For string stream
#include <iomanip>    // For output formatting
#include <ctime>      // For time operations
using namespace std;

class Transaction {
private:
    int userid;              // User who made the transaction
    double amount;           // Transaction amount
    string transactiontype;  // Type (withdrawal, deposit, transfer)
    string transactionDate;  // When transaction occurred
    int transferRecipientid; // Recipient ID for transfers

public:
    // Constructor for transfers
    Transaction(int userid, double amount, const string& transactiontype, int transferRecipientid){
        this->userid = userid;
        this->amount = amount;
        this->transactiontype = transactiontype;
        this->transferRecipientid = transferRecipientid;
    }

    // Constructor for non-transfer transactions
    Transaction(int userid, double amount, string transactiontype){
        this->userid = userid;
        this->amount = amount;
        this->transactiontype = transactiontype;
        this->transferRecipientid = -1;  // No recipient for non-transfers
    }

    // Log transaction to system
    bool logTransaction(){
        stringstream ss;
        // Format transaction details
        ss << "User Id: " << userid 
           << " Amount: " << amount 
           << " Transaction Type: " << transactiontype 
           << " Transaction Date: " << transactionDate << endl;
        
        // Add recipient info for transfers
        if (transactiontype == "transfer"){
            ss << "Recipient Id: " << transferRecipientid << endl;
        }
        return true; // Placeholder for actual storage implementation
    }

    // Display transaction details
    void display() const {
        cout << "User ID: " << userid 
             << " | Amount: " << fixed << setprecision(2) << amount 
             << " | Type: " << transactiontype;
        
        // Show recipient for transfers
        if (transactiontype == "transfer") {
            cout << " | Recipient ID: " << transferRecipientid;
        }
        cout << endl;
    }
};

/*


Key points about Transaction class:
1. **Data Storage**: Keeps track of who, what, when, and how much for each transaction
2. **Transaction Types**: Handles different types (transfers have recipients, others don't)
3. **Formatting**: Uses proper money formatting (2 decimal places)
4. **Display**: Organized output of transaction details

Let me know if you want me to continue with `file_handler.cpp` and `user_type.cpp`. Note that `file_handler.cpp` is quite large, so I'll break it down into logical sections for better understanding.

Would you like me to continue with those files?*/