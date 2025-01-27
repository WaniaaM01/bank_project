#include "../include/business_account.h"

BusinessAccount::BusinessAccount()
    : AccountType("Business", 20000000.0, true) {}

bool BusinessAccount::validateWithdrawal(double amount, bool isCompanyAccount) const {
    return AccountType::validateWithdrawal(amount, isCompanyAccount);
} 