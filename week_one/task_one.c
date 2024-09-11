// task_one.c
#include <stdio.h>
#include <string.h>
#include <openssl/sha.h>

#define MAX_INPUT_LENGTH 1000
#define HASH_SIZE 65

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

int main() {
    char input[MAX_INPUT_LENGTH];
    char output[HASH_SIZE];
    
    printf("Task 1: SHA-256 Hashing using OpenSSL\n");
    printf("Enter a string to hash (max %d characters): ", MAX_INPUT_LENGTH - 1);
    fgets(input, sizeof(input), stdin);
    
    // Remove trailing newline if present
    size_t len = strlen(input);
    if (len > 0 && input[len-1] == '\n') {
        input[len-1] = '\0';
    }
    
    sha256(input, output);
    
    printf("Input: %s\n", input);
    printf("SHA-256 Hash: %s\n", output);

    return 0;
}
