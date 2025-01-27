#include "../include/gold_account.h"

GoldAccount::GoldAccount()
    : AccountType("Gold", 500000.0, false) {}

bool GoldAccount::validateWithdrawal(double amount, bool isCompanyAccount) const {
    return AccountType::validateWithdrawal(amount, isCompanyAccount);
} 