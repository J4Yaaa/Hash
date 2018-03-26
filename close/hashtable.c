#include "hashtable.h"
#include <stdio.h>

size_t HashFuncDefault(KeyType key)
{
    return key % HashTableMaxSize;
}

void HashInit(HashTable* ht,HashFunc hash_func)
{
    if(ht == NULL)
    {
        return;
    }
    ht->hash_func = hash_func;
    ht->size = 0;
    size_t i = 0;
    for(; i < HashTableMaxSize;++i)
    {
        ht->data[i].stat = EMPTY;
    }
}

void HashInsert(HashTable* ht,KeyType key,ValueType value)
{
    if(ht == NULL)
    {
        return;
    }
    if(ht->size >= HashTableMaxSize)
    {
        return;
    }
    size_t offset = ht->hash_func(key);
    //以下分三种情况讨论
    //   1.找到key的位置发现状态为valid并且key值相同  ->  这里不允许重复，直接返回即可
    if(ht->data[offset].stat == VALID && ht->data[offset].key == key)
    {
        return;
    }
    //   2.找到key的位置发现状态不为 Valid  ->  直接插入
    else if(ht->data[offset].stat != VALID)
    {
        ht->data[offset].key = key;
        ht->data[offset].value = value;
        ht->data[offset].stat = VALID;
        ++ht->size;
        return;
    }
    //   3.找到key的置为发现状态是valid但是值却不是key，证明前面因为哈希冲突占用了这个位置  ->  向后找到第一个状态不为valid的空位
    else
    {
        while(ht->data[offset].stat == VALID)
        {
            ++offset;
            if(offset >= HashTableMaxSize)
            {
                offset -= HashTableMaxSize;
            }
        }
        ht->data[offset].key = key;
        ht->data[offset].value = value;
        ht->data[offset].stat = VALID;
        ++ht->size;
        return;
    }
    return;
}

// 输入key, 查找对应key的value. 
 int HashFind(HashTable* ht, KeyType key, ValueType* value)
{
    if(ht == NULL || value == NULL)
    {
        return 0;
    }
    size_t offset = ht->hash_func(key);
    //这里分三种情况
    //    1.找到对应的offset，状态为valid并且key值相同  ->  找到了，直接返回
    if(ht->data[offset].stat == VALID && ht->data[offset].key == key)
    {
        *value = ht->data[offset].value;
        return 1;
    }
    //    2.状态为empty  ->  不存在
    else if(ht->data[offset].stat == EMPTY)
    {
        return 0;
    }
    //    3.状态不为empty，并且key值不相同  ->  找到第一个empty之前，如果找到key值相同就是找到，没有就证明不存在
    else
    {
        while(ht->data[offset].stat != EMPTY)
        {
            if(ht->data[offset].key == key && ht->data[offset].stat == VALID)
            {
                *value = ht->data[offset].value;
                return 1;
            }
            ++offset;
            if(offset >= HashTableMaxSize)
            {
                offset -= HashTableMaxSize;
            }
        }
        return 0;
    }
    return 0;
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
    //这里需要先找到，和find一样分为三种情况
    if(ht->data[offset].stat == VALID && ht->data[offset].key == key)
    {
        ht->data[offset].stat = INVALID;
        --ht->size;
        return;
    }
    else if(ht->data[offset].stat == EMPTY)
    {
        return;
    }
    else
    {
        while(ht->data[offset].stat != EMPTY)
        {
            if(ht->data[offset].stat == VALID && ht->data[offset].key == key)
            {
                ht->data[offset].stat = INVALID;
                --ht->size;
                return;
            }
            ++offset;
            if(offset >= HashTableMaxSize)
            {
                offset %= HashTableMaxSize;
            }
        }
        return;
    }
    return;
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

 size_t HashSize(HashTable* ht)
{
    if(ht == NULL)
    {
        return 0;
    }
    return ht->size;
}

 void HashDestroy(HashTable* ht)
{
    if(ht == NULL)
    {
        return;
    }
    size_t i = 0;
    for(; i < HashTableMaxSize; ++i)
    {
        ht->data[i].stat = EMPTY;
    }
    ht->size = 0;
}


////////////////////////////////////////////////////////
//测试代码
////////////////////////////////////////////////////////

#if 1

#define TEST_HEADER printf("\n-------------%s--------------\n",__FUNCTION__)

void HashTablePrint(HashTable* ht,const char* str)
{
    if(ht == NULL || str == NULL)
    {
        return;
    }
    printf("%s\n",str);
    size_t i = 0;
    for(; i < HashTableMaxSize; ++i)
    {
        if(ht->data[i].stat != EMPTY)
        {
            printf("key = %d :value =  %d : stat = %d\n",ht->data[i].key,ht->data[i].value,ht->data[i].stat);
        }
    }
}

void TestInit()
{
    TEST_HEADER;
    HashTable ht;
    HashInit(&ht,HashFuncDefault);
    printf("hash->size except 0,actual %lu\n",ht.size);
    printf("hash->hash_func except %p,actual %p\n",HashFuncDefault,ht.hash_func);
    size_t i = 0;
    for(;i < HashTableMaxSize;++i)
    {
        if(ht.data[i].stat != EMPTY)
        {
            printf("init error...\n");
        }
    }
}

void TestInsert()
{
    TEST_HEADER;
    HashTable ht;
    HashInit(&ht,HashFuncDefault);
    HashInsert(&ht,1,100);
    HashInsert(&ht,2,200);
    HashInsert(&ht,101,300);
    HashInsert(&ht,102,400);
    HashInsert(&ht,101,500);
    HashTablePrint(&ht,"插入5各元素，成功四个");
}

void TestFind()
{
    TEST_HEADER;
    HashTable ht;
    HashInit(&ht,HashFuncDefault);
    HashInsert(&ht,1,100);
    HashInsert(&ht,2,200);
    HashInsert(&ht,101,300);
    HashInsert(&ht,102,400);
    HashInsert(&ht,101,500);
    HashTablePrint(&ht,"插入5各元素，成功四个");
    ValueType value;
    int ret = 0;

    ret = HashFind(&ht,2,&value);
    printf("ret except 1,actual %d; value escept 200,actual %d\n",ret,value);

    ret = HashFind(&ht,102,&value);
    printf("ret except 1,actual %d; value escept 400,actual %d\n",ret,value);

    ret = HashFind(&ht,101,&value);
    printf("ret except 1,actual %d; value escept 300,actual %d\n",ret,value);
}

void TestRemve()
{
    TEST_HEADER;
    HashTable ht;
    HashInit(&ht,HashFuncDefault);
    HashInsert(&ht,1,100);
    HashInsert(&ht,2,200);
    HashInsert(&ht,101,300);
    HashInsert(&ht,102,400);
    HashInsert(&ht,101,500);
    HashTablePrint(&ht,"插入5各元素，成功四个");
    HashRemove(&ht,101);
    HashTablePrint(&ht,"删除 101,300");
    HashRemove(&ht,2);
    HashTablePrint(&ht,"删除 2,200");
    ValueType value;
    int ret = 0;
    ret = HashFind(&ht,102,&value);
    printf("ret except 1,actual %d; value escept 400,actual %d\n",ret,value);
    ret = HashFind(&ht,101,&value);
    printf("ret except 0,actual %d;\n",ret);
}

void Test()
{
    TEST_HEADER;
    HashTable ht;
    HashInit(&ht,HashFuncDefault);
    printf("hashtable empty except 1,actual %d\n",HashEmpty(&ht));
    printf("hashtable size except 0,actual %lu\n",HashSize(&ht));
    HashInsert(&ht,1,100);
    HashInsert(&ht,2,200);
    HashInsert(&ht,101,300);
    HashInsert(&ht,102,400);
    HashInsert(&ht,101,500);
    HashTablePrint(&ht,"插入5各元素，成功四个");
    printf("hashtable empty except 0,actual %d\n",HashEmpty(&ht));
    printf("hashtable size except 4,actual %lu\n",HashSize(&ht));
    HashDestroy(&ht);
    printf("hashtable empty except 1,actual %d\n",HashEmpty(&ht));
    printf("hashtable size except 0,actual %lu\n",HashSize(&ht));
}

int main()
{
    TestInit();
    TestInsert();
    TestFind();
    TestRemve();
    Test();
    return 0;
}

#endif
