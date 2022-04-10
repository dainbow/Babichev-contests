#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <immintrin.h>

#pragma GCC target "avx2"

const uint64_t HASHSIZE = 700001;
#define STRING_MAX_SIZE 4097

uint64_t H2(char* s, uint64_t hashsize) {
    uint64_t sum = 0;
    const uint64_t FACTOR = 5;

    while (*s) {
        sum <<= FACTOR;
        sum += *s++;
    }

    return sum % hashsize;
}

uint64_t H1(char* s, uint64_t hashsize) {
    uint64_t hash = 0;

    while(*s) {
        hash = _mm_crc32_u64(hash, *((uint64_t*)s));
        s += 8;
    }

    return hash % hashsize;
}

uint64_t FindKey(FILE* table, char* key, char freeFlag, char nextFlag) {
    uint64_t hash = H1(key, HASHSIZE);
    fseek(table, hash * sizeof(uint64_t), SEEK_SET);

    uint64_t hashIdx = 0;
    fread(&hashIdx, sizeof(uint64_t), 1, table);

    uint64_t preIdx    = hash * sizeof(uint64_t);
    uint64_t prePreIdx = 0;

    while(hashIdx != 0) {
        prePreIdx = preIdx;
        preIdx    = hashIdx;

        fseek(table, hashIdx, SEEK_SET);
        fread(&hashIdx, sizeof(uint64_t), 1, table);

        uint16_t keyLength = 0;
        fread(&keyLength, sizeof(uint16_t), 1, table);
        fseek(table, sizeof(uint16_t), SEEK_CUR);

        char keyStored[STRING_MAX_SIZE] = {};
        fread(keyStored, sizeof(char), keyLength, table);

        if (strncmp(key, keyStored, STRING_MAX_SIZE) == 0) {
            if (freeFlag)
                return 1;

            if (nextFlag)
                return prePreIdx;

            return preIdx;
        }
    }

    if (freeFlag)
        return preIdx;

    return 0;
}

void WriteToTable(FILE* table, char* key, char* value, uint64_t* dataSize) {
    uint64_t hashIdx = FindKey(table, key, 1, 0);
    if (hashIdx == 1) {
        printf("ERROR\n");
        
        return;
    }

    fseek(table, hashIdx, SEEK_SET);
    fwrite(dataSize, sizeof(uint64_t), 1, table);

    uint16_t keyLength   = 0;
    uint16_t valueLength = 0;
    keyLength   = strlen(key);
    valueLength = strlen(value);

    fseek(table, *dataSize + sizeof(uint64_t), SEEK_SET);

    fwrite(&keyLength, sizeof(uint16_t), 1, table);
    fwrite(&valueLength, sizeof(uint16_t), 1, table);
    fwrite(key, sizeof(char), keyLength, table);
    fwrite(value, sizeof(char), valueLength, table);

    *dataSize += sizeof(uint64_t) + 2 * sizeof(uint16_t) + keyLength + valueLength;
}

void PrintHT(FILE* table, char* key) {
    uint64_t keyIdx = FindKey(table, key, 0, 0);
    if (keyIdx == 0) {
        printf("ERROR\n");

        return;
    }

    fseek(table, keyIdx + sizeof(uint64_t), SEEK_SET);

    uint16_t keyLength   = 0;
    uint16_t valueLength = 0;
    fread(&keyLength, sizeof(uint16_t), 1, table);
    fread(&valueLength, sizeof(uint16_t), 1, table);

    char value[STRING_MAX_SIZE] = {};
    fseek(table, keyIdx + sizeof(uint64_t) + 2 * sizeof(uint16_t) + keyLength, SEEK_SET);
    fread(value, sizeof(char), valueLength, table);

    printf("%s %s\n", key, value);
}

int32_t DeleteElem(FILE* table, char* key) {
    uint64_t keyIdx = FindKey(table, key, 0, 1);
    if (keyIdx == 0) {
        printf("ERROR\n");

        return 1;
    }

    uint64_t idxRemember = keyIdx;

    fseek(table, keyIdx, SEEK_SET);
    fread(&keyIdx, sizeof(uint64_t), 1, table);
    
    fseek(table, keyIdx, SEEK_SET);
    fread(&keyIdx, sizeof(uint64_t), 1, table);

    fseek(table, idxRemember, SEEK_SET);
    fwrite(&keyIdx, sizeof(uint64_t), 1, table);

    return 0;
}

void UpdateHT(FILE* table, char* key, char* value, uint64_t* dataSize) {
    if (DeleteElem(table, key))
        return;

    WriteToTable(table, key, value, dataSize);
}

int main() {
    uint32_t cmdAmount = 0;
    scanf("%u", &cmdAmount);
    assert(cmdAmount <= 300000);

    FILE* table = fopen("HashTable.tbl", "w+");

    uint64_t dataSize = HASHSIZE * sizeof(uint64_t);
    for (uint32_t curCmd = 0; curCmd < cmdAmount; curCmd++) {
        int32_t curChar = getchar();
        switch(curChar) {
            case 'A': {
                char key[STRING_MAX_SIZE]   = {};
                char value[STRING_MAX_SIZE] = {};
                scanf("DD %s %s", key, value);

                WriteToTable(table, key, value, &dataSize);
            }
            break;

            case 'U': {
                char key[STRING_MAX_SIZE]   = {};
                char value[STRING_MAX_SIZE] = {};

                scanf("PDATE %s %s", key, value);

                UpdateHT(table, key, value, &dataSize);
            }
            break;

            case 'P': {
                char key[STRING_MAX_SIZE]   = {};
                scanf("RINT %s", key);

                PrintHT(table, key);
            }
            break;

            case 'D': {
                char key[STRING_MAX_SIZE] = {};
                scanf("ELETE %s", key);

                DeleteElem(table, key);
            }
            break;

            default:
                curCmd--;
                break;
        }
    }

    fclose(table);
}