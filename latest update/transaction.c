#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_ACCOUNTS 100
#define NAME_LENGTH 50


struct Account {
    int accountNumber;
    char firstName[NAME_LENGTH];
    char lastName[NAME_LENGTH];
    double balance;
    char pin[5];
    int isActive;
};

// Global variables
struct Account accounts[MAX_ACCOUNTS];
int totalAccounts = 0;

// Function prototypes
void loadAccounts();
void saveAccounts();
void createAccount();
void displayAllAccounts();
void searchByName();
void sortAccounts();
void processTransaction();
int authenticateUser(int accountIndex);
void generateReceipt(int accountNumber, char* transactionType, double amount, double newBalance);
void showTransactionConfirmation(int success, char* operation);
int findAccountByNumber(int accountNumber);


int main() {
    int choice;
    
    loadAccounts();
    
    while(1) {
        printf("\n=== ENHANCED BANK TRANSACTION SYSTEM ===\n");
        printf("1. Create New Account\n");
        printf("2. Display All Accounts\n");
        printf("3. Search Account by Name\n");
        printf("4. Sort Accounts\n");
        printf("5. Process Transaction\n");
        printf("6. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        
        switch(choice) {
            case 1:
                createAccount();
                break;
            case 2:
                displayAllAccounts();
                break;
            case 3:
                searchByName();
                break;
            case 4:
                sortAccounts();
                break;
            case 5:
                processTransaction();
                break;
            case 6:
                saveAccounts();
                printf("Thank you for using our banking system!\n");
                exit(0);
            default:
                printf("Invalid choice! Please try again.\n");
        }
    }
    return 0;
}

void loadAccounts() {
    FILE *file = fopen("accounts.dat", "rb");
    if (file != NULL) {
        fread(&totalAccounts, sizeof(int), 1, file);
        fread(accounts, sizeof(struct Account), totalAccounts, file);
        fclose(file);
        printf("Loaded %d accounts from database.\n", totalAccounts);
    } else {
        printf("No existing database found. Starting fresh.\n");
        totalAccounts = 0;
    }
}

void saveAccounts() {
    FILE *file = fopen("accounts.dat", "wb");
    if (file != NULL) {
        fwrite(&totalAccounts, sizeof(int), 1, file);
        fwrite(accounts, sizeof(struct Account), totalAccounts, file);
        fclose(file);
        printf("Database saved successfully.\n");
    } else {
        printf("Error saving database!\n");
    }
}

void createAccount() {
    if (totalAccounts >= MAX_ACCOUNTS) {
        printf("Maximum account limit reached!\n");
        return;
    }
    
    struct Account newAccount;
    
    printf("\n=== CREATE NEW ACCOUNT ===\n");
    printf("Enter account number: ");
    scanf("%d", &newAccount.accountNumber);
    
    // Check if account number already exists
    if (findAccountByNumber(newAccount.accountNumber) != -1) {
        printf("Account number already exists!\n");
        return;
    }
    
    printf("Enter first name: ");
    scanf("%s", newAccount.firstName);
    printf("Enter last name: ");
    scanf("%s", newAccount.lastName);
    printf("Enter initial balance: $");
    scanf("%lf", &newAccount.balance);
    printf("Set a 4-digit PIN: ");
    scanf("%s", newAccount.pin);
    
    newAccount.isActive = 1;
    
    accounts[totalAccounts] = newAccount;
    totalAccounts++;
    
    printf("Account created successfully!\n");
    saveAccounts();
}

void displayAllAccounts() {
    if (totalAccounts == 0) {
        printf("No accounts found!\n");
        return;
    }
    
    printf("\n=== ALL ACCOUNTS ===\n");
    printf("%-10s %-15s %-15s %-12s %-8s\n", "Acc No.", "First Name", "Last Name", "Balance", "Status");
    printf("----------------------------------------------------------------\n");
    
    for (int i = 0; i < totalAccounts; i++) {
        printf("%-10d %-15s %-15s $%-11.2f %-8s\n", 
               accounts[i].accountNumber,
               accounts[i].firstName,
               accounts[i].lastName,
               accounts[i].balance,
               accounts[i].isActive ? "Active" : "Inactive");
    }
}

// FEATURE 4: Search by Name
void searchByName() {
    char searchName[NAME_LENGTH];
    int found = 0;
    
    printf("\n=== SEARCH BY NAME ===\n");
    printf("Enter first or last name to search: ");
    scanf("%s", searchName);
    
    printf("\nSearch Results:\n");
    printf("%-10s %-15s %-15s %-12s\n", "Acc No.", "First Name", "Last Name", "Balance");
    printf("--------------------------------------------------------\n");
    
    for (int i = 0; i < totalAccounts; i++) {
        if (strstr(accounts[i].firstName, searchName) != NULL || 
            strstr(accounts[i].lastName, searchName) != NULL) {
            printf("%-10d %-15s %-15s $%-11.2f\n", 
                   accounts[i].accountNumber,
                   accounts[i].firstName,
                   accounts[i].lastName,
                   accounts[i].balance);
            found = 1;
        }
    }
    
    if (!found) {
        printf("No accounts found with name containing '%s'\n", searchName);
    }
}

// FEATURE 5: Sort Accounts
void sortAccounts() {
    int choice;
    
    if (totalAccounts == 0) {
        printf("No accounts to sort!\n");
        return;
    }
    
    printf("\n=== SORT ACCOUNTS ===\n");
    printf("1. Sort by Account Number\n");
    printf("2. Sort by Name (First Name)\n");
    printf("3. Sort by Balance\n");
    printf("Enter sorting option: ");
    scanf("%d", &choice);
    
    if (choice < 1 || choice > 3) {
        printf("Invalid sorting option!\n");
        return;
    }
    
    // Simple bubble sort
    for (int i = 0; i < totalAccounts - 1; i++) {
        for (int j = 0; j < totalAccounts - i - 1; j++) {
            int swap = 0;
            
            switch(choice) {
                case 1: // Sort by account number
                    if (accounts[j].accountNumber > accounts[j + 1].accountNumber)
                        swap = 1;
                    break;
                case 2: // Sort by first name
                    if (strcmp(accounts[j].firstName, accounts[j + 1].firstName) > 0)
                        swap = 1;
                    break;
                case 3: // Sort by balance
                    if (accounts[j].balance > accounts[j + 1].balance)
                        swap = 1;
                    break;
            }
            
            if (swap) {
                struct Account temp = accounts[j];
                accounts[j] = accounts[j + 1];
                accounts[j + 1] = temp;
            }
        }
    }
    
    printf("Accounts sorted successfully!\n");
    displayAllAccounts();
}

// FIXED TRANSACTION PROCESSING - Bug corrected
void processTransaction() {
    int accountNumber, choice;
    double amount;
    
    printf("\n=== PROCESS TRANSACTION ===\n");
    printf("Enter account number: ");
    scanf("%d", &accountNumber);
    
    int accountIndex = findAccountByNumber(accountNumber);
    if (accountIndex == -1) {
        printf("Account not found!\n");
        showTransactionConfirmation(0, "Account Lookup");
        return;
    }
    
    // FEATURE 1: Simple Password Protection
    if (!authenticateUser(accountIndex)) {
        printf("Authentication failed!\n");
        showTransactionConfirmation(0, "Authentication");
        return;
    }
    
    printf("\n1. Deposit\n2. Withdraw\nEnter transaction type: ");
    scanf("%d", &choice);
    
    // INPUT VALIDATION - This was the missing piece!
    if (choice != 1 && choice != 2) {
        printf("Invalid transaction type! Please enter 1 for Deposit or 2 for Withdraw.\n");
        showTransactionConfirmation(0, "Invalid Input");
        return;
    }
    
    printf("Enter amount: $");
    scanf("%lf", &amount);
    
    // Amount validation
    if (amount <= 0) {
        printf("Invalid amount! Amount must be greater than 0.\n");
        showTransactionConfirmation(0, "Invalid Amount");
        return;
    }
    
    double oldBalance = accounts[accountIndex].balance;
    
    if (choice == 1) { // Deposit
        accounts[accountIndex].balance += amount;
        showTransactionConfirmation(1, "Deposit");
        generateReceipt(accountNumber, "DEPOSIT", amount, accounts[accountIndex].balance);
        saveAccounts(); // Only save after successful transaction
    } else if (choice == 2) { // Withdraw
        if (accounts[accountIndex].balance >= amount) {
            accounts[accountIndex].balance -= amount;
            showTransactionConfirmation(1, "Withdrawal");
            generateReceipt(accountNumber, "WITHDRAWAL", amount, accounts[accountIndex].balance);
            saveAccounts(); // Only save after successful transaction
        } else {
            printf("Insufficient balance! Available: $%.2f\n", accounts[accountIndex].balance);
            showTransactionConfirmation(0, "Insufficient Funds");
        }
    }
}

// FEATURE 1: Simple Password Protection
int authenticateUser(int accountIndex) {
    char enteredPin[5];
    int attempts = 3;
    
    while (attempts > 0) {
        printf("Enter your 4-digit PIN: ");
        scanf("%s", enteredPin);
        
        if (strcmp(accounts[accountIndex].pin, enteredPin) == 0) {
            printf("Authentication successful!\n");
            return 1;
        } else {
            attempts--;
            if (attempts > 0) {
                printf("Incorrect PIN! %d attempts remaining.\n", attempts);
            }
        }
    }
    return 0;
}

// FEATURE 2: Transaction Confirmation
void showTransactionConfirmation(int success, char* operation) {
    printf("\n=== TRANSACTION CONFIRMATION ===\n");
    if (success) {
        printf("✓ %s completed successfully!\n", operation);
    } else {
        printf("✗ %s failed!\n", operation);
    }
    printf("================================\n");
}

// FEATURE 3: Transaction Receipts
void generateReceipt(int accountNumber, char* transactionType, double amount, double newBalance) {
    time_t now;
    time(&now);
    
    printf("\n=== TRANSACTION RECEIPT ===\n");
    printf("Date & Time: %s", ctime(&now));
    printf("Account Number: %d\n", accountNumber);
    printf("Transaction Type: %s\n", transactionType);
    printf("Amount: $%.2f\n", amount);
    printf("New Balance: $%.2f\n", newBalance);
    printf("===========================\n");
    
    // Also save receipt to file
    char filename[50];
    sprintf(filename, "receipt_%d_%ld.txt", accountNumber, (long)now);
    
    FILE *receiptFile = fopen(filename, "w");
    if (receiptFile != NULL) {
        fprintf(receiptFile, "=== TRANSACTION RECEIPT ===\n");
        fprintf(receiptFile, "Date & Time: %s", ctime(&now));
        fprintf(receiptFile, "Account Number: %d\n", accountNumber);
        fprintf(receiptFile, "Transaction Type: %s\n", transactionType);
        fprintf(receiptFile, "Amount: $%.2f\n", amount);
        fprintf(receiptFile, "New Balance: $%.2f\n", newBalance);
        fprintf(receiptFile, "===========================\n");
        fclose(receiptFile);
        printf("Receipt saved as: %s\n", filename);
    }
}

int findAccountByNumber(int accountNumber) {
    for (int i = 0; i < totalAccounts; i++) {
        if (accounts[i].accountNumber == accountNumber) {
            return i;
        }
    }
    return -1;
}

