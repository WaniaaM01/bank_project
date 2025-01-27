#ifndef ERROR_LOGGER_H
#define ERROR_LOGGER_H

#include <string>
#include <fstream>
#include <ctime>
#include <iostream>

using namespace std;

class ErrorLogger {
private:
    static const string ERROR_LOG_FILE;
    static ofstream logFile;
    
    static string getCurrentTimestamp();
    static bool openLogFile();

public:
    static void log(const string& error);
    static void logFileError(const string& filename, const string& operation);
    static void logTransactionError(int userId, const string& operation);
    static void logSecurityError(int userId, const string& operation);
};

#endif 