#ifndef GOLD_ACCOUNT_H
#define GOLD_ACCOUNT_H

#include "account_type.h"

class GoldAccount : public AccountType {
public:
    GoldAccount();
    bool validateWithdrawal(double amount, bool isCompanyAccount) const override;
};

#endif 