#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// clientData structure definition
struct clientData {
    unsigned int acctNum;
    char lastName[15];
    char firstName[10];
    double balance;
};

// prototypes
unsigned int enterChoice(void);
void textFile(FILE *readPtr);
void updateRecord(FILE *fPtr);
void newRecord(FILE *fPtr);
void deleteRecord(FILE *fPtr);
void initializeFile(void);

int main(int argc, char *argv[]) {
    FILE *cfPtr;
    unsigned int choice;

    // Try opening the file
    if ((cfPtr = fopen("credit.dat", "rb+")) == NULL) {
        printf("File 'credit.dat' not found. Initializing with blank records...\n");
        initializeFile();
        cfPtr = fopen("credit.dat", "rb+");
        if (cfPtr == NULL) {
            printf("%s: File could not be opened.\n", argv[0]);
            exit(EXIT_FAILURE);
        }
    }

    // Menu loop
    while ((choice = enterChoice()) != 5) {
        switch (choice) {
            case 1:
                textFile(cfPtr);
                break;
            case 2:
                updateRecord(cfPtr);
                break;
            case 3:
                newRecord(cfPtr);
                break;
            case 4:
                deleteRecord(cfPtr);
                break;
            default:
                puts("Incorrect choice");
                break;
        }
    }

    fclose(cfPtr);
    return 0;
}

// Create text file for printing
void textFile(FILE *readPtr) {
    FILE *writePtr;
    int result;
    struct clientData client = {0, "", "", 0.0};

    if ((writePtr = fopen("accounts.txt", "w")) == NULL) {
        puts("File could not be opened.");
    } else {
        rewind(readPtr);
        fprintf(writePtr, "%-6s%-16s%-11s%10s\n", "Acct", "Last Name", "First Name", "Balance");

        while ((result = fread(&client, sizeof(struct clientData), 1, readPtr)) == 1) {
            if (client.acctNum != 0) {
                fprintf(writePtr, "%-6d%-16s%-11s%10.2f\n",
                        client.acctNum, client.lastName, client.firstName, client.balance);
            }
        }

        fclose(writePtr);
    }
}

// Update an account
void updateRecord(FILE *fPtr) {
    unsigned int account;
    double transaction;
    struct clientData client = {0, "", "", 0.0};

    printf("Enter account to update (1 - 100): ");
    if (scanf("%u", &account) != 1 || account < 1 || account > 100) {
        printf("Invalid input.\n");
        return;
    }

    fseek(fPtr, (account - 1) * sizeof(struct clientData), SEEK_SET);
    fread(&client, sizeof(struct clientData), 1, fPtr);

    if (client.acctNum == 0) {
        printf("Account #%d has no information.\n", account);
    } else {
        printf("%-6d%-16s%-11s%10.2f\n\n", client.acctNum, client.lastName, client.firstName, client.balance);
        printf("Enter charge (+) or payment (-): ");
        if (scanf("%lf", &transaction) != 1) {
            printf("Invalid amount.\n");
            return;
        }

        client.balance += transaction;
        printf("%-6d%-16s%-11s%10.2f\n", client.acctNum, client.lastName, client.firstName, client.balance);

        fseek(fPtr, -(long)sizeof(struct clientData), SEEK_CUR);

        fwrite(&client, sizeof(struct clientData), 1, fPtr);
    }
}

// Delete a record
void deleteRecord(FILE *fPtr) {
    struct clientData client;
    struct clientData blankClient = {0, "", "", 0.0};
    unsigned int accountNum;

    printf("Enter account number to delete (1 - 100): ");
    if (scanf("%u", &accountNum) != 1 || accountNum < 1 || accountNum > 100) {
        printf("Invalid input.\n");
        return;
    }

    fseek(fPtr, (accountNum - 1) * sizeof(struct clientData), SEEK_SET);
    fread(&client, sizeof(struct clientData), 1, fPtr);

    if (client.acctNum == 0) {
        printf("Account %d does not exist.\n", accountNum);
    } else {
        fseek(fPtr, (accountNum - 1) * sizeof(struct clientData), SEEK_SET);
        fwrite(&blankClient, sizeof(struct clientData), 1, fPtr);
        printf("Account %d deleted successfully.\n", accountNum);
    }
}

// Create a new record
void newRecord(FILE *fPtr) {
    struct clientData client = {0, "", "", 0.0};
    unsigned int accountNum;

    printf("Enter new account number (1 - 100): ");
    if (scanf("%u", &accountNum) != 1 || accountNum < 1 || accountNum > 100) {
        printf("Invalid input.\n");
        return;
    }

    fseek(fPtr, (accountNum - 1) * sizeof(struct clientData), SEEK_SET);
    fread(&client, sizeof(struct clientData), 1, fPtr);

    if (client.acctNum != 0) {
        printf("Account #%d already contains information.\n", client.acctNum);
    } else {
        printf("Enter lastname, firstname, balance: ");
        if (scanf("%14s%9s%lf", client.lastName, client.firstName, &client.balance) != 3) {
            printf("Invalid input.\n");
            return;
        }

        client.acctNum = accountNum;
        fseek(fPtr, (client.acctNum - 1) * sizeof(struct clientData), SEEK_SET);
        fwrite(&client, sizeof(struct clientData), 1, fPtr);
        printf("Account #%d created successfully.\n", client.acctNum);
    }
}

// Menu
unsigned int enterChoice(void) {
    unsigned int menuChoice;
    printf("\nEnter your choice\n"
           "1 - store a formatted text file of accounts called\n"
           "    \"accounts.txt\" for printing\n"
           "2 - update an account\n"
           "3 - add a new account\n"
           "4 - delete an account\n"
           "5 - end program\n? ");
    scanf("%u", &menuChoice);
    return menuChoice;
}

// Helper: Initialize credit.dat with 100 blank records
void initializeFile(void) {
    FILE *fPtr = fopen("credit.dat", "wb");
    struct clientData blankClient = {0, "", "", 0.0};
    for (int i = 0; i < 100; ++i) {
        fwrite(&blankClient, sizeof(struct clientData), 1, fPtr);
    }
    fclose(fPtr);
}
