# Enhanced Banking System

## Overview
This is an enhanced banking system application written in C that manages client accounts with additional features beyond basic account management. The system maintains account information in a binary file and provides various operations including transaction history tracking, account searching, and backup/restore functionality.

## Features

### Core Features
1. **Account Management**
   - Create new accounts
   - Update account balances (deposits/withdrawals)
   - Delete accounts
   - View account details

2. **File Operations**
   - Create a human-readable text file from binary data
   - Maintain data in a binary file for efficient storage

### Enhanced Features
1. **Transaction History**
   - Track all deposits and withdrawals
   - View complete transaction history for any account
   - Automatic timestamp recording for each transaction

2. **Search Functionality**
   - Search accounts by name (first or last)
   - Case-insensitive search

3. **Reporting**
   - Generate comprehensive account summary
   - Display statistics including:
     - Total active accounts
     - Total bank balance
     - Highest/lowest balances
     - Average balance

4. **Data Protection**
   - Create timestamped backups
   - Restore from backup files
   - Confirmation prompt before overwriting data

5. **User Interface**
   - Clear menu system
   - Input validation
   - Informative prompts and messages

## Data Structure
The system uses a `clientData` structure that contains:
- Account number
- Client name (first and last)
- Current balance
- Transaction history (up to 10 transactions per account)
  - Transaction type (Deposit/Withdraw/Initial)
  - Amount
  - Timestamp
  - Balance after transaction

## File Handling
- **Main data file**: `clients.dat` (binary format)
- **Text export**: `accounts.txt`
- **Backup files**: Automatically named with timestamp (e.g., `clients_backup_2024_01_15_10_30_45.dat`)

## Usage
1. Compile the program using a C compiler (e.g., gcc)
2. Run the executable
3. Follow the menu prompts to perform operations

### Menu Options
1. Create printable accounts text file
2. Update an account
3. Add a new account
4. Delete an account
5. View account details
6. Search account by name
7. View transaction history
8. Generate account summary
9. Backup accounts
10. Restore from backup
11. Exit

## Technical Details
- Maximum accounts: 100
- Maximum transactions per account: 10
- Case-insensitive name search
- Automatic timestamp generation
- Input validation for account numbers and amounts

## Requirements
- Standard C library
- POSIX-compliant system for timestamp functionality

## Notes
- The program automatically creates a new data file if one doesn't exist
- Backup files preserve all account data including transaction histories
- The system prevents account number conflicts