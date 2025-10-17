#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Structure to store account information
struct Account {
    int accountNo;
    char name[100];
    float balance;
};

// Function declarations
void addAccount();
void depositMoney();
void withdrawMoney();
void checkBalance();
void deleteAccount();
void displayMenu();
void clearScreen();

// File where data will be stored
const char *FILENAME = "bank_data.txt";

int main() {
    int choice;
    do {
        clearScreen();
        printf("\n==============================\n");
        printf("     BANK MANAGEMENT SYSTEM    \n");
        printf("==============================\n");
        printf("1. Add New Account\n");
        printf("2. Deposit Money\n");
        printf("3. Withdraw Money\n");
        printf("4. Check Balance\n");
        printf("5. Delete Account\n");
        printf("6. Exit\n");
        printf("------------------------------\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: addAccount(); break;
            case 2: depositMoney(); break;
            case 3: withdrawMoney(); break;
            case 4: checkBalance(); break;
            case 5: deleteAccount(); break;
            case 6:
                clearScreen();
                printf("\n\n====================================\n");
                printf("   Developed by: MD NAZMUL HASAN   \n");
                printf("====================================\n\n");
                exit(0);
            default:
                printf("Invalid choice! Please try again.\n");
        }

        printf("\nPress Enter to continue...");
        getchar(); getchar();
    } while (choice != 6);

    return 0;
}

// Function to clear console screen
void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

// Add a new account
void addAccount() {
    struct Account acc;
    FILE *fp = fopen(FILENAME, "a+");

    if (fp == NULL) {
        printf("Error opening file!\n");
        return;
    }

    clearScreen();
    printf("\n--- Create New Account ---\n");
    printf("Enter Account Number: ");
    scanf("%d", &acc.accountNo);
    printf("Enter Account Holder Name: ");
    scanf(" %[^\n]", acc.name);
    printf("Enter Initial Balance: ");
    scanf("%f", &acc.balance);

    fwrite(&acc, sizeof(acc), 1, fp);
    fclose(fp);

    printf("\nAccount created successfully!\n");
}

// Deposit money
void depositMoney() {
    struct Account acc;
    FILE *fp = fopen(FILENAME, "r+b");
    int accountNo, found = 0;
    float amount;

    if (fp == NULL) {
        printf("Error opening file!\n");
        return;
    }

    clearScreen();
    printf("\n--- Deposit Money ---\n");
    printf("Enter Account Number: ");
    scanf("%d", &accountNo);

    while (fread(&acc, sizeof(acc), 1, fp)) {
        if (acc.accountNo == accountNo) {
            printf("Enter amount to deposit: ");
            scanf("%f", &amount);
            acc.balance += amount;
            fseek(fp, -sizeof(acc), SEEK_CUR);
            fwrite(&acc, sizeof(acc), 1, fp);
            found = 1;
            printf("\nDeposit successful! New balance: %.2f\n", acc.balance);
            break;
        }
    }

    if (!found)
        printf("Account not found!\n");

    fclose(fp);
}

// Withdraw money
void withdrawMoney() {
    struct Account acc;
    FILE *fp = fopen(FILENAME, "r+b");
    int accountNo, found = 0;
    float amount;

    if (fp == NULL) {
        printf("Error opening file!\n");
        return;
    }

    clearScreen();
    printf("\n--- Withdraw Money ---\n");
    printf("Enter Account Number: ");
    scanf("%d", &accountNo);

    while (fread(&acc, sizeof(acc), 1, fp)) {
        if (acc.accountNo == accountNo) {
            printf("Enter amount to withdraw: ");
            scanf("%f", &amount);
            if (amount > acc.balance) {
                printf("Insufficient balance!\n");
            } else {
                acc.balance -= amount;
                fseek(fp, -sizeof(acc), SEEK_CUR);
                fwrite(&acc, sizeof(acc), 1, fp);
                printf("\nWithdrawal successful! New balance: %.2f\n", acc.balance);
            }
            found = 1;
            break;
        }
    }

    if (!found)
        printf("Account not found!\n");

    fclose(fp);
}

// Check balance
void checkBalance() {
    struct Account acc;
    FILE *fp = fopen(FILENAME, "rb");
    int accountNo, found = 0;

    if (fp == NULL) {
        printf("Error opening file!\n");
        return;
    }

    clearScreen();
    printf("\n--- Check Account Balance ---\n");
    printf("Enter Account Number: ");
    scanf("%d", &accountNo);

    while (fread(&acc, sizeof(acc), 1, fp)) {
        if (acc.accountNo == accountNo) {
            printf("\nAccount Holder: %s\n", acc.name);
            printf("Current Balance: %.2f\n", acc.balance);
            found = 1;
            break;
        }
    }

    if (!found)
        printf("Account not found!\n");

    fclose(fp);
}

// Delete account
void deleteAccount() {
    struct Account acc;
    FILE *fp = fopen(FILENAME, "rb");
    FILE *temp = fopen("temp.txt", "wb");
    int accountNo, found = 0;

    if (fp == NULL || temp == NULL) {
        printf("Error opening file!\n");
        return;
    }

    clearScreen();
    printf("\n--- Delete Account ---\n");
    printf("Enter Account Number: ");
    scanf("%d", &accountNo);

    while (fread(&acc, sizeof(acc), 1, fp)) {
        if (acc.accountNo == accountNo)
            found = 1;
        else
            fwrite(&acc, sizeof(acc), 1, temp);
    }

    fclose(fp);
    fclose(temp);

    remove(FILENAME);
    rename("temp.txt", FILENAME);

    if (found)
        printf("\nAccount deleted successfully!\n");
    else
        printf("Account not found!\n");
}
