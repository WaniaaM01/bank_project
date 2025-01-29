/*class AccountType {
    public:
    string name;                // Stores account type name
    double dailywithdrawalLimit; // Maximum amount you can withdraw per day

    // Checks if a withdrawal is allowed
    bool canwithdraw(double amount){
        // Can't withdraw negative or zero amount
        if(amount <= 0) return false;
        // Can't withdraw more than daily limit
        if(amount > dailywithdrawalLimit) return false;
        return true;
    }
}

// Different account types with their limits
class Bronze : public AccountType {
    Bronze(){
        name = "Bronze";
        dailywithdrawalLimit = 100000.0;  // Bronze accounts can withdraw up to 100,000 per day
    }
}

class Gold : public AccountType {
    Gold(){
        name = "Gold";
        dailywithdrawalLimit = 500000.0;  // Gold accounts can withdraw up to 500,000 per day
    }
}

class Business : public AccountType {
    Business(){
        name = "Business";
        dailywithdrawalLimit = 20000000.0;  // Business accounts can withdraw up to 20,000,000 per day
    }
}*/  /*AccountType Class (account_type.cpp):*/



/*Card Class (card.cpp):*/
/*class Card {
    private:
    int userid;         // Who owns the card
    string cardnumber;  // Card number (like your actual bank card)
    string pin;         // Secret PIN for the card
    bool isActive;      // Whether card is active or blocked

    public:
    // Creates a new card for a user
    Card(int uid) {
        userid = uid;
        isActive = true;
        cardnumber = generateCardNumber();  // Creates random 16-digit number
    }

    // Creates a random 16-digit card number
    string generateCardNumber() {
        // Uses random number generator to create 16 digits
        stringstream ss;
        for (int i = 0; i < 16; ++i) {
            ss << random_number_between_0_and_9;
        }
        return ss.str();
    }

    // Sets the card's PIN
    bool setPin(const string& cardPin) {
        // PIN must be exactly 4 digits
        if(cardPin.length() != 4) return false;
        pin = cardPin;
        return true;
    }

    // Checks if entered PIN is correct
    bool validatePin(const string& cardPin) {
        return isActive && pin == cardPin;
    }
}*/


/* class Transaction {
    private:
    int userid;              // Who made the transaction
    double amount;           // How much money
    string transactiontype;  // What type (deposit/withdraw/transfer)
    string transactionDate;  // When it happened
    int transferRecipientid; // Who received the money (for transfers)

    public:
    // Constructor for transfer transactions
    Transaction(int userid, double amount, const string& transactiontype, int transferRecipientid) {
        this->userid = userid;
        this->amount = amount;
        this->transactiontype = transactiontype;
        this->transferRecipientid = transferRecipientid;
    }

    // Constructor for deposits/withdrawals
    Transaction(int userid, double amount, string transactiontype) {
        this->userid = userid;
        this->amount = amount;
        this->transactiontype = transactiontype;
        this->transferRecipientid = -1;  // No recipient for non-transfers
    }

    // Records transaction to system
    bool logTransaction() {
        // Creates a record like: "User 123 deposited $500 on 2024-03-20"
        stringstream ss;
        ss << "User Id: " << userid 
           << " Amount: " << amount 
           << " Transaction Type: " << transactiontype 
           << " Transaction Date: " << transactionDate;
        
        // Add recipient info if it's a transfer
        if (transactiontype == "transfer") {
            ss << " Recipient Id: " << transferRecipientid;
        }
        return true;
    }

    // Shows transaction details
    void display() const {
        cout << "User ID: " << userid 
             << " | Amount: $" << fixed << setprecision(2) << amount 
             << " | Type: " << transactiontype;
        if (transactiontype == "transfer") {
            cout << " | Sent to: " << transferRecipientid;
        }
        cout << endl;
    }
}; */


/*File handler.cpp*/
/*class FileHandler {
    // Singleton pattern - only one instance can exist
    static FileHandler* instance;

    // File names for different types of data
    const string USERS_FILE = "users.txt";         // Stores user information
    const string COMPANIES_FILE = "companies.txt";  // Stores company information
    const string CARDS_FILE = "cards.txt";         // Stores card information
    // ... other file names ...

    public:
    // Basic file operations
    
    // Reads all lines from a file
    vector<string> readFileLines(const string& filename) {
        vector<string> lines;
        ifstream file(filename);  // Opens the file
        string line;
    
        if (file.is_open()) {
            // Read line by line until end of file
            while (getline(file, line)) {
                lines.push_back(line);
            }
        }
        file.close();
        return lines;
    }

    // Writes lines to a file
    bool writeFileLines(const string& filename, const vector<string>& lines) {
        ofstream file(filename);
        if (!file.is_open()) return false;
    
        // Write each line to file
        for (const auto& line : lines) {
            file << line << endl;
        }
        file.close();
        return true;
    }

    // Adds a new line to end of file
    bool appendFileLine(const string& filename, const string& line) {
        ofstream file(filename, ios::app);  // Open in append mode
        if (!file.is_open()) return false;
    
        file << line << endl;
        file.close();
        return true;
    }

    // Creates new unique ID for users/companies
    int generateNewId(const string& filename) {
        auto lines = readFileLines(filename);
        int maxId = 0;
    
        // Find highest existing ID
        for (const auto& line : lines) {
            istringstream iss(line);
            int id;
            if (iss >> id) {
                maxId = max(maxId, id);
            }
        }
        // Return next available ID
        return maxId + 1;
    }
    
    class FileHandler {
    // ... (previous code) ...

    // User Management Functions
    
    // Adds a new user to the system
    bool addUser(const string& name, const string& address, const string& cnic, 
                const string& password, const string& accountType) {
        // Generate new unique ID for user
        int id = generateNewId(USERS_FILE);
        
        // Create user record in format:
        // ID|Name|Address|CNIC|Password|Balance|Status|AccountType|WithdrawalLimit
        stringstream ss;
        ss << id << "|" << name << "|" << address << "|" << cnic << "|" 
           << password << "|0.0|pending|" << accountType << "|0.0";
    
        // Add user to main file and pending accounts
        if (!appendFileLine(USERS_FILE, ss.str())) {
            return false;
        }
        return appendFileLine(PENDING_ACCOUNTS_FILE, to_string(id));
    }

    // Checks if login credentials are correct
    bool verifyUserClient(int id, const string& password) {
        // Read all users from file
        auto lines = readFileLines(USERS_FILE);
    
        for (const auto& line : lines) {
            // Split line into fields using | delimiter
            vector<string> fields;
            stringstream ss(line);
            string field;
            
            while (getline(ss, field, '|')) {
                fields.push_back(field);
            }
            
            // Check if ID and password match
            if (fields.size() >= 5 && stoi(fields[0]) == id) {
                return fields[4] == password;
            }
        }
        return false;
    }

    // Updates user's balance
    bool updateBalance(int userId, double newBalance) {
        auto lines = readFileLines(USERS_FILE);
        bool found = false;
        
        // Look for user's record
        for (auto& line : lines) {
            vector<string> fields;
            stringstream ss(line);
            string field;
            
            while (getline(ss, field, '|')) {
                fields.push_back(field);
            }
            
            // When user found, update their balance
            if (fields.size() >= 6 && stoi(fields[0]) == userId) {
                fields[5] = to_string(newBalance);  // Update balance field
                
                // Rebuild the line with updated balance
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
        
        // Write all lines back to file if user was found
        return found && writeFileLines(USERS_FILE, lines);
    }

    // Approves a pending account
    bool approveAccount(int clientId) {
        auto lines = readFileLines(USERS_FILE);
        bool found = false;
        
        // Find and update user's status
        for (auto& line : lines) {
            vector<string> fields;
            stringstream ss(line);
            string field;
            
            while (getline(ss, field, '|')) {
                fields.push_back(field);
            }
            
            // When user found, change status from 'pending' to 'active'
            if (fields.size() >= 7 && stoi(fields[0]) == clientId) {
                fields[6] = "active";  // Change status
                
                // Rebuild line with new status
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
        
        // If user found, also remove from pending accounts
        if (found) {
            auto pendingLines = readFileLines(PENDING_ACCOUNTS_FILE);
            // Remove user ID from pending list
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
    
    


Let me explain a key concept here: **File Storage Format**

For example, a user record in the file looks like this:
```
1|John Doe|123 Main St|1234567890123|password123|1000.50|active|Gold|500.00
```
Breaking it down:
1. User ID: 1
2. Name: John Doe
3. Address: 123 Main St
4. CNIC: 1234567890123
5. Password: password123
6. Balance: 1000.50
7. Status: active
8. Account Type: Gold
9. Current Day Withdrawal: 500.00









Part 4: More FileHandler Functions (Card and Transaction Management)

```cpp
class FileHandler {
    // ... (previous code) ...

    // Card Management Functions
    
    // Gets all cards for a specific user
    vector<Card*> getUserCards(int userId) {
        vector<Card*> cards;
        auto lines = readFileLines(CARDS_FILE);
        
        // Format in CARDS_FILE: userId|cardNumber|pin|status
        for (const auto& line : lines) {
            vector<string> fields;
            stringstream ss(line);
            string field;
            
            while (getline(ss, field, '|')) {
                fields.push_back(field);
            }
            
            // If card belongs to this user, create Card object
            if (fields.size() >= 3 && stoi(fields[0]) == userId) {
                Card* card = new Card(userId);  // Create new card
                card->setPin(fields[2]);        // Set PIN from file
                cards.push_back(card);          // Add to list
            }
        }
        return cards;
    }

    // Transaction Management Functions
    
    // Records a new transaction
    bool logTransaction(const Transaction& trans) {
        stringstream ss;
        // Format: timestamp|userId|amount|type|recipientId(optional)
        ss << getCurrentTimestamp() << "|"
           << trans.getUserId() << "|"
           << fixed << setprecision(2) << trans.getAmount() << "|"
           << trans.getType();
        
        if (trans.getType() == "transfer") {
            ss << "|" << trans.getRecipientId();
        }
        
        return appendFileLine(TRANSACTIONS_FILE, ss.str());
    }

    // Gets transaction history for a user
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
            
            // If transaction involves this user (sender or receiver)
            if (fields.size() >= 4 && 
                (stoi(fields[1]) == userId || 
                 (fields[3] == "transfer" && fields.size() > 4 && 
                  stoi(fields[4]) == userId))) {
                
                // Create Transaction object
                if (fields[3] == "transfer" && fields.size() > 4) {
                    transactions.push_back(Transaction(
                        stoi(fields[1]),     // sender ID
                        stod(fields[2]),     // amount
                        fields[3],           // type
                        stoi(fields[4])      // recipient ID
                    ));
                } else {
                    transactions.push_back(Transaction(
                        stoi(fields[1]),     // user ID
                        stod(fields[2]),     // amount
                        fields[3]            // type
                    ));
                }
            }
        }
        return transactions;
    }

    // Display Functions
    
    // Shows all accounts in the system
    void displayAllAccounts() {
        auto lines = readFileLines(USERS_FILE);
        
        // Print header
        cout << "\nAll Accounts:\n";
        cout << setw(5) << "ID" << " | " 
             << setw(20) << "Name" << " | "
             << setw(10) << "Balance" << " | "
             << "Status\n";
        cout << string(50, '-') << "\n";
        
        // Print each account
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

    // Shows pending account applications
    void displayPendingApplications() {
        auto lines = readFileLines(PENDING_ACCOUNTS_FILE);
        
        cout << "\nPending Applications:\n";
        cout << "User ID\n";
        cout << string(20, '-') << "\n";
        
        // Print each pending application
        for (const auto& line : lines) {
            cout << line << "\n";
        }
    }
```

Example of how transactions are stored in TRANSACTIONS_FILE:
```
2024-03-20 14:30:25|1|500.00|deposit
2024-03-20 14:35:12|1|200.00|withdraw
2024-03-20 15:00:00|1|300.00|transfer|2
```

Breaking down a transaction record:
1. Timestamp: When it happened
2. User ID: Who made the transaction
3. Amount: How much money
4. Type: deposit/withdraw/transfer
5. Recipient ID: (only for transfers) Who received the money

Would you like me to continue with the User system next? This will show how all these components work together.*/


/*User system (user_type.cpp):*/


/*// Base User Class - Template for all user types
class User {
    protected:
        int userid;           // Unique identifier
        string password;      // User's password
        string accountType;   // Bronze/Gold/Business
        bool isAccountFrozen; // If account is frozen

    public:
       // Initialize new user
       User(){
        userid = 0;
        isAccountFrozen = false;
        accountType = "";
       }
       
       virtual ~User(){}; // Cleanup

       // Check password format (basic version)
       virtual bool validatePassword(const string& pwd) const{
        return pwd.length();
       }
       
       // Login method - must be implemented by child classes
       virtual bool login(int id, const string& pwd) = 0;
       
       // Get user's ID
       int getuserid() const {
        return userid;
       }
};

// Regular User Client Class
class UserClient : public User {
    private:
        double balance;            // Account balance
        vector<Card*> cards;       // User's bank cards
        bool isApproved;          // If account is approved
        string fullName;          // User's name
        string address;           // User's address
        string CNIC;              // National ID
        double currentwithdrawal; // Today's withdrawal total

        // Validate CNIC format (must be 13 digits)
        bool validateCNIC(const string& cnic) const {
            if(cnic.length() != 13) return false;
            for (char c:cnic){
                if(!isdigit(c))return false;
            }
            return true;
        }

        // Validate password requirements
        bool validatePassword(const string& pwd) const {
            if(pwd.length() < 8) return false;
            bool hasUpper=false, hasLower=false, hasDigit=false;
            
            // Check for required character types
            for (char c:pwd){
                if(isupper(c)) hasUpper=true;
                if(islower(c)) hasLower=true;
                if(isdigit(c)) hasDigit=true;
            }
            return hasUpper && hasLower && hasDigit;
        }

    public:
        // Initialize new user client
        UserClient(){
            balance = 0.0;
            isApproved = false;
            isAccountFrozen = false;
            currentwithdrawal = 0.0;
        }

        // Cleanup - delete all cards
        ~UserClient(){
            for(auto& card:cards){
                delete card;
            }
        }

        // Create new account
        bool createAccount(const string& fullName, const string& address, 
                         const string& CNIC, const string& password, 
                         const string& accountType){
            // Validate inputs
            if (!validatePassword(password) || !validateCNIC(CNIC)) 
                return false;
            
            // Store user details
            this->fullName = fullName;
            this->address = address;
            this->CNIC = CNIC;
            this->password = password;
            this->accountType = accountType;

            // Add to system through FileHandler
            return fh->addUser(fullName, address, CNIC, password, accountType);
        }

        // Login to account
        bool login(int id, const string& pwd) {
            // Verify credentials
            if (!fh->verifyUserClient(id, pwd)) return false;

            // Load user data if verified
            userid = id;
            password = pwd;
            balance = fh->getUserBalance(id);
            currentwithdrawal = fh->getUserCurrentWithdrawal(id);
            accountType = fh->getUserAccountType(id);
            isAccountFrozen = fh->isUserAccountFrozen(id);
            cards = fh->getUserCards(id);

            return true;
        }

        // Card Management Methods
        bool requestCard(){
            if(!isApproved || isAccountFrozen) return false;

            Card* newCard = new Card(userid);
            cards.push_back(newCard);
            return true;
        }

        bool validateCardPin(const string& cardPin){
            for(auto& card:cards){
                if(card->validatePin(cardPin)) return true;
            }
            return false;
        }

        void displayCards() const {
            if(cards.empty()){
                cout << "No cards found" << endl;
                return;
            }
            cout << "Your Cards:" << endl;
            for(auto& card:cards){
                cout << "Card Number: " << card->getCardNumber() << endl;
            }
        }
        
        









Part 6: UserClient Transaction Methods and CompanyClient

```cpp
// Continuing UserClient class...

    // Transaction Methods
    
    // Handle deposits
    bool deposit(double amount){
        // Basic checks
        if(!isApproved) {
            cout << "Account not approved for transactions." << endl;
            return false;
        }
        if(isAccountFrozen) {
            cout << "Account is frozen. Contact bank support." << endl;
            return false;
        }
        if(amount <= 0) {
            cout << "Invalid amount. Must be greater than 0." << endl;
            return false;
        }
        
        // Process deposit
        balance += amount;
        
        // Update balance in file system
        bool updateSuccess = fh->updateBalance(userid, balance);
        
        // Log the transaction
        Transaction trans(userid, amount, "deposit");
        bool logSuccess = fh->logTransaction(trans);
        
        return updateSuccess && logSuccess;
    }

    // Handle withdrawals
    bool withdraw(double amount, const string& cardPin){
        // Security and validity checks
        if(!isApproved || isAccountFrozen) {
            cout << "Account not available for withdrawals." << endl;
            return false;
        }
        if(amount <= 0) {
            cout << "Invalid withdrawal amount." << endl;
            return false;
        }
        if(amount > balance) {
            cout << "Insufficient funds." << endl;
            return false;
        }
        if(!validateCardPin(cardPin)) {
            cout << "Invalid PIN." << endl;
            return false;
        }

        // Check withdrawal limits based on account type
        if(accountType == "Gold"){
            Gold gold;
            if(!gold.canwithdraw(amount + currentwithdrawal)) {
                cout << "Exceeds daily withdrawal limit for Gold account." << endl;
                return false;
            }
        }
        else if(accountType == "Bronze"){
            Bronze bronze;
            if(!bronze.canwithdraw(amount + currentwithdrawal)) {
                cout << "Exceeds daily withdrawal limit for Bronze account." << endl;
                return false;
            }
        }

        // Process withdrawal
        currentwithdrawal += amount;
        balance -= amount;
        
        // Update system and log transaction
        bool updateSuccess = fh->updateBalance(userid, balance);
        Transaction trans(userid, amount, "withdraw");
        bool logSuccess = fh->logTransaction(trans);
        
        return updateSuccess && logSuccess;
    }

    // Handle transfers to other accounts
    bool transfer(double amount, const string& cardPin, int receiverId){
        // Security and validity checks
        if(!isApproved || isAccountFrozen) {
            cout << "Account not available for transfers." << endl;
            return false;
        }
        if(amount <= 0 || amount > balance) {
            cout << "Invalid transfer amount." << endl;
            return false;
        }
        if(!validateCardPin(cardPin)) {
            cout << "Invalid PIN." << endl;
            return false;
        }

        // Process transfer
        balance -= amount;
        
        // Update system and log transaction
        bool updateSuccess = fh->updateBalance(userid, balance);
        // Also update receiver's balance
        bool receiverUpdate = fh->addToBalance(receiverId, amount);
        
        // Log transfer transaction
        Transaction trans(userid, amount, "transfer", receiverId);
        bool logSuccess = fh->logTransaction(trans);
        
        return updateSuccess && receiverUpdate && logSuccess;
    }

    // View transaction history
    bool viewTransactionHistory() const {
        if(!isApproved || isAccountFrozen) {
            cout << "Cannot view history of inactive account." << endl;
            return false;
        }
        
        vector<Transaction> transactions = fh->viewTransactionHistory(userid);
        
        cout << "\nTransaction History:\n";
        cout << string(50, '-') << endl;
        
        for(const auto& trans : transactions) {
            trans.display();
        }
        
        return true;
    }
};

// Company Client Class - Similar to UserClient but with business features
class CompanyClient : public User {
    private:
        string companyName;
        double balance;
        vector<Card*> cards;
        vector<int> employees;    // List of employee IDs
        bool isApproved;
        int taxNumber;           // Company tax ID
        double currentwithdrawal;

        // Validate tax number
        bool validateTaxNumber(int taxNum) const {
            return taxNum > 0 && taxNum < 1000000000; // Basic validation
        }

    public:
        CompanyClient(){
            balance = 0.0;
            currentwithdrawal = 0.0;
            isApproved = false;
            isAccountFrozen = false;
        }
```

Would you like me to continue with the CompanyClient methods? These include:
1. Business-specific account management
2. Employee management
3. Business loan requests
4. Higher transaction limits
5. Special business card handling

*/


/*









Let me explain the complete flow of the banking application:

1. **Program Startup and User Types**:
```mermaid
graph TD
    A[Start Program] --> B{Choose User Type}
    B -->|1| C[Regular User]
    B -->|2| D[Company User]
    B -->|3| E[Bank Employee]
```

2. **Regular User Flow**:
```cpp
// Example of user interaction flow
int main() {
    while(true) {
        cout << "\nBanking System\n";
        cout << "1. Create New Account\n";
        cout << "2. Login\n";
        cout << "3. Exit\n";
        
        int choice;
        cin >> choice;

        if(choice == 1) {
            // New Account Creation
            UserClient newUser;
            string name, address, cnic, password, accountType;
            
            cout << "Enter Full Name: ";
            getline(cin, name);
            // ... get other details ...

            if(newUser.createAccount(name, address, cnic, password, accountType)) {
                cout << "Account created! Waiting for approval.\n";
            }
        }
        else if(choice == 2) {
            // Login Process
            int userid;
            string password;
            cout << "Enter User ID: ";
            cin >> userid;
            cout << "Enter Password: ";
            cin >> password;

            UserClient user;
            if(user.login(userid, password)) {
                // Show logged-in menu
                while(true) {
                    cout << "\n1. View Balance\n";
                    cout << "2. Deposit\n";
                    cout << "3. Withdraw\n";
                    cout << "4. Transfer\n";
                    cout << "5. Request Card\n";
                    cout << "6. View Transactions\n";
                    cout << "7. Logout\n";
                    
                    // Handle user choices...
                }
            }
        }
    }
}
```

3. **Transaction Flow**:
```mermaid
graph TD
    A[User Initiates Transaction] --> B{Check Account Status}
    B -->|Not Approved| C[Reject Transaction]
    B -->|Frozen| D[Reject Transaction]
    B -->|Active| E{Check Transaction Type}
    E -->|Deposit| F[Process Deposit]
    E -->|Withdraw| G[Check Withdrawal Limits]
    E -->|Transfer| H[Verify Recipient]
    G --> I{Validate Card PIN}
    H --> I
    I -->|Invalid| J[Reject Transaction]
    I -->|Valid| K[Process Transaction]
    K --> L[Update Balance]
    L --> M[Log Transaction]
```

4. **Data Flow and Storage**:
```
User Actions → Validation → FileHandler → Data Files
┌─────────────┐    ┌──────────┐    ┌───────────┐
│ users.txt   │    │ Cards    │    │Transaction│
│ cards.txt   │ ←→ │ Accounts │ ←→ │ Processing│
│ trans.txt   │    │ Security │    │           │
└─────────────┘    └──────────┘    └───────────┘
```

5. **Security Flow**:
```cpp
// Example of security checks in withdrawal
bool withdraw(double amount, const string& cardPin) {
    // 1. Account Status Check
    if(!isApproved || isAccountFrozen) return false;

    // 2. Amount Validation
    if(amount <= 0 || amount > balance) return false;

    // 3. Card PIN Verification
    if(!validateCardPin(cardPin)) return false;

    // 4. Daily Limit Check
    if(accountType == "Gold") {
        Gold gold;
        if(!gold.canwithdraw(amount + currentwithdrawal)) 
            return false;
    }
    // ... similar for other account types

    // 5. Process Transaction
    balance -= amount;
    currentwithdrawal += amount;

    // 6. Update System
    return fh->updateBalance(userid, balance);
}
```

6. **Bank Employee Flow**:
```mermaid
graph TD
    A[Bank Employee Login] --> B{Choose Action}
    B -->|1| C[View Pending Accounts]
    B -->|2| D[Approve/Reject Account]
    B -->|3| E[View All Accounts]
    B -->|4| F[Freeze/Unfreeze Account]
    B -->|5| G[Handle Loan Requests]
    C --> H[Update Account Status]
    D --> H
    F --> H
    G --> I[Update Loan Status]
```

7. **Company Account Flow**:
```mermaid
graph TD
    A[Company Login] --> B{Business Operations}
    B -->|1| C[Regular Banking]
    B -->|2| D[Employee Management]
    B -->|3| E[Business Loans]
    C --> F[Higher Transaction Limits]
    D --> G[Add/Remove Employees]
    E --> H[Loan Applications]
```

*/