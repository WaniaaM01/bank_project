#include <iostream>
#include <string>
using namespace std;

class AccountType{
    public:
    string name;
    double dailywithdrawalLimit;
    
    AccountType(){};
    ~AccountType(){};
    bool canwithdraw(double amount){
        if(amount <= 0) return false;
        if(amount > dailywithdrawalLimit) return false;
        return true;
    }
    double getDailyWithdrawalLimit(){
        return dailywithdrawalLimit;
    }
};

class Bronze : public AccountType{
    public:
    Bronze(){
        name = "Bronze";
        dailywithdrawalLimit = 100000.0;
    }
};

class Gold : public AccountType{
    public:
    Gold(){
        name = "Gold";
        dailywithdrawalLimit = 500000.0;
    }
};

class Business : public AccountType{
    public:
    Business(){
        name = "Business";
        dailywithdrawalLimit = 20000000.0;
    }
};
