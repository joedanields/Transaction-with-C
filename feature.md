# Banking System Features

## Current Features

### 1. **Account Management**
- **Add New Account**: Create new customer accounts with account number, first name, last name, and initial balance
- **Delete Account**: Remove existing accounts from the system
- **View Account Details**: Display specific account information including balance and customer details

### 2. **Transaction Processing**
- **Update Account Balance**: Process transactions (charges and payments) to modify account balances
- **Balance Tracking**: Maintain real-time balance updates for all accounts

### 3. **File Operations**
- **Binary File Storage**: Store account data in `clients.dat` binary file for persistent storage
- **Text File Export**: Generate readable `accounts.txt` file with formatted account listings
- **Automatic File Creation**: Create data file if it doesn't exist on first run

### 4. **System Features**
- **Menu-driven Interface**: User-friendly console menu for navigation
- **Input Validation**: Basic validation for account numbers and ranges
- **Multi-account Support**: Handle up to 100 accounts simultaneously

---

## Suggested New Features (Beginner-Friendly)

### 1. **Account Search by Name**
**Description**: Add functionality to search for accounts using customer's first or last name instead of just account number.
- Add a new menu option "Search Account by Name"
- Allow users to enter partial names for flexible searching
- Display all matching accounts with their details

### 2. **Transaction History Log**
 
**Description**: Keep a simple log of recent transactions for each account.
- Add a transaction history array to the client structure
- Store last 5-10 transactions with date and amount
- Add menu option to view transaction history for an account
- Show transaction type (deposit/withdrawal) and remaining balance

### 3. **Account Summary Report**

**Description**: Generate basic statistics and summary information about all accounts.
- Add menu option for "Account Summary"
- Display total number of active accounts
- Show total money in the bank (sum of all balances)
- List accounts with highest and lowest balances
- Show average account balance

### 4. **Simple Account Backup**

**Description**: Create a backup copy of the account database.
- Add menu option for "Backup Accounts"
- Copy `clients.dat` to `clients_backup.dat` with timestamp
- Add option to restore from backup
- Show backup creation date and time
- Simple confirmation messages for backup operations

