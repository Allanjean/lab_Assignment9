#include <stdio.h>
#include <stdlib.h>

/*
Allan Jean-Jacques
COP3502
Lab Assignment 9
*/

// RecordType
struct RecordType {
    int    id;
    char   name;
    int    order;
};

// Node in the chained list
struct ListNode {
    struct RecordType data;
    struct ListNode* next;
};

// Hash table with an array of pointers to ListNode for chaining
struct HashType {
    struct ListNode** buckets;
    int size;
};

// Hash function
int hash(int id, int tableSize) {
    return id % tableSize;
}

// Initializes the hash table
struct HashType* createHashTable(int size) {
    struct HashType* hashTable = (struct HashType*) malloc(sizeof(struct HashType));
    if (hashTable == NULL) {
        printf("Failed to allocate memory for hash table\n");
        exit(-1);
    }

    hashTable->size = size;
    hashTable->buckets = (struct ListNode**) malloc(sizeof(struct ListNode*) * size);
    if (hashTable->buckets == NULL) {
        printf("Failed to allocate memory for buckets\n");
        free(hashTable);
        exit(-1);
    }

    for (int i = 0; i < size; i++) {
        hashTable->buckets[i] = NULL;
    }

    return hashTable;
}

// Inserts a record into the hash table
void insertRecord(struct HashType* hashTable, struct RecordType newRecord) {
    int index = hash(newRecord.id, hashTable->size);
    struct ListNode* newNode = (struct ListNode*) malloc(sizeof(struct ListNode));
    if (newNode == NULL) {
        printf("Failed to allocate memory for a new node\n");
        return;
    }

    newNode->data = newRecord;
    newNode->next = hashTable->buckets[index];
    hashTable->buckets[index] = newNode;
}

// Display records in the hash structure
void displayRecordsInHash(struct HashType* hashTable) {
    for (int i = 0; i < hashTable->size; i++) {
        struct ListNode* current = hashTable->buckets[i];
        if (current != NULL) {
            printf("Index %d -> ", i);
            while (current != NULL) {
                printf("%d %c %d -> ", current->data.id, current->data.name, current->data.order);
                current = current->next;
            }
            printf("NULL\n");
        }
    }
}

// Parses input file to an array of RecordType
int parseData(char* inputFileName, struct RecordType** ppData) {
    FILE* inFile = fopen(inputFileName, "r");
    int dataSz = 0;
    int i, id, order;
    char name;
    struct RecordType *pRecord;
    *ppData = NULL;

    if (inFile) {
        fscanf(inFile, "%d\n", &dataSz);
        *ppData = (struct RecordType*) malloc(sizeof(struct RecordType) * dataSz);
        if (*ppData == NULL) {
            printf("Cannot allocate memory\n");
            exit(-1);
        }
        for (i = 0; i < dataSz; ++i) {
            pRecord = *ppData + i;
            fscanf(inFile, "%d %c %d\n", &id, &name, &order);
            pRecord->id = id;
            pRecord->name = name;
            pRecord->order = order;
        }
        fclose(inFile);
    }
    return dataSz;
}

// Print all records
void printRecords(struct RecordType pData[], int dataSz) {
    int i;
    printf("\nRecords:\n");
    for (i = 0; i < dataSz; ++i) {
        printf("\t%d %c %d\n", pData[i].id, pData[i].name, pData[i].order);
    }
    printf("\n\n");
}

int main(void) {
    struct RecordType *pRecords;
    int recordSz = parseData("input.txt", &pRecords);

	// Set hash table size
    struct HashType* hashTable = createHashTable(10);  
    for (int i = 0; i < recordSz; i++) {
        insertRecord(hashTable, pRecords[i]);
    }

    printRecords(pRecords, recordSz);
    displayRecordsInHash(hashTable);

    return 0;
}