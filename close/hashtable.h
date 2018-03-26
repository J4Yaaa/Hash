#pragma once
#include <stddef.h>

#define HashTableMaxSize 1000

typedef int KeyType;
typedef int ValueType;

typedef size_t(*HashFunc)(KeyType key);

typedef enum Stat
{
    VALID,
    INVALID,
    EMPTY
}Stat;

typedef struct HashElem
{
    KeyType key;
    ValueType value;
    Stat stat;
}HashElem;

typedef struct HashTable
{
    HashElem data[HashTableMaxSize];
    size_t size;
    HashFunc hash_func;
}HashTable;

void HashInit(HashTable* ht,HashFunc hash_func);

void HashInsert(HashTable* ht,KeyType key,ValueType value);

// 输入key, 查找对应key的value. 
 int HashFind(HashTable* ht, KeyType key, ValueType* value);

 void HashRemove(HashTable* ht, KeyType key);

 int HashEmpty(HashTable* ht);

 size_t HashSize(HashTable* ht);

 void HashDestroy(HashTable* ht);

