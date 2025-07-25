#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_ACCOUNTS 100
#define NAME_LENGTH 50
#define PIN_LENGTH 5

typedef struct {
    int accountNumber;
    char firstName[NAME_LENGTH];
    char lastName[NAME_LENGTH];
    double balance;
    char pin[PIN_LENGTH];
    int isActive;
} Account;

static Account accounts[MAX_ACCOUNTS];
static int totalAccounts = 0;

// Prototypes
void loadAccounts(void);
void saveAccounts(void);
void createAccount(void);
void displayAllAccounts(void);
void searchByName(void);
void sortAccounts(void);
void processTransaction(void);
void deactivateOrDeleteAccount(void);
void activateAccount(void);
int authenticateUser(const int);
void generateReceipt(const Account*, const char*, double, double);
void showTransactionConfirmation(const int, const char*);
int findAccountByNumber(const int);

// Helper for string input
static void inputString(const char *prompt, char *buffer, size_t len) {
    printf("%s", prompt);
    if (fgets(buffer, (int)len, stdin)) {
        buffer[strcspn(buffer, "\n")] = 0;
    }
}

int main(void) {
    int choice;
    loadAccounts();
    while (1) {
        printf("\n=== ENHANCED BANK TRANSACTION SYSTEM ===\n");
        printf("1. Create New Account\n");
        printf("2. Display All Accounts\n");
        printf("3. Search Account by Name\n");
        printf("4. Sort Accounts\n");
        printf("5. Process Transaction\n");
        printf("6. Deactivate/Delete Account\n");
        printf("7. Activate Account\n");
        printf("8. Exit\n");
        printf("Enter your choice: ");
        if (scanf("%d", &choice) != 1) {
            while(getchar()!='\n');
            puts("Invalid input.");
            continue;
        }
        switch(choice) {
            case 1: createAccount(); break;
            case 2: displayAllAccounts(); break;
            case 3: searchByName(); break;
            case 4: sortAccounts(); break;
            case 5: processTransaction(); break;
            case 6: deactivateOrDeleteAccount(); break;
            case 7: activateAccount(); break;
            case 8: saveAccounts(); puts("Thank you for using our banking system!"); exit(0);
            default: puts("Invalid choice! Please try again.");
        }
    }
    return 0;
}

void loadAccounts(void) {
    FILE *file = fopen("accounts.dat", "rb");
    if (file) {
        fread(&totalAccounts, sizeof(int), 1, file);
        fread(accounts, sizeof(Account), totalAccounts, file);
        fclose(file);
        printf("Loaded %d accounts from database.\n", totalAccounts);
    } else {
        puts("No existing database found. Starting fresh.");
        totalAccounts = 0;
    }
}

void saveAccounts(void) {
    FILE *file = fopen("accounts.dat", "wb");
    if (file) {
        fwrite(&totalAccounts, sizeof(int), 1, file);
        fwrite(accounts, sizeof(Account), totalAccounts, file);
        fclose(file);
        puts("Database saved successfully.");
    } else
        puts("Error saving database!");
}

void createAccount(void) {
    if (totalAccounts >= MAX_ACCOUNTS) {
        puts("Maximum account limit reached!");
        return;
    }
    Account newAccount = {0};

    puts("\n=== CREATE NEW ACCOUNT ===");
    printf("Enter account number: ");
    while (scanf("%d", &newAccount.accountNumber)!=1) {
        while(getchar()!='\n');
        puts("Enter a number for account number:");
    }
    if (findAccountByNumber(newAccount.accountNumber)!=-1) {
        puts("Account number already exists!");
        while(getchar()!='\n');
        return;
    }
    while(getchar()!='\n'); // clean up input
    inputString("Enter first name: ", newAccount.firstName, NAME_LENGTH);
    inputString("Enter last name: ", newAccount.lastName, NAME_LENGTH);

    printf("Enter initial balance: $");
    while (scanf("%lf", &newAccount.balance)!=1 || newAccount.balance<0) {
        while(getchar()!='\n');
        puts("Balance must be a positive number:");
    }
    while(getchar()!='\n');
    inputString("Set a 4-digit PIN: ", newAccount.pin, PIN_LENGTH);
    newAccount.isActive = 1;
    accounts[totalAccounts++] = newAccount;
    puts("Account created successfully!");
    saveAccounts();
}

void displayAllAccounts(void) {
    if (totalAccounts == 0) {
        puts("No accounts found!");
        return;
    }
    puts("\n=== ALL ACCOUNTS ===");
    printf("%-10s %-15s %-15s %-12s %-8s\n", "Acc No.", "First Name", "Last Name", "Balance", "Status");
    puts("----------------------------------------------------------------");
    for (int i=0; i<totalAccounts; ++i)
        printf("%-10d %-15s %-15s $%-11.2f %-8s\n",
            accounts[i].accountNumber, accounts[i].firstName,
            accounts[i].lastName, accounts[i].balance,
            accounts[i].isActive?"Active":"Inactive");
}

void searchByName(void) {
    char searchName[NAME_LENGTH];
    int found=0;
    puts("\n=== SEARCH BY NAME ===");
    printf("Enter first or last name to search: ");
    while(getchar()!='\n');
    fgets(searchName, NAME_LENGTH, stdin);
    searchName[strcspn(searchName, "\n")] = 0;
    puts("\nSearch Results:");
    printf("%-10s %-15s %-15s %-12s\n", "Acc No.", "First Name", "Last Name", "Balance");
    puts("--------------------------------------------------------");
    for (int i=0; i<totalAccounts; ++i) {
        if (strstr(accounts[i].firstName, searchName) || strstr(accounts[i].lastName, searchName)) {
            printf("%-10d %-15s %-15s $%-11.2f\n",
                accounts[i].accountNumber, accounts[i].firstName,
                accounts[i].lastName, accounts[i].balance);
            found=1;
        }
    }
    if (!found)
        printf("No accounts found with name containing '%s'\n", searchName);
}

void sortAccounts(void) {
    if (totalAccounts == 0) {
        puts("No accounts to sort!");
        return;
    }
    puts("\n=== SORT ACCOUNTS ===");
    puts("1. Sort by Account Number");
    puts("2. Sort by Name (First Name)");
    puts("3. Sort by Balance");
    printf("Enter sorting option: ");
    int choice=0; scanf("%d", &choice);
    switch (choice) {
        case 1:
            for (int i=0; i<totalAccounts-1;++i)
            for (int j=0;j<totalAccounts-1-i;++j)
                if (accounts[j].accountNumber > accounts[j+1].accountNumber) {
                    Account tmp = accounts[j]; accounts[j]=accounts[j+1]; accounts[j+1]=tmp;
                }
            break;
        case 2:
            for (int i=0; i<totalAccounts-1;++i)
            for (int j=0;j<totalAccounts-1-i;++j)
                if (strcmp(accounts[j].firstName, accounts[j+1].firstName)>0) {
                    Account tmp = accounts[j]; accounts[j]=accounts[j+1]; accounts[j+1]=tmp;
                }
            break;
        case 3:
            for (int i=0; i<totalAccounts-1;++i)
            for (int j=0;j<totalAccounts-1-i;++j)
                if (accounts[j].balance > accounts[j+1].balance) {
                    Account tmp = accounts[j]; accounts[j]=accounts[j+1]; accounts[j+1]=tmp;
                }
            break;
        default:
            puts("Invalid sorting option!");
            return;
    }
    puts("Accounts sorted successfully!");
    displayAllAccounts();
}

void processTransaction(void) {
    puts("\n=== PROCESS TRANSACTION ===");
    printf("Enter account number: ");
    int accountNumber,choice;
    double amount=0;
    if (scanf("%d", &accountNumber)!=1) { while(getchar()!='\n'); puts("Invalid account number!"); return;}
    int idx = findAccountByNumber(accountNumber);
    if (idx == -1) {
        puts("Account not found!");
        showTransactionConfirmation(0, "Account Lookup");
        return;
    }
    if (!accounts[idx].isActive) {
        puts("ERROR: This account is inactive. Contact bank for support.");
        showTransactionConfirmation(0, "Account Inactive");
        return;
    }
    if (!authenticateUser(idx)) {
        puts("Authentication failed!");
        showTransactionConfirmation(0, "Authentication");
        return;
    }
    puts("\n1. Deposit\n2. Withdraw");
    printf("Enter transaction type: ");
    if(scanf("%d",&choice)!=1 || (choice!=1 && choice!=2)) {
        puts("Invalid transaction type!");
        showTransactionConfirmation(0, "Invalid Input");
        return;
    }
    printf("Enter amount: $");
    if(scanf("%lf",&amount)!=1 || amount<=0) {
        puts("Invalid amount! Amount must be greater than 0.");
        showTransactionConfirmation(0, "Invalid Amount");
        return;
    }
    Account *acct = &accounts[idx];
    if(choice==1) {
        acct->balance+=amount;
        showTransactionConfirmation(1, "Deposit");
        generateReceipt(acct, "DEPOSIT", amount, acct->balance);
        saveAccounts();
    } else if(choice==2) {
        if(acct->balance>=amount) {
            acct->balance-=amount;
            showTransactionConfirmation(1, "Withdrawal");
            generateReceipt(acct, "WITHDRAWAL", amount, acct->balance);
            saveAccounts();
        } else {
            printf("Insufficient balance! Available: $%.2f\n", acct->balance);
            showTransactionConfirmation(0, "Insufficient Funds");
        }
    }
}

int authenticateUser(const int accountIndex) {
    for(int attempts=3; attempts>0; --attempts) {
        char enteredPin[PIN_LENGTH];
        printf("Enter your 4-digit PIN: ");
        scanf("%4s", enteredPin);
        while(getchar()!='\n'); // flush
        if(!strncmp(accounts[accountIndex].pin, enteredPin, 4)) {
            puts("Authentication successful!");
            return 1;
        } else if(attempts>1)
            printf("Incorrect PIN! %d attempts remaining.\n", attempts-1);
    }
    return 0;
}

void showTransactionConfirmation(const int success, const char* operation) {
    puts("\n=== TRANSACTION CONFIRMATION ===");
    if(success)
        printf("✓ %s completed successfully!\n", operation);
    else
        printf("✗ %s failed!\n", operation);
    puts("================================");
}

void generateReceipt(const Account *acct, const char *transactionType, double amount, double newBalance) {
    time_t now; time(&now);
    printf("\n=== TRANSACTION RECEIPT ===\n");
    printf("Date & Time: %s", ctime(&now));
    printf("Account Number: %d\n", acct->accountNumber);
    printf("Transaction Type: %s\n", transactionType);
    printf("Amount: $%.2f\n", amount);
    printf("New Balance: $%.2f\n", newBalance);
    puts("===========================");

    char filename[64];
    snprintf(filename, sizeof(filename), "receipt_%d_%ld.txt", acct->accountNumber, (long)now);
    FILE *f = fopen(filename, "w");
    if(f) {
        fprintf(f, "=== TRANSACTION RECEIPT ===\n");
        fprintf(f, "Date & Time: %s", ctime(&now));
        fprintf(f, "Account Number: %d\n", acct->accountNumber);
        fprintf(f, "Transaction Type: %s\n", transactionType);
        fprintf(f, "Amount: $%.2f\n", amount);
        fprintf(f, "New Balance: $%.2f\n", newBalance);
        fputs("===========================\n",f);
        fclose(f);
        printf("Receipt saved as: %s\n", filename);
    }
}

int findAccountByNumber(const int accountNumber) {
    for(int i=0; i<totalAccounts; ++i)
        if(accounts[i].accountNumber==accountNumber) return i;
    return -1;
}

// FEATURE: Deactivate or Delete an Account
void deactivateOrDeleteAccount(void) {
    int accNum;
    printf("\n=== DEACTIVATE OR DELETE ACCOUNT ===\n");
    printf("Enter account number: ");
    if (scanf("%d", &accNum) != 1) {
        puts("Invalid input! Returning to menu.");
        while(getchar()!='\n');
        return;
    }
    int idx = findAccountByNumber(accNum);
    if (idx == -1) {
        puts("Account not found!");
        return;
    }
    Account *acct = &accounts[idx];
    printf("Account found: %s %s | Status: %s\n",
           acct->firstName, acct->lastName,
           acct->isActive ? "Active" : "Inactive");

    printf("What do you want to do?\n");
    printf("1. Deactivate Account\n");
    printf("2. Delete Account PERMANENTLY\n");
    printf("Enter choice: ");
    int choice;
    if (scanf("%d", &choice) != 1) {
        puts("Invalid input!");
        while(getchar()!='\n');
        return;
    }
    while(getchar()!='\n'); // Clean buffer

    if (choice == 1) {
        if (!acct->isActive) {
            puts("Account is already inactive.");
            return;
        }
        acct->isActive = 0;
        saveAccounts();
        puts("Account deactivated successfully.");
    } 
    else if (choice == 2) {
        for (int i = idx; i < totalAccounts - 1; i++) {
            accounts[i] = accounts[i + 1];
        }
        totalAccounts--;
        saveAccounts();
        puts("Account deleted PERMANENTLY.");
    } else {
        puts("Invalid choice.");
    }
}

// FEATURE: Activate Account
void activateAccount(void) {
    int accNum;
    printf("\n=== ACTIVATE ACCOUNT ===\n");
    printf("Enter account number to activate: ");
    if (scanf("%d", &accNum) != 1) {
        puts("Invalid input!");
        while(getchar()!='\n');
        return;
    }
    int idx = findAccountByNumber(accNum);
    if (idx == -1) {
        puts("Account not found!");
        return;
    }
    Account *acct = &accounts[idx];
    if (acct->isActive) {
        puts("This account is already active.");
        return;
    }
    acct->isActive = 1;
    saveAccounts();
    puts("Account activated successfully.");
}
