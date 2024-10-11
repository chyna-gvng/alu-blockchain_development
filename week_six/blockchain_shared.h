#ifndef BLOCKCHAIN_SHARED_H
#define BLOCKCHAIN_SHARED_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <openssl/sha.h>

#define MAX_TRANSACTIONS 100
#define MAX_BLOCKS 1000
#define ADDRESS_LENGTH 41
#define BLOCKCHAIN_FILE "blockchain.dat"

typedef struct {
    char sender[ADDRESS_LENGTH];
    char recipient[ADDRESS_LENGTH];
    double amount;
} Transaction;

typedef struct {
    int index;
    time_t timestamp;
    Transaction transactions[MAX_TRANSACTIONS];
    int transaction_count;
    unsigned char previous_hash[SHA256_DIGEST_LENGTH];
    int proof;
} Block;

typedef struct {
    Block blocks[MAX_BLOCKS];
    int block_count;
} Blockchain;

void sha256(const char* input, unsigned char* output) {
    SHA256_CTX sha256;
    SHA256_Init(&sha256);
    SHA256_Update(&sha256, input, strlen(input));
    SHA256_Final(output, &sha256);
}

void generate_address(char* address) {
    unsigned char hash[SHA256_DIGEST_LENGTH];
    char time_str[20];
    sprintf(time_str, "%ld", time(NULL));
    sha256(time_str, hash);
    
    for (int i = 0; i < 20; i++) {
        sprintf(&address[i*2], "%02x", hash[i]);
    }
    address[40] = '\0';
}

void init_blockchain(Blockchain* blockchain) {
    blockchain->block_count = 0;
    printf("Initialized blockchain with %d blocks\n", blockchain->block_count);
}

void add_block(Blockchain* blockchain, Block* block) {
    if (blockchain->block_count < MAX_BLOCKS) {
        memcpy(&blockchain->blocks[blockchain->block_count], block, sizeof(Block));
        blockchain->block_count++;
        printf("Added block %d to blockchain\n", blockchain->block_count - 1);
    } else {
        printf("Error: Maximum number of blocks reached\n");
    }
}

void save_blockchain(Blockchain* blockchain) {
    FILE* file = fopen(BLOCKCHAIN_FILE, "wb");
    if (file) {
        size_t written = fwrite(blockchain, sizeof(Blockchain), 1, file);
        fclose(file);
        printf("Saved blockchain with %d blocks (written: %zu)\n", blockchain->block_count, written);
    } else {
        printf("Error: Unable to open blockchain file for writing\n");
    }
}

void load_blockchain(Blockchain* blockchain) {
    FILE* file = fopen(BLOCKCHAIN_FILE, "rb");
    if (file) {
        size_t read = fread(blockchain, sizeof(Blockchain), 1, file);
        fclose(file);
        printf("Loaded blockchain with %d blocks (read: %zu)\n", blockchain->block_count, read);
    } else {
        printf("Blockchain file not found. Initializing new blockchain.\n");
        init_blockchain(blockchain);
    }
}

double get_balance(Blockchain* blockchain, const char* address) {
    double balance = 0.0;
    for (int i = 0; i < blockchain->block_count; i++) {
        Block* block = &blockchain->blocks[i];
        for (int j = 0; j < block->transaction_count; j++) {
            Transaction* transaction = &block->transactions[j];
            if (strcmp(transaction->recipient, address) == 0) {
                balance += transaction->amount;
            }
            if (strcmp(transaction->sender, address) == 0) {
                balance -= transaction->amount;
            }
        }
    }
    return balance;
}

int add_transaction(Blockchain* blockchain, const char* sender, const char* recipient, double amount) {
    if (blockchain->block_count == 0) {
        Block new_block = {0};
        new_block.index = 0;
        new_block.timestamp = time(NULL);
        new_block.transaction_count = 0;
        new_block.proof = 100;
        memset(new_block.previous_hash, 0, SHA256_DIGEST_LENGTH);
        add_block(blockchain, &new_block);
    }

    Block* last_block = &blockchain->blocks[blockchain->block_count - 1];
    if (last_block->transaction_count < MAX_TRANSACTIONS) {
        Transaction* new_transaction = &last_block->transactions[last_block->transaction_count];
        strncpy(new_transaction->sender, sender, ADDRESS_LENGTH - 1);
        new_transaction->sender[ADDRESS_LENGTH - 1] = '\0';
        strncpy(new_transaction->recipient, recipient, ADDRESS_LENGTH - 1);
        new_transaction->recipient[ADDRESS_LENGTH - 1] = '\0';
        new_transaction->amount = amount;
        last_block->transaction_count++;
        printf("Added transaction: %s -> %s: %.2f\n", sender, recipient, amount);
        return 1;
    }
    printf("Error: Maximum number of transactions reached for the current block\n");
    return 0;
}

#endif // BLOCKCHAIN_SHARED_H