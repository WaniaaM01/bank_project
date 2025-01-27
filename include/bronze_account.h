#ifndef BRONZE_ACCOUNT_H
#define BRONZE_ACCOUNT_H

#include "account_type.h"

class BronzeAccount : public AccountType {
public:
    BronzeAccount();
    bool validateWithdrawal(double amount, bool isCompanyAccount) const override;
};

#endif 