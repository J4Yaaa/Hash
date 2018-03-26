#include "hashtable.h"
#include <stdio.h>
#include <stdlib.h>

size_t HashFuncDefault(KeyType key)
{
    return key % HashMaxSize;
}

void HashInit(HashTable* ht, HashFunc hash_func)
{
    if(ht == NULL)
    {
        return;
    }
    ht->size = 0;
    ht->hash_func = hash_func;
    size_t i = 0;
    for(; i < HashMaxSize; ++i)
    {
        ht->data[i] = NULL;
    }
}

HashElem* CreateHashElem(KeyType key,ValueType value)
{
    HashElem* hash_elem = (HashElem*)malloc(sizeof(HashElem));
    hash_elem->key = key;
    hash_elem->value = value;
    hash_elem->next = NULL;
    return hash_elem;
}

// 约定哈希表中不能包含 key 相同的值. 
int HashInsert(HashTable* ht, KeyType key, ValueType value)
{
    if(ht == NULL)
    {
        return 0;
    }
    HashElem* hash_elem = CreateHashElem(key,value);
    size_t offset = ht->hash_func(key);
    if(ht->data[offset] == NULL)
    {
        ht->data[offset] = hash_elem;
    }
    else
    {
        hash_elem->next = ht->data[offset];
        ht->data[offset] = hash_elem;
    }
    ++ht->size;
    return 1;
}

int HashFind(HashTable* ht, KeyType key, ValueType* value)
{
    if(ht == NULL || value == NULL)
    {
        return 0;
    }
    size_t offset = ht->hash_func(key);
    if(ht->data[offset] == NULL)
    {
        return 0;
    }
    HashElem* cur = ht->data[offset];
    while(cur)
    {
        if(cur->key == key)
        {
            *value = cur->value;
            return 1;
        }
        cur = cur->next;
    }
    return 0;
}

void DestoryHashElem(HashElem* ptr)
{
    free(ptr);
}

HashElem* FindPrevElem(HashTable* ht,size_t offset,KeyType key)
{
    if(ht == NULL)
    {
        return NULL;
    }
    if(ht->data[offset]->key == key)
    {
        return NULL;
    }
    HashElem* cur = ht->data[offset];
    while(cur->next != NULL)
    {
        if(cur->next->key == key)
        {
            return cur;
        }
        cur = cur->next;
    }
    return NULL;
}

void HashRemove(HashTable* ht, KeyType key)
{
    if(ht == NULL)
    {
        return;
    }
    if(ht->size == 0)
    {
        return;
    }
    size_t offset = ht->hash_func(key);
    if(ht->data[offset] == NULL)
    {
        return;
    }
    if(ht->data[offset]->key == key)
    {
        HashElem* to_delete = ht->data[offset];
        ht->data[offset] = to_delete->next;
        DestoryHashElem(to_delete);
        --ht->size;
        return;
    }
    else
    {
        HashElem* prev_elem = FindPrevElem(ht,offset,key);
        HashElem* to_delete = prev_elem->next;
        prev_elem->next = to_delete->next;
        DestoryHashElem(to_delete);
        --ht->size;
        return;
    }
    return;
}

size_t HashSize(HashTable* ht)
{
    if(ht == NULL)
    {
        return 0;
    }
    return ht->size;
}

int HashEmpty(HashTable* ht)
{
    if(ht == NULL)
    {
        return 0;
    }
    if(ht->size == 0)
    {
        return 1;
    }
    return 0;
}

void HashPopFront(HashTable* ht,size_t i)
{
    if(ht == NULL)
    {
        return;
    }
    if(i < 0 || i >= HashMaxSize)
    {
        return;
    }
    while(ht->data[i] != NULL)
    {
        HashElem* to_delete = ht->data[i];
        ht->data[i] = to_delete->next;
        DestoryHashElem(to_delete);
        --ht->size;
    }
    return;
}

void HashDestroy(HashTable* ht)
{
    if(ht == NULL)
    {
        return; 
    }
    size_t i = 0;
    for(; i < HashMaxSize; ++i)
    {
        if(ht->data[i] != NULL)
        {
            HashPopFront(ht,i);
        }
    }
    return;
}


/////////////////////////////////////////////////////////
//测试代码
/////////////////////////////////////////////////////////

#if 1

#define TEST_HEADER printf("\n-----------%s----------\n",__FUNCTION__)

void PrintHashTable(HashTable* ht)
{
    if(ht == NULL)
    {
        return;
    }
    size_t i = 0;
    for(; i < HashMaxSize; ++i)
    {
        if(ht->data[i] != NULL)
        {
            HashElem* cur = ht->data[i];
            while(cur)
            {
                printf("[%d : %d] -> ",cur->key,cur->value);
                cur = cur->next;
            }
            printf("NULL\n");
        }
    }
}

void TestInit()
{
    TEST_HEADER;
    HashTable ht;
    HashInit(&ht,HashFuncDefault);
    printf("hash->size except 0,actual %lu,hash->hash_func except %p,actual %p\n",ht.size,HashFuncDefault,ht.hash_func);
    PrintHashTable(&ht);
}

void TestInsert()
{
    TEST_HEADER;
    HashTable ht;
    HashInit(&ht,HashFuncDefault);
    HashInsert(&ht,1,100);
    HashInsert(&ht,101,200);
    HashInsert(&ht,2,300);
    HashInsert(&ht,202,400);
    HashInsert(&ht,3,500);
    HashInsert(&ht,303,600);
    PrintHashTable(&ht);
}

void TestFind()
{
    TEST_HEADER;
    HashTable ht;
    HashInit(&ht,HashFuncDefault);
    HashInsert(&ht,1,100);
    HashInsert(&ht,101,200);
    HashInsert(&ht,2,300);
    HashInsert(&ht,202,400);
    HashInsert(&ht,3,500);
    HashInsert(&ht,303,600);
    ValueType value;
    int ret = 0;

    ret = HashFind(&ht,1,&value);
    printf("ret except 1,actual %d; value except 100,actual %d\n",ret,value);

    ret = HashFind(&ht,101,&value);
    printf("ret except 1,actual %d; value except 200,actual %d\n",ret,value);

    ret = HashFind(&ht,1001,&value);
    printf("ret except 0,actual %d;\n",ret);
}

void TestRemove()
{
    TEST_HEADER;
    HashTable ht;
    HashInit(&ht,HashFuncDefault);
    HashInsert(&ht,1,100);
    HashInsert(&ht,101,200);
    HashInsert(&ht,2,300);
    HashInsert(&ht,202,400);
    HashInsert(&ht,3,500);
    HashInsert(&ht,303,600);

    HashRemove(&ht,1);
    PrintHashTable(&ht);

    HashRemove(&ht,202);
    PrintHashTable(&ht);

    HashRemove(&ht,101);
    PrintHashTable(&ht);
}

void Test()
{
    TEST_HEADER;
    HashTable ht;
    HashInit(&ht,HashFuncDefault);
    printf("ht->size except 0,actual %lu\n",HashSize(&ht));
    printf("ht empty except 1,actual %d\n",HashEmpty(&ht));
    HashInsert(&ht,1,100);
    HashInsert(&ht,101,200);
    HashInsert(&ht,2,300);
    HashInsert(&ht,202,400);
    HashInsert(&ht,3,500);
    HashInsert(&ht,303,600);
    printf("ht->size except 6,actual %lu\n",HashSize(&ht));
    printf("ht empty except 0,actual %d\n",HashEmpty(&ht));
    HashDestroy(&ht);
    printf("ht->size except 0,actual %lu\n",HashSize(&ht));
    printf("ht empty except 1,actual %d\n",HashEmpty(&ht));
    PrintHashTable(&ht);
}

int main()
{
    TestInit();
    TestInsert();
    TestFind();
    TestRemove();
    Test();
    return 0;
}
#endif
