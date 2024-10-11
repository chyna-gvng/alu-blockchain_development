# Blockchain Wallet Application

This is a simple blockchain wallet application that simulates two wallets interacting with a shared blockchain. The application is written in C and uses OpenSSL for cryptographic operations.

## Components

1. `blockchain_shared.h`: Header file containing shared structures and functions for the blockchain and transactions.
2. `wallet-one.c`: Source code for the first wallet.
3. `wallet-two.c`: Source code for the second wallet.

## Prerequisites

- GCC compiler
- OpenSSL library

## Compilation

To compile the wallet applications, use the following commands:

```bash
gcc -o wallet-one wallet-one.c -lssl -lcrypto
gcc -o wallet-two wallet-two.c -lssl -lcrypto
```

## Usage

1. Run both wallet applications in separate terminal windows:

   ```bash
   ./wallet-one
   ./wallet-two
   ```

2. Each wallet will generate a unique address on first run.

3. Use the menu options in each wallet to:
   - Check balance
   - Send a transaction
   - View wallet address
   - Exit the application

4. To send a transaction from one wallet to another:
   - Copy the recipient's address from one wallet
   - Use the "Send Transaction" option in the other wallet
   - Enter the copied address and the amount to send

## Files

- `blockchain.dat`: Shared blockchain file used by both wallets
- `wallet_one.dat`: Stores the address for wallet-one
- `wallet_two.dat`: Stores the address for wallet-two

## Notes

- This is a simplified simulation and lacks many features of a real blockchain system.
- The balance calculation is basic and doesn't account for transaction fees or block rewards.
- There's no verification of transactions or prevention of double-spending.
- In a real system, you'd want to implement proper synchronization and locking mechanisms for accessing the shared blockchain file.

## Troubleshooting

If you encounter any issues:
1. Ensure OpenSSL is properly installed and linked.
2. Check for any error messages in the console output.
3. If a segmentation fault occurs, you can use GDB for debugging:
   ```bash
   gdb ./wallet-one
   (gdb) run
   (gdb) backtrace  # If it crashes
   ```

For any other issues or questions, please contact the developer.