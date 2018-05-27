#define MEM(void) MEM_init()

#ifndef MEMORYS_H_INCLUDED
#define MEMORYS_H_INCLUDED

typedef struct MEM_Chain {
    void *data;
    void (*freeCallback)(void *);
    struct MEM_Chain *next;
} MEM_Chain;

typedef struct MEM_Block {
    MEM_Chain *top;
    MEM_Chain *cur;
} MEM_Block;

///Global mirrors instanced
void MEM_init();
void MEM_addData(void *data, void (*freeCallback)(void *));
void MEM_freeMemory();
void *MEM_malloc(size_t size, void (*freeCallback)(void *));
void *MEM_calloc(size_t nmemb, size_t size, void (*freeCallback)(void *));
void MEM_exit();///you will have to call MEM_init() for further global use


///Instance

///Make Your Managed Memory
MEM_Block *MEM_newBlock();
///Add Stuff To It
void MEM_addDataTo(MEM_Block *dataBlock, void *data, void (*freeCallback)(void *));
///Delete And Free The Managed Memory
void MEM_freeBlock(void *dataBlock);
///Keep the block, but free any data inside
void MEM_freeMemoryIn(MEM_Block *dataBlock);

///Uses These For 1 Liners :y
void *MEM_mallocTo(MEM_Block *dataBlock, size_t size, void (*freeCallback)(void *));
void *MEM_callocTo(MEM_Block *dataBlock, size_t nmemb, size_t size, void (*freeCallback)(void *));

///You're 'probably' not going to use these
///If you do use these, they are useful for arbitrary linked data and functions
MEM_Chain *MEM_newChain(void *data, void (*freeCallback)(void *));

#endif // MEMORYS_H_INCLUDED
