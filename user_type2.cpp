#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include "card.h"
#include "account_type.cpp"
#include "file_handler.cpp"
using namespace std;

FileHandler* fh = FileHandler::getInstance();
class User{
    protected:
        int userid;
        string password;
        string accountType;
        bool isAccountFrozen;
    public:
       User(){
        userid=0;
        isAccountFrozen=false;
        accountType="";
       }
       ~User(){};

       virtual bool validatePassword(const string& pwd) const{
        return pwd.length();
       }
       virtual bool login(int id, const string& pwd)=0;
       int getuserid() const {
        return userid;
       }
};


class UserClient : public User{
    private:
        double balance;
        vector<Card*> cards;
        bool isApproved;
        string fullName;
        string address;
        string CNIC;
        double currentwithdrawal;

        bool validateCNIC(const string& cnic) const {
            if(cnic.length() != 13) return false;

            for (char c:cnic){
                if(!isdigit(c))return false;
            }
            return true;
        }
        bool validatePassword(const string& pwd) const {
            if(pwd.length() < 8) return false;
            bool hasUpper=false, hasLower=false, hasDigit=false;
            for (char c:pwd){
                if(isupper(c))hasUpper=true;
                if(islower(c))hasLower=true;
                if(isdigit(c))hasDigit=true;
            }
            return true;
        }


    public:
        UserClient(){
            balance=0;
            isApproved=false;
            isAccountFrozen=false;
            currentwithdrawal=0;
        }
        ~UserClient(){
            for(auto& card:cards){
                delete &card;
            }
        }
        bool createAccount(const string& fullName, const string& address, const string& CNIC, const string& password, const string& accountType){
            if (!validatePassword(password)||!validateCNIC(CNIC))return false;
            this->fullName=fullName;
            this->address=address;
            this->CNIC=CNIC;
            this->password=password;
            this->accountType=accountType;

            return fh->addUser(fullName, address, CNIC, password, accountType);

        }
        bool login(int id, const string& pwd) {
            FileHandler* fh = FileHandler::getInstance();
            if (!fh->verifyUserClient(id, pwd)) return false;

            userid = id;
            password = pwd;
            balance = fh->getUserBalance(id);
            currentwithdrawal = fh->getUserCurrentWithdrawal(id);
            accountType = fh->getUserAccountType(id);
            isAccountFrozen = fh->isUserAccountFrozen(id);
            cards = fh->getUserCards(id);

            return true;
        }
        bool requestCard(){
            if(!isApproved || isAccountFrozen)return false;

            Card* newCard = new Card(userid);
            cards.push_back(newCard);
            return true;
        }
        bool validateCardPin(const string& cardPin){
            for(auto& card:cards){
                if(card->validatePin(cardPin))return true;
            }
            return false;
        }
        void displayCards() const {
            if(cards.empty()){
                cout << "No cards found" << endl;
            }
            cout << "Your Cards:" << endl;
            for(auto& card:cards){
                cout << "Card Number:" << card->getCardNumber() << endl;
            }
        }
        bool deposit(double amount){
            if(!isApproved || isAccountFrozen || amount <= 0) return false;
            bool f1, f2=true;
            // add storage;
            f1= fh->updateBalance(userid, balance);
            balance +=amount;
            return f1 ||f2;
        }
        bool withdraw(double amount, const string& cardPin){
            if(!isApproved || isAccountFrozen || amount <= 0 || amount > balance) return false;
            if(!validateCardPin(cardPin))return false;

            if(accountType == "Gold"){
                if(accountType == "Gold"){
                Gold gold;
                if(!gold.canwithdraw(amount + currentwithdrawal))return false;
                }
            }
            else if(accountType == "Bronze"){
                Bronze bronze;
                if(!bronze.canwithdraw(amount + currentwithdrawal))return false;
            }

            bool f1,f2=true;
            // add storage;
            currentwithdrawal += amount; // make a update currentwithdrawal file handling function
            balance -=amount;
            f1= fh->updateBalance(userid, balance);
            return f1 || f2;
        }
        bool transfer(double amount, const string& cardPin, int receiverId){
            if(!isApproved || isAccountFrozen || amount <= 0 || amount > balance) return false;
            if(!validateCardPin(cardPin))return false;
            // add storage;
            bool f1, f2=true;
            balance -=amount;
            f1= fh->updateBalance(userid, balance);
            return f1 || f2;
        }
        bool viewTransactionHistory(int userid) const {
            if(!isApproved || isAccountFrozen)return false;
            // add file parsing
            // parse the file at userid match
            vector<Transaction> transactions = fh->viewTransactionHistory(userid);
            for(const auto& trans : transactions) {
            trans.display(); // forgot display function in transaction class
            }
            return true;
        }
};


class  CompanyClient : public User{
    private:
        string companyName;
        string address;
        int taxNumber;
        double currentwithdrawal;
        double balance;
        vector<Card*> cards;
        vector<int> employees;
        bool isApproved;


        bool validateTaxNumber(int taxNumber) const {
            return taxNumber > 0 && taxNumber < 1000000000;
        }
    public:
        CompanyClient(){
            balance=0;
            currentwithdrawal=0;
            isApproved=false;
            isAccountFrozen=false;
        }
        ~CompanyClient(){
            for(auto& card:cards){
                delete &card;
            }
        }
        bool createAccount(const string& companyName, const string& address, int taxNumber, const string& password, const string& accountType){
            if(!validatePassword(password) || !validateTaxNumber(taxNumber))return false;
            this->companyName=companyName;
            this->address=address;
            this->taxNumber=taxNumber;
            this->password=password;
            this->accountType=accountType;
            // add DB and File handling
            return fh->addCompany(companyName, address, taxNumber, password, accountType);

            return true;
        }
        bool login(int id, const string& pwd) {

            if (!fh->verifyUserClient(id, pwd)) return false;

            userid = id;
            password = pwd;
            balance = fh->getUserBalance(id);
            currentwithdrawal = fh->getUserCurrentWithdrawal(id);
            accountType = fh->getUserAccountType(id);
            isAccountFrozen = fh->isUserAccountFrozen(id);
            employees = fh->getCompanyEmployees(id);
            cards = fh->getUserCards(id);

            return true;
        }
        bool requestCard(){
            if(!isApproved || isAccountFrozen)return false;
            Card* newCard = new Card(userid);
            cards.push_back(newCard);
            return true;
        }
        bool validateCardPin(const string& cardPin){
            for(auto& card:cards){
                if(card->validatePin(cardPin))return true;
            }
            return false;
        }
        void displayCards() const {
            if(cards.empty()){
                cout << "No cards found" << endl;
            }
            cout << "Your Cards:" << endl;
            for(auto& card:cards){
                cout << "Card Number:" << card->getCardNumber() << endl;
            }
        }
        bool deposit(double amount){
            if(!isApproved || isAccountFrozen || amount <= 0) return false;
            bool f1,f2=true;
            // add storage;
            balance +=amount;
            f1= fh->updateBalance(userid, balance);
            return f1 ||f2;
        }
        bool withdraw(double amount, const string& cardPin){
            if(!isApproved || isAccountFrozen || amount <= 0 || amount > balance) return false;
            if(!validateCardPin(cardPin))return false;
            if(accountType == "Gold"){
                if(accountType == "Gold"){
                Gold gold;
                if(!gold.canwithdraw(amount + currentwithdrawal))return false;
                }
            }
            else if(accountType == "Bronze"){
                Bronze bronze;
                if(!bronze.canwithdraw(amount + currentwithdrawal))return false;
            }
            else if(accountType == "Business"){
                Business business;
                if(!business.canwithdraw(amount + currentwithdrawal))return false;
            }
            bool f1,f2=true;
            // add storage;
            currentwithdrawal += amount; // make a update currentwithdrawal file handling function
            balance -=amount;
            f1= fh->updateBalance(userid, balance);
            return f1 || f2;
        }
        bool transfer(double amount, const string& cardPin, int receiverId){
            if(!isApproved || isAccountFrozen || amount <= 0 || amount > balance) return false;
            if(!validateCardPin(cardPin))return false;
            // add storage;
            bool f1, f2=true;
            balance -=amount;
            f1= fh->updateBalance(userid, balance);
            return f1 || f2;
        }
        bool viewTransactionHistory(int userid) const {
            if(!isApproved || isAccountFrozen)return false;
            // add file parsing
            // parse the file at userid match
            vector<Transaction> transactions = fh->viewTransactionHistory(userid);
            for(const auto& trans : transactions) {
                trans.display(); // forgot display function in transaction class
    }
            return true;
        }
        bool addEmployee(int employeeId){
            if(!isApproved || isAccountFrozen)return false;

            // add file parsing
            // parse the file at userid match
            // add the employee id to the file
            employees.push_back(employeeId);
            return fh->addCompanyEmployee(userid, employeeId);;
        }
        bool removeEmployee(int employeeId){
            if(!isApproved || isAccountFrozen)return false;
            // add file parsing
            // parse the file at userid match
            // remove the employee id from the file
            employees.erase(remove(employees.begin(), employees.end(), employeeId), employees.end());
            return fh->removeCompanyEmployee(userid, employeeId);;
        }
        void displayEmployees() const {
            if(employees.empty()){
                cout << "No employees found" << endl;
            }
            cout << "Employees:" << endl;
            for(auto& employee:employees){
                cout << "Employee ID:" << employee << endl;
            }
            return;
        }
        bool requestLoan(double amount, const string& reason){
            if(!isApproved || isAccountFrozen)return false;
            // add file parsing
            // parse the file at userid match
            // add the loan amount to the file
            return fh->addLoanRequest(userid, amount, reason);;
        }
};


class BankEmployee : public User{
    public:
        bool createAccount(const string& name, const string& address, const string& password) {

            return fh->addBankEmployee(name, password);
}
        bool login(int id, const string& pwd){
            if (!fh->verifyBankEmployee(id, pwd)) return false;
            userid=id;
            password=pwd;
            return true;
        }
        bool approveNewAccount(int userid){
            // add file parsing
            // parse the file at userid match
            // approve the account
            return fh->approveAccount(userid);
        }
        bool freezeAccount(int userid){
            // add file parsing
            // parse the file at userid match
            // freeze the account
            return fh->freezeAccount(userid);
        }
        bool unfreezeAccount(int userid){
            // add file parsing
            // parse the file at userid match
            // unfreeze the account
            return fh->unfreezeAccount(userid);
        }
        bool approveLoan(int loanId, int userid){
            // add file parsing
            // parse the file at userid match
            // approve the loan
            return fh->approveLoan(loanId, userid);
        }
        bool rejectLoan(int loanId, int userid){
            // add file parsing
            // parse the file at userid match
            // reject the loan
            return fh->rejectLoan(loanId, userid);
        }
        void viewAllAccounts() const {
            // add file parsing
            // view all the accounts
            fh->displayAllAccounts();
            return;
        }
        void viewTransactionHistory(int userid) const {
            // add file parsing
            // parse the file at userid match
            // view the transaction history
            return;
        }
        void viewPendingAccounts() const {
            // add file parsing
            // view all the pending accounts
            fh->displayPendingApplications();
            return;
        }
};
