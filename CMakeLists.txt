cmake_minimum_required(VERSION 3.10)
project(BankManagementSystem)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include directories
include_directories(${PROJECT_SOURCE_DIR}/include)

# Add source files
set(SOURCES
        src/main.cpp
        src/file_handler.cpp
        src/user.cpp
        src/bank_employee.cpp
        src/user_client.cpp
        src/company_client.cpp
        src/account_type.cpp
        src/bronze_account.cpp
        src/gold_account.cpp
        src/business_account.cpp
        src/card.cpp
        src/transaction.cpp
        include/account_type.h
        src/error_logger.cpp
        src/error_logger.cpp
)

# Create executable
add_executable(bank_management ${SOURCES})
