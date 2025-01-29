class Card {
    private:
    int userid;         // Who owns the card
    string cardnumber;  // Card number (like your actual bank card)
    string pin;         // Secret PIN for the card
    bool isActive;      // Whether card is active or blocked

    public:
    // Creates a new card for a user
    Card(int uid) {
        userid = uid;
        isActive = true;
        cardnumber = generateCardNumber();  // Creates random 16-digit number
    }

    // Creates a random 16-digit card number
    string generateCardNumber() {
        // Uses random number generator to create 16 digits
        stringstream ss;
        for (int i = 0; i < 16; ++i) {
            ss << random_number_between_0_and_9;
        }
        return ss.str();
    }

    // Sets the card's PIN
    bool setPin(const string& cardPin) {
        // PIN must be exactly 4 digits
        if(cardPin.length() != 4) return false;
        pin = cardPin;
        return true;
    }

    // Checks if entered PIN is correct
    bool validatePin(const string& cardPin) {
        return isActive && pin == cardPin;
    }
}
