# Enhanced Banking System

A comprehensive C-based banking management system with advanced features for account management, transaction tracking, and data backup/restore capabilities.

## Features

### Core Banking Operations
- **Account Management**: Create, update, delete, and view account details
- **Transaction Processing**: Deposit and withdrawal operations with automatic balance updates
- **File Operations**: Export account data to readable text format

### Advanced Features
- **Smart Search**: Search accounts by first name or last name (case-insensitive)
- **Transaction History**: Track up to 10 recent transactions per account with timestamps
- **Account Analytics**: Generate comprehensive summary reports with statistics
- **Data Backup**: Create timestamped backups of all account data
- **Data Restore**: Restore accounts from backup files
- **Transaction Logging**: Automatic recording of all financial transactions

## System Requirements

- **Compiler**: GCC or any C99-compatible compiler
- **Operating System**: Windows, Linux, or macOS
- **Memory**: Minimal (supports up to 100 accounts)
- **Storage**: Small disk space for data files

## Installation & Compilation

### Step 1: Save the Code
Save the provided C code as `banking_system.c`

### Step 2: Compile the Program

**On Linux/macOS:**
```bash
gcc -o banking_system banking_system.c
```

**On Windows (using MinGW):**
```cmd
gcc -o banking_system.exe banking_system.c
```

**Alternative with debugging information:**
```bash
gcc -g -Wall -o banking_system banking_system.c
```

### Step 3: Run the Program

**On Linux/macOS:**
```bash
./banking_system
```

**On Windows:**
```cmd
banking_system.exe
```

## Usage Guide

### First Run
When you first run the program, it will automatically create a `clients.dat` file to store account data.

### Menu Options

#### Original Features
1. **Create Printable Accounts File** - Exports all accounts to `accounts.txt`
2. **Update Account** - Modify account balance (deposits/withdrawals)
3. **Add New Account** - Create a new customer account
4. **Delete Account** - Remove an existing account
5. **View Account Details** - Display specific account information

#### Enhanced Features
6. **Search by Name** - Find accounts using partial name matching
7. **Transaction History** - View detailed transaction log for any account
8. **Account Summary** - Generate comprehensive banking statistics
9. **Backup Accounts** - Create timestamped backup files
10. **Restore from Backup** - Restore data from backup files
11. **Exit** - Close the program safely

### Data Files Created

- `clients.dat` - Main binary database file
- `accounts.txt` - Human-readable account export
- `clients_backup_YYYY_MM_DD_HH_MM_SS.dat` - Timestamped backup files

## Account Management

### Adding New Accounts
1. Select option 3 from the menu
2. Enter account number (1-100)
3. Provide customer details (last name, first name)
4. Set initial balance
5. System automatically logs the initial deposit

### Updating Accounts
1. Select option 2 from the menu
2. Enter account number
3. Enter transaction amount:
   - Positive numbers for deposits
   - Negative numbers for withdrawals
4. System updates balance and records transaction history

### Searching Accounts
1. Select option 6 from the menu
2. Enter any part of first or last name
3. System displays all matching accounts (case-insensitive)

## Transaction History

Each account maintains a log of the last 10 transactions including:
- Transaction date and time
- Transaction type (Deposit/Withdraw/Initial)
- Amount
- Balance after transaction

When the history limit is reached, older transactions are automatically removed.

## Backup & Restore

### Creating Backups
1. Select option 9 from the menu
2. System creates a timestamped backup file
3. Backup includes all account data and transaction histories

### Restoring Data
1. Select option 10 from the menu
2. Enter the backup filename
3. Confirm the restore operation
4. System replaces current data with backup data

## Account Summary Reports

The system generates detailed reports including:
- Total number of active accounts
- Total bank balance across all accounts
- Average account balance
- Highest and lowest balance accounts
- Available account slots
- Report generation timestamp

## Technical Specifications

### Data Structure
```c
struct clientData {
    unsigned int acctNum;           // Account number (1-100)
    char lastName[15];              // Customer last name
    char firstName[10];             // Customer first name
    double balance;                 // Current account balance
    struct transaction history[10]; // Transaction history
    int transaction_count;          // Number of transactions
};
```

### File Format
- Binary file format for efficient storage and retrieval
- Fixed-size records for direct access
- Automatic file creation and initialization

### Limitations
- Maximum 100 accounts
- Maximum 10 transactions per account in history
- Names limited to 14/9 characters (last/first)
- Single-user system (no concurrent access)

## Error Handling

The system includes comprehensive error handling for:
- File access errors
- Invalid account numbers
- Invalid menu choices
- Memory allocation issues
- Backup/restore operations

## Code Quality Improvements Made

### Input Validation
- Account number range checking
- Menu choice validation
- File operation error checking

### Memory Management
- Proper file handle management
- Buffer overflow prevention
- Clean resource cleanup

### User Experience
- Clear menu structure
- Informative error messages
- Confirmation prompts for destructive operations

## Development Notes

### Potential Enhancements
1. **Multi-user Support**: Add user authentication
2. **Network Capabilities**: Enable remote access
3. **Database Integration**: Replace file system with proper database
4. **Enhanced Security**: Add encryption for sensitive data
5. **GUI Interface**: Develop graphical user interface
6. **Report Generation**: Add more detailed financial reports

### Code Structure
- Modular function design
- Clear separation of concerns
- Comprehensive error handling
- Consistent coding style

## Troubleshooting

### Common Issues

**"File could not be opened" Error:**
- Ensure you have write permissions in the current directory
- Check available disk space

**"Account not found" Error:**
- Verify account number is between 1-100
- Ensure account has been created

**Backup/Restore Issues:**
- Check backup file exists and is accessible
- Ensure proper file permissions
- Verify backup file is not corrupted

### Getting Help
If you encounter issues:
1. Check file permissions in your working directory
2. Ensure you have sufficient disk space
3. Verify your compiler supports C99 standard
4. Try running with administrator/root privileges if needed

## License

This banking system is provided as-is for educational and small business use. Feel free to modify and distribute according to your needs.

---

**Note**: This system is designed for single-user environments and educational purposes. For production banking applications, additional security measures, data validation, and regulatory compliance features would be required.