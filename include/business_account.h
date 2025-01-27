#ifndef BUSINESS_ACCOUNT_H
#define BUSINESS_ACCOUNT_H

#include "account_type.h"

class BusinessAccount : public AccountType {
public:
    BusinessAccount();
    bool validateWithdrawal(double amount, bool isCompanyAccount) const override;
};

#endif 