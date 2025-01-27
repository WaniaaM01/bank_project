#include "../include/transaction.h"
#include <ctime>
#include <sstream>
#include <iomanip>

// Helper function to parse date string
time_t parseDate(const string& dateStr) {
    istringstream ss(dateStr);
    tm timeinfo = {};
    
    ss >> get_time(&timeinfo, "%Y-%m-%d");
    if (ss.fail()) {
        return time(nullptr); // Return current time if parsing fails
    }
    
    return mktime(&timeinfo);
}

Transaction::Transaction() : timestamp(0), userId(0), amount(0.0), type(""), recipientId(-1) {}

Transaction::Transaction(const string& transactionString) {
    istringstream iss(transactionString);
    string dateStr;
    
    iss >> dateStr >> userId >> amount >> type;
    timestamp = parseDate(dateStr);
    
    if (type == "transfer") {
        iss >> recipientId;
    } else {
        recipientId = -1;
    }
}

Transaction::Transaction(const string& date, int userId, double amount, const string& type)
    : userId(userId), amount(amount), type(type), recipientId(-1) {
    timestamp = parseDate(date);
}

Transaction::Transaction(const string& date, int userId, double amount, const string& type, int recipientId)
    : userId(userId), amount(amount), type(type), recipientId(recipientId) {
    timestamp = parseDate(date);
}

Transaction::Transaction(time_t timestamp, int userId, double amount, const string& type)
    : timestamp(timestamp), userId(userId), amount(amount), type(type), recipientId(-1) {}

Transaction::Transaction(time_t timestamp, int userId, double amount, const string& type, int recipientId)
    : timestamp(timestamp), userId(userId), amount(amount), type(type), recipientId(recipientId) {}

string Transaction::getDate() const {
    char buffer[26];
    #ifdef _WIN32
        ctime_s(buffer, sizeof(buffer), &timestamp);
    #else
        ctime_r(&timestamp, buffer);
    #endif
    string date(buffer);
    return date.substr(0, date.length() - 1);
}

string Transaction::getFormattedDate() const {
    char buffer[11];
    tm* timeinfo = localtime(&timestamp);
    strftime(buffer, sizeof(buffer), "%Y-%m-%d", timeinfo);
    return string(buffer);
}

int Transaction::getUserId() const {
    return userId;
}

double Transaction::getAmount() const {
    return amount;
}

string Transaction::getType() const {
    return type;
}

int Transaction::getRecipientId() const {
    return recipientId;
}

bool Transaction::isValid() const {
    if (amount <= 0) {
        return false;
    }
    if (type == "transfer" && recipientId <= 0) {
        return false;
    }
    return true;
}

string Transaction::toString() const {
    ostringstream oss;
    oss << getFormattedDate() << " " << userId << " " << fixed << setprecision(2) << amount << " " << type;
    if (recipientId > 0) {
        oss << " " << recipientId;
    }
    return oss.str();
} 