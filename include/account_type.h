#ifndef ACCOUNT_TYPE_H
#define ACCOUNT_TYPE_H

#include <string>
using namespace std;

class AccountType {
private:
    string name;
    double dailyWithdrawalLimit;
    bool isBusinessOnly;

public:
    AccountType(const string& name, double limit, bool businessOnly);
    virtual ~AccountType() = default;

    // Getters
    string getName() const;
    double getDailyWithdrawalLimit() const;
    bool getIsBusinessOnly() const;

    // Virtual function for validation
    virtual bool validateWithdrawal(double amount, bool isCompanyAccount) const;
};

#endif 