#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ACCOUNTS 100
#define LAST_NAME_LEN 15
#define FIRST_NAME_LEN 10

// Structure definition
struct clientData {
    unsigned int acctNum;
    char lastName[LAST_NAME_LEN];
    char firstName[FIRST_NAME_LEN];
    double balance;
};

// Function prototypes
unsigned int enterChoice(void);
void createTextFile(FILE *readPtr);
void updateRecord(FILE *fPtr);
void addAccount(FILE *fPtr);
void deleteAccount(FILE *fPtr);
void displayAccount(FILE *fPtr);
void clearInputBuffer(void);

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
        struct clientData blankClient = {0, "", "", 0.0};
        for (int i = 0; i < MAX_ACCOUNTS; ++i)
            fwrite(&blankClient, sizeof(struct clientData), 1, cfPtr);
    }

    while ((choice = enterChoice()) != 6) {
        switch (choice) {
            case 1: createTextFile(cfPtr); break;
            case 2: updateRecord(cfPtr); break;
            case 3: addAccount(cfPtr); break;
            case 4: deleteAccount(cfPtr); break;
            case 5: displayAccount(cfPtr); break;
            default: puts("Invalid choice. Try again."); break;
        }
    }

    fclose(cfPtr);
    puts("Program ended.");
    return 0;
}

// Menu
unsigned int enterChoice(void) {
    unsigned int choice;
    puts("\n*** Menu ***");
    puts("1 - Create printable accounts text file");
    puts("2 - Update an account");
    puts("3 - Add a new account");
    puts("4 - Delete an account");
    puts("5 - View account details");
    puts("6 - Exit");
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

// Update account
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
        printf("Enter transaction amount (+charge or -payment): ");
        scanf("%lf", &transaction);
        clearInputBuffer();

        client.balance += transaction;

        fseek(fPtr, (account - 1) * sizeof(struct clientData), SEEK_SET);
        fwrite(&client, sizeof(struct clientData), 1, fPtr);
        printf("New balance: %.2f\n", client.balance);
    }
}

// Add new account
void addAccount(FILE *fPtr) {
    struct clientData client = {0, "", "", 0.0};
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

    fseek(fPtr, (account - 1) * sizeof(struct clientData), SEEK_SET);
    fwrite(&client, sizeof(struct clientData), 1, fPtr);

    puts("Account created successfully.");
}

// Delete account
void deleteAccount(FILE *fPtr) {
    struct clientData client;
    struct clientData blankClient = {0, "", "", 0.0};
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
    }
}
