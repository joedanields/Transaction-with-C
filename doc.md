
---
# Trans.c — Detailed README

## Overview

This C program manages bank accounts stored in a binary file named `credit.dat`. It allows users to:

* Create a printable text report of accounts (`accounts.txt`)
* Update account balances
* Add new accounts
* Delete accounts

The program uses fixed-size records stored sequentially in the binary file for easy random access.

---

## Data Structure

```c
struct clientData {
    unsigned int acctNum;    // Account number (1-100), 0 means no record
    char lastName[15];       // Client's last name (max 14 chars + null)
    char firstName[10];      // Client's first name (max 9 chars + null)
    double balance;          // Account balance (double precision)
};
```

* The size of `struct clientData` is fixed, allowing direct positioning in the file by multiplying the record index by the struct size.
* If `acctNum` is 0, the record is considered empty or deleted.

---

## Main Program Flow

```c
int main(int argc, char *argv[]) {
    FILE *cfPtr;
    unsigned int choice;

    // Try to open credit.dat in read-write binary mode
    if ((cfPtr = fopen("credit.dat", "rb+")) == NULL) {
        // File does not exist, create and initialize blank records
        printf("File 'credit.dat' not found. Initializing with blank records...\n");
        initializeFile();

        // Open file again after initialization
        cfPtr = fopen("credit.dat", "rb+");
        if (cfPtr == NULL) {
            // If file still can't be opened, exit with error
            printf("%s: File could not be opened.\n", argv[0]);
            exit(EXIT_FAILURE);
        }
    }

    // Display menu and process user choices until they choose to exit (choice 5)
    while ((choice = enterChoice()) != 5) {
        switch (choice) {
            case 1:
                textFile(cfPtr);  // Create text report
                break;
            case 2:
                updateRecord(cfPtr);  // Update existing account
                break;
            case 3:
                newRecord(cfPtr);  // Add new account
                break;
            case 4:
                deleteRecord(cfPtr);  // Delete account
                break;
            default:
                puts("Incorrect choice");
                break;
        }
    }

    fclose(cfPtr);
    return 0;
}
```

---

## Functions Explained

### 1. `initializeFile()`

* Purpose: Creates the binary file `credit.dat` and writes 100 blank (`acctNum == 0`) records.
* This prepares the file for random access read/write.
* Uses a loop to write 100 instances of `blankClient` (all zeroed).

```c
void initializeFile(void) {
    FILE *fPtr = fopen("credit.dat", "wb");
    struct clientData blankClient = {0, "", "", 0.0};

    for (int i = 0; i < 100; ++i) {
        fwrite(&blankClient, sizeof(struct clientData), 1, fPtr);
    }

    fclose(fPtr);
}
```

---

### 2. `enterChoice()`

* Displays the user menu.
* Reads user input for menu selection.
* Returns the user's choice as an unsigned int.

```c
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
```

---

### 3. `textFile(FILE *readPtr)`

* Reads all records sequentially from `credit.dat`.
* Writes non-empty records into a formatted text file `accounts.txt`.
* The file contains headers and columns for account number, last name, first name, and balance.
* Uses `rewind()` to go to the start of the file before reading.

```c
void textFile(FILE *readPtr) {
    FILE *writePtr;
    int result;
    struct clientData client = {0, "", "", 0.0};

    if ((writePtr = fopen("accounts.txt", "w")) == NULL) {
        puts("File could not be opened.");
    } else {
        rewind(readPtr); // Move file pointer to beginning

        // Write table header
        fprintf(writePtr, "%-6s%-16s%-11s%10s\n", "Acct", "Last Name", "First Name", "Balance");

        // Read each record
        while ((result = fread(&client, sizeof(struct clientData), 1, readPtr)) == 1) {
            if (client.acctNum != 0) {  // Skip empty records
                fprintf(writePtr, "%-6d%-16s%-11s%10.2f\n",
                        client.acctNum, client.lastName, client.firstName, client.balance);
            }
        }

        fclose(writePtr);
    }
}
```

---

### 4. `updateRecord(FILE *fPtr)`

* Prompts the user for an account number to update.
* Validates the number is between 1 and 100.
* Uses `fseek` to position the file pointer to the record offset.
* Reads the client record.
* If account exists, shows current details.
* Prompts for a transaction amount (positive for charge, negative for payment).
* Updates balance and writes the updated record back to the same position.
* Uses `fseek` with `SEEK_CUR` and a negative offset to move back before writing.

```c
void updateRecord(FILE *fPtr) {
    unsigned int account;
    double transaction;
    struct clientData client = {0, "", "", 0.0};

    printf("Enter account to update (1 - 100): ");
    if (scanf("%u", &account) != 1 || account < 1 || account > 100) {
        printf("Invalid input.\n");
        return;
    }

    // Position file pointer to record (account - 1)
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

        // Move back file pointer to overwrite the current record
        fseek(fPtr, -(long)sizeof(struct clientData), SEEK_CUR);

        fwrite(&client, sizeof(struct clientData), 1, fPtr);
    }
}
```

---

### 5. `newRecord(FILE *fPtr)`

* Asks for a new account number (1-100).
* Checks if the record already exists (if `acctNum != 0`).
* If not, asks for last name, first name, and initial balance.
* Writes the new record to the correct file position.

```c
void newRecord(FILE *fPtr) {
    struct clientData client = {0, "", "", 0.0};
    unsigned int accountNum;

    printf("Enter new account number (1 - 100): ");
    if (scanf("%u", &accountNum) != 1 || accountNum < 1 || accountNum > 100) {
        printf("Invalid input.\n");
        return;
    }

    // Check if account already exists
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

        // Write new record to file
        fseek(fPtr, (client.acctNum - 1) * sizeof(struct clientData), SEEK_SET);
        fwrite(&client, sizeof(struct clientData), 1, fPtr);
        printf("Account #%d created successfully.\n", client.acctNum);
    }
}
```

---

### 6. `deleteRecord(FILE *fPtr)`

* Prompts for account number to delete.
* Validates input.
* Reads the record.
* If the account exists, replaces it with a blank record.
* Confirms deletion to user.

```c
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
        // Overwrite record with blank client data
        fseek(fPtr, (accountNum - 1) * sizeof(struct clientData), SEEK_SET);
        fwrite(&blankClient, sizeof(struct clientData), 1, fPtr);
        printf("Account %d deleted successfully.\n", accountNum);
    }
}
```

---

## Important Notes

* **File Positioning:**
  Since records are fixed size, file position for account number `n` is `(n - 1) * sizeof(struct clientData)`. This allows direct random access, improving performance over sequential scanning.

* **Binary Mode:**
  File is opened in `"rb+"` mode (binary read-write) to ensure consistent behavior across platforms.

* **Input Safety:**
  All user inputs are checked


for validity (number range and successful scans).

* **Account Number Range:**
  The program supports account numbers from 1 to 100 only.

---

## Example Usage

1. Run the program.
2. If `credit.dat` does not exist, it creates and initializes the file.
3. User sees menu and selects:

   * **1** to create `accounts.txt`
   * **2** to update a balance
   * **3** to add new account
   * **4** to delete an account
   * **5** to exit
4. Operations update `credit.dat` immediately.
5. Text report generated by option 1 can be printed or viewed.

---

