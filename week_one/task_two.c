// task_two.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <openssl/sha.h>

#define MAX_DATA_SIZE 256
#define HASH_SIZE 65

typedef struct Block {
    int index;
    long timestamp;
    char data[MAX_DATA_SIZE];
    char previousHash[HASH_SIZE];
    char hash[HASH_SIZE];
    struct Block *next;
} Block;

void sha256(const char *input, char outputBuffer[HASH_SIZE]) {
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256_CTX sha256;
    SHA256_Init(&sha256);
    SHA256_Update(&sha256, input, strlen(input));
    SHA256_Final(hash, &sha256);
    int i;
    for(i = 0; i < SHA256_DIGEST_LENGTH; i++) {
        sprintf(outputBuffer + (i * 2), "%02x", hash[i]);
    }
    outputBuffer[64] = 0;
}

void calculateHash(Block *block) {
    char input[MAX_DATA_SIZE + HASH_SIZE + 20];
    sprintf(input, "%d%ld%s%s", block->index, block->timestamp, block->data, block->previousHash);
    sha256(input, block->hash);
}

int validateChain(Block *head) {
    Block *current = head;
    Block *previous = NULL;

    while (current != NULL) {
        if (previous != NULL) {
            if (strcmp(current->previousHash, previous->hash) != 0) {
                return 0;
            }

            char calculatedHash[HASH_SIZE];
            char input[MAX_DATA_SIZE + HASH_SIZE + 20];
            sprintf(input, "%d%ld%s%s", current->index, current->timestamp, current->data, current->previousHash);
            sha256(input, calculatedHash);

            if (strcmp(calculatedHash, current->hash) != 0) {
                return 0;
            }
        }

        previous = current;
        current = current->next;
    }

    return 1;
}

void addBlock(Block **head, const char *data) {
    Block *newBlock = (Block*)malloc(sizeof(Block));
    Block *last = *head;

    newBlock->index = (last == NULL) ? 0 : last->index + 1;
    newBlock->timestamp = time(NULL);
    strncpy(newBlock->data, data, MAX_DATA_SIZE);
    newBlock->next = NULL;

    if (*head == NULL) {
        strcpy(newBlock->previousHash, "0");
    } else {
        while (last->next != NULL) {
            last = last->next;
        }
        strcpy(newBlock->previousHash, last->hash);
    }

    calculateHash(newBlock);

    if (*head == NULL) {
        *head = newBlock;
    } else {
        last->next = newBlock;
    }
}

void printBlockchain(Block *head) {
    Block *current = head;
    int blockCount = 0;

    while (current != NULL) {
        printf("Block %d\n", blockCount);
        printf("Index: %d\n", current->index);
        printf("Timestamp: %ld\n", current->timestamp);
        printf("Data: %s\n", current->data);
        printf("Previous Hash: %s\n", current->previousHash);
        printf("Hash: %s\n\n", current->hash);

        current = current->next;
        blockCount++;
    }
}

int main() {
    printf("Task 2: Simple Blockchain Simulation\n\n");
    Block *blockchain = NULL;

    addBlock(&blockchain, "Genesis Block");
    addBlock(&blockchain, "Transaction 1");
    addBlock(&blockchain, "Transaction 2");
    addBlock(&blockchain, "Transaction 3");

    printBlockchain(blockchain);

    if (validateChain(blockchain)) {
        printf("Blockchain is valid.\n");
    } else {
        printf("Blockchain is invalid.\n");
    }

    return 0;
}
