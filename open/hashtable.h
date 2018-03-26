#pragma once
#include <stddef.h>

#define HashMaxSize 100

typedef int KeyType;
typedef int ValueType;
typedef size_t(*HashFunc)(KeyType key);

typedef struct HashElem
{
   KeyType key;
   ValueType value;
   struct HashElem* next;
}HashElem;

typedef struct HashTable
{
    HashElem* data[HashMaxSize];
    size_t size;
    HashFunc hash_func;
}HashTable;

void HashInit(HashTable* ht, HashFunc hash_func);

// 约定哈希表中不能包含 key 相同的值. 
int HashInsert(HashTable* ht, KeyType key, ValueType value);

int HashFind(HashTable* ht, KeyType key, ValueType* value);

void HashRemove(HashTable* ht, KeyType key);

size_t HashSize(HashTable* ht);

int HashEmpty(HashTable* ht);

void HashDestroy(HashTable* ht);
