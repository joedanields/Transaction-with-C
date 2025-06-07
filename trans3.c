#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_ACCOUNTS 100
#define LAST_NAME_LEN 15
#define FIRST_NAME_LEN 10
#define MAX_TRANSACTIONS 10
#define DATE_LEN 20

// Transaction structure for history
struct transaction {
    char date[DATE_LEN];
    double amount;
    char type[10]; // "Deposit" or "Withdraw"
    double balance_after;
};

// Enhanced structure definition
struct clientData {
    unsigned int acctNum;
    char lastName[LAST_NAME_LEN];
    char firstName[FIRST_NAME_LEN];
    double balance;
    struct transaction history[MAX_TRANSACTIONS];
    int transaction_count;
};

// Function prototypes - Original functions
unsigned int enterChoice(void);
void createTextFile(FILE *readPtr);
void updateRecord(FILE *fPtr);
void addAccount(FILE *fPtr);
void deleteAccount(FILE *fPtr);
void displayAccount(FILE *fPtr);
void clearInputBuffer(void);

// New function prototypes
void searchAccountByName(FILE *fPtr);
void viewTransactionHistory(FILE *fPtr);
void generateAccountSummary(FILE *fPtr);
void backupAccounts(FILE *fPtr);
void restoreBackup(FILE **fPtr);
void addTransaction(struct clientData *client, double amount, const char* type);
void getCurrentDateTime(char *dateTime);

// Main function
int main(void) {
    FILE *cfPtr;
    unsigned int choice;

    if ((cfPtr = fopen("clients.dat", "rb+")) == NULL) {
        puts("File could not be opened. Creating a new file...");
        cfPtr = fopen("clients.dat", "wb+");

        if (cfPtr == NULL) {
            puts("Error: Could not create clients.dat");
            return EXIT_FAILURE;
        }

        // Initialize file with blank records
        struct clientData blankClient = {0, "", "", 0.0, {}, 0};
        for (int i = 0; i < MAX_ACCOUNTS; ++i)
            fwrite(&blankClient, sizeof(struct clientData), 1, cfPtr);
    }

    while ((choice = enterChoice()) != 11) { // Updated exit option
        switch (choice) {
            case 1: createTextFile(cfPtr); break;
            case 2: updateRecord(cfPtr); break;
            case 3: addAccount(cfPtr); break;
            case 4: deleteAccount(cfPtr); break;
            case 5: displayAccount(cfPtr); break;
            case 6: searchAccountByName(cfPtr); break;          // NEW FEATURE
            case 7: viewTransactionHistory(cfPtr); break;       // NEW FEATURE
            case 8: generateAccountSummary(cfPtr); break;       // NEW FEATURE
            case 9: backupAccounts(cfPtr); break;               // NEW FEATURE
            case 10: restoreBackup(&cfPtr); break;              // NEW FEATURE
            default: puts("Invalid choice. Try again."); break;
        }
    }

    fclose(cfPtr);
    puts("Program ended.");
    return 0;
}

// Enhanced Menu
unsigned int enterChoice(void) {
    unsigned int choice;
    puts("\n*** Enhanced Banking System Menu ***");
    puts("=== Original Features ===");
    puts("1 - Create printable accounts text file");
    puts("2 - Update an account");
    puts("3 - Add a new account");
    puts("4 - Delete an account");
    puts("5 - View account details");
    puts("=== New Features ===");
    puts("6 - Search account by name");
    puts("7 - View transaction history");
    puts("8 - Generate account summary");
    puts("9 - Backup accounts");
    puts("10 - Restore from backup");
    puts("11 - Exit");
    printf("Enter your choice: ");
    scanf("%u", &choice);
    clearInputBuffer();
    return choice;
}

// Clear input buffer
void clearInputBuffer(void) {
    while (getchar() != '\n');
}

// Create a text file from binary
void createTextFile(FILE *readPtr) {
    FILE *writePtr;
    struct clientData client;

    if ((writePtr = fopen("accounts.txt", "w")) == NULL) {
        puts("Could not open accounts.txt for writing.");
        return;
    }

    rewind(readPtr);
    fprintf(writePtr, "%-6s%-16s%-11s%10s\n", "Acct", "Last Name", "First Name", "Balance");

    while (fread(&client, sizeof(struct clientData), 1, readPtr) == 1) {
        if (client.acctNum != 0) {
            fprintf(writePtr, "%-6u%-16s%-11s%10.2f\n",
                    client.acctNum, client.lastName, client.firstName, client.balance);
        }
    }

    fclose(writePtr);
    puts("accounts.txt created.");
}

// Enhanced update record with transaction history
void updateRecord(FILE *fPtr) {
    struct clientData client;
    unsigned int account;
    double transaction;

    printf("Enter account to update (1 - %d): ", MAX_ACCOUNTS);
    scanf("%u", &account);
    clearInputBuffer();

    if (account < 1 || account > MAX_ACCOUNTS) {
        puts("Invalid account number.");
        return;
    }

    fseek(fPtr, (account - 1) * sizeof(struct clientData), SEEK_SET);
    fread(&client, sizeof(struct clientData), 1, fPtr);

    if (client.acctNum == 0) {
        printf("Account #%u not found.\n", account);
    } else {
        printf("Current balance: %.2f\n", client.balance);
        printf("Enter transaction amount (+deposit or -withdrawal): ");
        scanf("%lf", &transaction);
        clearInputBuffer();

        client.balance += transaction;
        
        // Add transaction to history
        const char* type = (transaction >= 0) ? "Deposit" : "Withdraw";
        addTransaction(&client, transaction, type);

        fseek(fPtr, (account - 1) * sizeof(struct clientData), SEEK_SET);
        fwrite(&client, sizeof(struct clientData), 1, fPtr);
        printf("New balance: %.2f\n", client.balance);
        puts("Transaction recorded in history.");
    }
}

// Add new account
void addAccount(FILE *fPtr) {
    struct clientData client = {0, "", "", 0.0, {}, 0};
    unsigned int account;

    printf("Enter new account number (1 - %d): ", MAX_ACCOUNTS);
    scanf("%u", &account);
    clearInputBuffer();

    if (account < 1 || account > MAX_ACCOUNTS) {
        puts("Invalid account number.");
        return;
    }

    fseek(fPtr, (account - 1) * sizeof(struct clientData), SEEK_SET);
    fread(&client, sizeof(struct clientData), 1, fPtr);

    if (client.acctNum != 0) {
        puts("Account already exists.");
        return;
    }

    client.acctNum = account;

    printf("Enter last name: ");
    scanf("%14s", client.lastName);
    clearInputBuffer();

    printf("Enter first name: ");
    scanf("%9s", client.firstName);
    clearInputBuffer();

    printf("Enter initial balance: ");
    scanf("%lf", &client.balance);
    clearInputBuffer();

    // Add initial balance as first transaction if > 0
    if (client.balance > 0) {
        addTransaction(&client, client.balance, "Initial");
    }

    fseek(fPtr, (account - 1) * sizeof(struct clientData), SEEK_SET);
    fwrite(&client, sizeof(struct clientData), 1, fPtr);

    puts("Account created successfully.");
}

// Delete account
void deleteAccount(FILE *fPtr) {
    struct clientData client;
    struct clientData blankClient = {0, "", "", 0.0, {}, 0};
    unsigned int account;

    printf("Enter account to delete (1 - %d): ", MAX_ACCOUNTS);
    scanf("%u", &account);
    clearInputBuffer();

    if (account < 1 || account > MAX_ACCOUNTS) {
        puts("Invalid account number.");
        return;
    }

    fseek(fPtr, (account - 1) * sizeof(struct clientData), SEEK_SET);
    fread(&client, sizeof(struct clientData), 1, fPtr);

    if (client.acctNum == 0) {
        puts("Account does not exist.");
    } else {
        fseek(fPtr, (account - 1) * sizeof(struct clientData), SEEK_SET);
        fwrite(&blankClient, sizeof(struct clientData), 1, fPtr);
        puts("Account deleted.");
    }
}

// Display specific account
void displayAccount(FILE *fPtr) {
    struct clientData client;
    unsigned int account;

    printf("Enter account number to view (1 - %d): ", MAX_ACCOUNTS);
    scanf("%u", &account);
    clearInputBuffer();

    if (account < 1 || account > MAX_ACCOUNTS) {
        puts("Invalid account number.");
        return;
    }

    fseek(fPtr, (account - 1) * sizeof(struct clientData), SEEK_SET);
    fread(&client, sizeof(struct clientData), 1, fPtr);

    if (client.acctNum == 0) {
        puts("No record found.");
    } else {
        printf("Account #%u\nLast Name: %s\nFirst Name: %s\nBalance: %.2f\n",
               client.acctNum, client.lastName, client.firstName, client.balance);
        printf("Total Transactions: %d\n", client.transaction_count);
    }
}

// NEW FEATURE 1: Search account by name
void searchAccountByName(FILE *fPtr) {
    struct clientData client;
    char searchName[20];
    int found = 0;

    printf("Enter name to search (first or last name): ");
    scanf("%19s", searchName);
    clearInputBuffer();

    // Convert search term to lowercase for case-insensitive search
    for (int i = 0; searchName[i]; i++) {
        if (searchName[i] >= 'A' && searchName[i] <= 'Z') {
            searchName[i] = searchName[i] + 32;
        }
    }

    rewind(fPtr);
    printf("\n=== Search Results ===\n");
    printf("%-6s%-16s%-11s%10s\n", "Acct", "Last Name", "First Name", "Balance");
    printf("---------------------------------------------------\n");

    while (fread(&client, sizeof(struct clientData), 1, fPtr) == 1) {
        if (client.acctNum != 0) {
            char tempLast[LAST_NAME_LEN], tempFirst[FIRST_NAME_LEN];
            strcpy(tempLast, client.lastName);
            strcpy(tempFirst, client.firstName);

            // Convert to lowercase for comparison
            for (int i = 0; tempLast[i]; i++) {
                if (tempLast[i] >= 'A' && tempLast[i] <= 'Z') {
                    tempLast[i] = tempLast[i] + 32;
                }
            }
            for (int i = 0; tempFirst[i]; i++) {
                if (tempFirst[i] >= 'A' && tempFirst[i] <= 'Z') {
                    tempFirst[i] = tempFirst[i] + 32;
                }
            }

            // Check if search term is found in either name
            if (strstr(tempLast, searchName) != NULL || strstr(tempFirst, searchName) != NULL) {
                printf("%-6u%-16s%-11s%10.2f\n",
                       client.acctNum, client.lastName, client.firstName, client.balance);
                found++;
            }
        }
    }

    if (found == 0) {
        printf("No accounts found matching '%s'\n", searchName);
    } else {
        printf("\nTotal accounts found: %d\n", found);
    }
}

// NEW FEATURE 2: View transaction history
void viewTransactionHistory(FILE *fPtr) {
    struct clientData client;
    unsigned int account;

    printf("Enter account number to view history (1 - %d): ", MAX_ACCOUNTS);
    scanf("%u", &account);
    clearInputBuffer();

    if (account < 1 || account > MAX_ACCOUNTS) {
        puts("Invalid account number.");
        return;
    }

    fseek(fPtr, (account - 1) * sizeof(struct clientData), SEEK_SET);
    fread(&client, sizeof(struct clientData), 1, fPtr);

    if (client.acctNum == 0) {
        puts("Account not found.");
        return;
    }

    printf("\n=== Transaction History for Account #%u ===\n", client.acctNum);
    printf("Account Holder: %s %s\n", client.firstName, client.lastName);
    printf("Current Balance: %.2f\n\n", client.balance);

    if (client.transaction_count == 0) {
        puts("No transaction history available.");
        return;
    }

    printf("%-20s%-10s%-12s%-12s\n", "Date", "Type", "Amount", "Balance After");
    printf("---------------------------------------------------------------\n");

    for (int i = 0; i < client.transaction_count; i++) {
        printf("%-20s%-10s%12.2f%12.2f\n",
               client.history[i].date,
               client.history[i].type,
               client.history[i].amount,
               client.history[i].balance_after);
    }
}

// NEW FEATURE 3: Generate account summary
void generateAccountSummary(FILE *fPtr) {
    struct clientData client;
    int activeAccounts = 0;
    double totalBalance = 0.0;
    double highestBalance = -1.0;
    double lowestBalance = -1.0;
    unsigned int highestAcct = 0, lowestAcct = 0;

    rewind(fPtr);

    printf("\n=== ACCOUNT SUMMARY REPORT ===\n");
    printf("Generated on: ");
    
    // Display current date
    time_t now;
    time(&now);
    printf("%s", ctime(&now));
    printf("=====================================\n");

    while (fread(&client, sizeof(struct clientData), 1, fPtr) == 1) {
        if (client.acctNum != 0) {
            activeAccounts++;
            totalBalance += client.balance;

            // Track highest balance
            if (highestBalance < 0 || client.balance > highestBalance) {
                highestBalance = client.balance;
                highestAcct = client.acctNum;
            }

            // Track lowest balance
            if (lowestBalance < 0 || client.balance < lowestBalance) {
                lowestBalance = client.balance;
                lowestAcct = client.acctNum;
            }
        }
    }

    printf("Total Active Accounts: %d\n", activeAccounts);
    printf("Total Bank Balance: $%.2f\n", totalBalance);

    if (activeAccounts > 0) {
        printf("Average Account Balance: $%.2f\n", totalBalance / activeAccounts);
        printf("Highest Balance: $%.2f (Account #%u)\n", highestBalance, highestAcct);
        printf("Lowest Balance: $%.2f (Account #%u)\n", lowestBalance, lowestAcct);
    }

    printf("Maximum Capacity: %d accounts\n", MAX_ACCOUNTS);
    printf("Available Slots: %d\n", MAX_ACCOUNTS - activeAccounts);
    printf("=====================================\n");
}

// NEW FEATURE 4: Backup accounts
void backupAccounts(FILE *fPtr) {
    FILE *backupPtr;
    char backupName[50];
    char dateTime[30];
    struct clientData client;
    int recordsCopied = 0;

    // Generate backup filename with timestamp
    getCurrentDateTime(dateTime);
    sprintf(backupName, "clients_backup_%s.dat", dateTime);

    // Replace spaces and colons with underscores in the filename
    for (int i = 0; backupName[i]; i++) {
        if (backupName[i] == ' ' || backupName[i] == ':') {
            backupName[i] = '_';
        }
    }

    if ((backupPtr = fopen(backupName, "wb")) == NULL) {
        puts("Error: Could not create backup file.");
        return;
    }

    rewind(fPtr);
    
    // Copy all records
    while (fread(&client, sizeof(struct clientData), 1, fPtr) == 1) {
        fwrite(&client, sizeof(struct clientData), 1, backupPtr);
        recordsCopied++;
    }

    fclose(backupPtr);
    printf("Backup completed successfully!\n");
    printf("Backup file: %s\n", backupName);
    printf("Records backed up: %d\n", recordsCopied);
    
    time_t now;
    time(&now);
    printf("Backup created on: %s", ctime(&now));
}

// NEW FEATURE 4B: Restore from backup
void restoreBackup(FILE **fPtr) {
    FILE *backupPtr;
    char backupName[50];
    struct clientData client;
    int recordsRestored = 0;
    char confirm;

    printf("Enter backup filename (e.g., clients_backup_2024_01_15_10_30_45.dat): ");
    scanf("%49s", backupName);
    clearInputBuffer();

    if ((backupPtr = fopen(backupName, "rb")) == NULL) {
        puts("Error: Backup file not found.");
        return;
    }

    printf("WARNING: This will overwrite current data. Continue? (y/n): ");
    scanf("%c", &confirm);
    clearInputBuffer();

    if (confirm != 'y' && confirm != 'Y') {
        fclose(backupPtr);
        puts("Restore cancelled.");
        return;
    }

    // Close current file and reopen for writing
    fclose(*fPtr);
    *fPtr = fopen("clients.dat", "wb");

    if (*fPtr == NULL) {
        puts("Error: Could not open main file for writing.");
        fclose(backupPtr);
        return;
    }

    // Copy all records from backup
    while (fread(&client, sizeof(struct clientData), 1, backupPtr) == 1) {
        fwrite(&client, sizeof(struct clientData), 1, *fPtr);
        recordsRestored++;
    }

    fclose(backupPtr);
    fclose(*fPtr);

    // Reopen in read-write mode
    *fPtr = fopen("clients.dat", "rb+");

    printf("Restore completed successfully!\n");
    printf("Records restored: %d\n", recordsRestored);
    puts("System ready with restored data.");
}

// Helper function: Add transaction to history
void addTransaction(struct clientData *client, double amount, const char* type) {
    char dateTime[DATE_LEN];
    getCurrentDateTime(dateTime);

    // Shift older transactions if we're at max capacity
    if (client->transaction_count >= MAX_TRANSACTIONS) {
        for (int i = 0; i < MAX_TRANSACTIONS - 1; i++) {
            client->history[i] = client->history[i + 1];
        }
        client->transaction_count = MAX_TRANSACTIONS - 1;
    }

    // Add new transaction
    strcpy(client->history[client->transaction_count].date, dateTime);
    client->history[client->transaction_count].amount = amount;
    strcpy(client->history[client->transaction_count].type, type);
    client->history[client->transaction_count].balance_after = client->balance;
    client->transaction_count++;
}

// Helper function: Get current date and time
void getCurrentDateTime(char *dateTime) {
    time_t now;
    struct tm *timeinfo;

    time(&now);
    timeinfo = localtime(&now);
    
    strftime(dateTime, 20, "%Y_%m_%d_%H_%M_%S", timeinfo);
}