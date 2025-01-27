#include "../include/bronze_account.h"

BronzeAccount::BronzeAccount()
    : AccountType("Bronze", 100000.0, false) {}

bool BronzeAccount::validateWithdrawal(double amount, bool isCompanyAccount) const {
    return AccountType::validateWithdrawal(amount, isCompanyAccount);
} 