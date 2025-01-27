#include "../include/account_type.h"
#include "../include/bronze_account.h"
#include "../include/gold_account.h"
#include "../include/business_account.h"

// Base AccountType Implementation
AccountType::AccountType(const string& name, double limit, bool businessOnly)
    : name(name), dailyWithdrawalLimit(limit), isBusinessOnly(businessOnly) {}

string AccountType::getName() const {
    return name;
}

double AccountType::getDailyWithdrawalLimit() const {
    return dailyWithdrawalLimit;
}

bool AccountType::getIsBusinessOnly() const {
    return isBusinessOnly;
}

bool AccountType::validateWithdrawal(double amount, bool isCompanyAccount) const {
    if (amount <= 0 || amount > dailyWithdrawalLimit) return false;
    if (isBusinessOnly && !isCompanyAccount) return false;
    return true;
}

// BronzeAccount Implementation
BronzeAccount::BronzeAccount() 
    : AccountType("Bronze", 100000.0, false) {}

bool BronzeAccount::validateWithdrawal(double amount, bool isCompanyAccount) const {
    if (amount > 50000.0 && !isCompanyAccount) {
        // Additional verification needed for large withdrawals
        return false;
    }
    return AccountType::validateWithdrawal(amount, isCompanyAccount);
}

// GoldAccount Implementation
GoldAccount::GoldAccount() 
    : AccountType("Gold", 500000.0, false) {}

bool GoldAccount::validateWithdrawal(double amount, bool isCompanyAccount) const {
    // Gold accounts get a 10% higher daily limit
    if (amount <= getDailyWithdrawalLimit() * 1.1) {
        return AccountType::validateWithdrawal(amount, isCompanyAccount);
    }
    return false;
}

// BusinessAccount Implementation
BusinessAccount::BusinessAccount() 
    : AccountType("Business", 2000000.0, true) {}

bool BusinessAccount::validateWithdrawal(double amount, bool isCompanyAccount) const {
    if (!isCompanyAccount) return false;
    return AccountType::validateWithdrawal(amount, isCompanyAccount);
}