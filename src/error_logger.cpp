#include "../include/error_logger.h"

const string ErrorLogger::ERROR_LOG_FILE = "data/error.log";
ofstream ErrorLogger::logFile;

string ErrorLogger::getCurrentTimestamp() {
    time_t now = time(nullptr);
    string ts(ctime(&now));
    return ts.substr(0, ts.length() - 1); // Remove newline
}

bool ErrorLogger::openLogFile() {
    logFile.open(ERROR_LOG_FILE, ios::app);
    return logFile.is_open();
}

void ErrorLogger::log(const string& error) {
    if (!openLogFile()) {
        cerr << "Failed to open error log file!" << endl;
        return;
    }
    
    logFile << "[" << getCurrentTimestamp() << "] " << error << endl;
    logFile.close();
}

void ErrorLogger::logFileError(const string& filename, const string& operation) {
    string error = "File Error: " + operation + " failed for " + filename;
    log(error);
}

void ErrorLogger::logTransactionError(int userId, const string& operation) {
    string error = "Transaction Error: User " + to_string(userId) + " - " + operation;
    log(error);
}

void ErrorLogger::logSecurityError(int userId, const string& operation) {
    string error = "Security Alert: User " + to_string(userId) + " - " + operation;
    log(error);
} 