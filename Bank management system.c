#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>  // for usleep()

// Structure to store account information
struct Account {
    int accountNo;
    char name[100];
    float balance;
};

const char *FILENAME = "bank_data.dat";

// Function declarations
void addAccount();
void depositMoney();
void withdrawMoney();
void checkBalance();
void deleteAccount();
void clearScreen();
void showIntro();

// Clear screen
void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

// Startup Logo with RGB gradient
void showIntro() {
    clearScreen();

    const char *lines[] = {
        "/ $$$$$$$   /$$$$$$  /$$   /$$ /$$   /$$",
        "| $$__  $$ /$$__  $$| $$$ | $$| $$  /$$/",
        "| $$  \\ $$| $$  \\ $$| $$$$| $$| $$ /$$/ ",
        "| $$$$$$$ | $$$$$$$$| $$ $$ $$| $$$$$/  ",
        "| $$__  $$| $$__  $$| $$  $$$$| $$  $$  ",
        "| $$  \\ $$| $$  | $$| $$\\  $$$| $$\\  $$ ",
        "| $$$$$$$/| $$  | $$| $$ \\  $$| $$ \\  $$",
        "|_______/ |__/  |__/|__/  \\__/|__/  \\__/",
        "",
        "",
    };

    int colors[][3] = {
        {255, 0, 0},     // Red
        {255, 0, 0},
        {255, 170, 0},
        {200, 255, 0},
        {85, 255, 0},    // Green
        {0, 255, 170},
        {0, 0, 255},
        {0, 0, 255}      // Blue
    };

    for (int i = 0; i < 8; i++) {
        printf("\033[38;2;%d;%d;%dm%s\033[0m\n",
               colors[i][0], colors[i][1], colors[i][2], lines[i]);
        usleep(120000); // Animation delay
    }

    printf("\n\033[38;2;150;150;150m        BANK  MANAGEMENT  SYSTEM\033[0m\n\n");
    usleep(300000);
}

// ------------ ADD NEW ACCOUNT ------------
void addAccount() {
    struct Account acc;
    FILE *fp = fopen(FILENAME, "ab");

    if (!fp) {
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

// ------------ DEPOSIT MONEY ------------
void depositMoney() {
    struct Account acc;
    FILE *fp = fopen(FILENAME, "r+b");
    int accountNo, found = 0;
    float amount;

    if (!fp) {
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

            printf("\nDeposit successful! New balance: %.2f\n", acc.balance);
            found = 1;
            break;
        }
    }

    if (!found)
        printf("Account not found!\n");

    fclose(fp);
}

// ------------ WITHDRAW MONEY ------------
void withdrawMoney() {
    struct Account acc;
    FILE *fp = fopen(FILENAME, "r+b");
    int accountNo, found = 0;
    float amount;

    if (!fp) {
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

// ------------ CHECK BALANCE ------------
void checkBalance() {
    struct Account acc;
    FILE *fp = fopen(FILENAME, "rb");
    int accountNo, found = 0;

    if (!fp) {
        printf("Error opening file!\n");
        return;
    }

    clearScreen();
    printf("\n--- Check Balance ---\n");
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

// ------------ DELETE ACCOUNT ------------
void deleteAccount() {
    struct Account acc;
    FILE *fp = fopen(FILENAME, "rb");
    FILE *temp = fopen("temp.dat", "wb");
    int accountNo, found = 0;

    if (!fp || !temp) {
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
    rename("temp.dat", FILENAME);

    if (found)
        printf("\nAccount deleted successfully!\n");
    else
        printf("Account not found!\n");
}

// ------------ MAIN MENU ------------
int main() {
    showIntro();

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
                return 0;

            default:
                printf("Invalid choice! Try again.\n");
        }

        printf("\nPress Enter to continue...");
        getchar(); getchar();

    } while (choice != 6);

    return 0;
}
