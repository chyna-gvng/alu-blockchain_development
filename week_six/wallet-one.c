#include "blockchain_shared.h"

#define WALLET_FILE "wallet_one.dat"

typedef struct {
    char address[ADDRESS_LENGTH];
} Wallet;

void save_wallet(Wallet* wallet) {
    FILE* file = fopen(WALLET_FILE, "wb");
    if (file) {
        size_t written = fwrite(wallet, sizeof(Wallet), 1, file);
        fclose(file);
        printf("Saved wallet (written: %zu)\n", written);
    } else {
        printf("Error: Unable to open wallet file for writing\n");
    }
}

void load_wallet(Wallet* wallet) {
    FILE* file = fopen(WALLET_FILE, "rb");
    if (file) {
        size_t read = fread(wallet, sizeof(Wallet), 1, file);
        fclose(file);
        printf("Loaded wallet (read: %zu)\n", read);
    } else {
        printf("Wallet file not found. Generating new address.\n");
        generate_address(wallet->address);
        save_wallet(wallet);
    }
}

void print_menu() {
    printf("\n1. Check Balance\n");
    printf("2. Send Transaction\n");
    printf("3. View Address\n");
    printf("4. Exit\n");
    printf("Enter your choice: ");
}

int main() {
    Wallet wallet;
    Blockchain blockchain;
    
    printf("Loading wallet...\n");
    load_wallet(&wallet);
    printf("Loading blockchain...\n");
    load_blockchain(&blockchain);
    
    int choice;
    char recipient[ADDRESS_LENGTH];
    double amount;
    
    while (1) {
        print_menu();
        if (scanf("%d", &choice) != 1) {
            printf("Invalid input. Please enter a number.\n");
            while (getchar() != '\n'); // Clear input buffer
            continue;
        }
        
        switch (choice) {
            case 1:
                printf("Your balance: %.2f\n", get_balance(&blockchain, wallet.address));
                break;
            case 2:
                printf("Enter recipient address: ");
                scanf("%40s", recipient);
                printf("Enter amount to send: ");
                if (scanf("%lf", &amount) != 1) {
                    printf("Invalid amount. Transaction cancelled.\n");
                    while (getchar() != '\n'); // Clear input buffer
                    break;
                }
                if (add_transaction(&blockchain, wallet.address, recipient, amount)) {
                    printf("Transaction added successfully.\n");
                    save_blockchain(&blockchain);
                } else {
                    printf("Transaction failed.\n");
                }
                break;
            case 3:
                printf("Your address: %s\n", wallet.address);
                break;
            case 4:
                printf("Exiting...\n");
                return 0;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    }
    
    return 0;
}