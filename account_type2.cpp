class AccountType {
    public:
    string name;                // Stores account type name
    double dailywithdrawalLimit; // Maximum amount you can withdraw per day

    // Checks if a withdrawal is allowed
    bool canwithdraw(double amount){
        // Can't withdraw negative or zero amount
        if(amount <= 0) return false;
        // Can't withdraw more than daily limit
        if(amount > dailywithdrawalLimit) return false;
        return true;
    }
}

// Different account types with their limits
class Bronze : public AccountType {
    Bronze(){
        name = "Bronze";
        dailywithdrawalLimit = 100000.0;  // Bronze accounts can withdraw up to 100,000 per day
    }
}

class Gold : public AccountType {
    Gold(){
        name = "Gold";
        dailywithdrawalLimit = 500000.0;  // Gold accounts can withdraw up to 500,000 per day
    }
}

class Business : public AccountType {
    Business(){
        name = "Business";
        dailywithdrawalLimit = 20000000.0;  // Business accounts can withdraw up to 20,000,000 per day
    }
}
