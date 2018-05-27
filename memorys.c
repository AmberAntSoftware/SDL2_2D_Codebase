#include <stdio.h>
#include <stdlib.h>

#include <SDL2/SDL.h>

#include "memorys.h"

///Universal data holding
static MEM_Block *allData = NULL;

void MEM_init(){
    if(allData == NULL){
        allData = SDL_calloc(sizeof(MEM_Chain), sizeof(char));
        allData->cur = NULL;
        allData->top = NULL;
    }
}

void MEM_addData(void *data, void (*freeCallback)(void *)){
    MEM_addDataTo(allData, data, freeCallback);
}

void MEM_freeMemory(){
    MEM_freeMemoryIn(allData);
}

void *MEM_malloc(size_t size, void (*freeCallback)(void *)){
    void *data = SDL_malloc(size);
    MEM_addDataTo(allData, data, freeCallback);
    return data;
}

void *MEM_calloc(size_t nmemb, size_t size, void (*freeCallback)(void *)){
    void *data = SDL_calloc(nmemb, size);
    MEM_addDataTo(allData, data, freeCallback);
    return data;
}

void MEM_exit(){
    MEM_freeBlock(allData);
    allData = NULL;
}

///Instances


void MEM_addDataTo(MEM_Block *dataBlock, void *data, void (*freeCallback)(void *)){
    MEM_Chain *link = MEM_newChain(data, freeCallback);
    if(dataBlock->top == NULL){
        dataBlock->top = link;
        dataBlock->cur = link;
    } else {
        dataBlock->cur->next = link;
        dataBlock->cur = link;
    }
}

void MEM_freeBlock(void *dataBlock){
    MEM_Block *dataBloc = (MEM_Block *)dataBlock;
    MEM_freeMemoryIn(dataBloc);
    SDL_free(dataBloc);
}

void MEM_freeMemoryIn(MEM_Block *dataBlock){
    if(dataBlock == NULL){
        return;
    }
    MEM_Chain *dump = dataBlock->top;
    MEM_Chain *link;
    while(dump != NULL){
        if(dump->data != NULL && (dump->data != dataBlock)){
            if(dump->freeCallback != NULL){
                dump->freeCallback(dump->data);
            } else {
                SDL_free(dump->data);
            }
        }
        link = dump;
        dump = dump->next;
        SDL_free(link);
    }
    dataBlock->top = NULL;
}

void *MEM_mallocTo(MEM_Block *dataBlock, size_t size, void (*freeCallback)(void *)){
    void *data = SDL_malloc(size);
    MEM_addDataTo(dataBlock, data, freeCallback);
    return data;
}

void *MEM_callocTo(MEM_Block *dataBlock, size_t nmemb, size_t size, void (*freeCallback)(void *)){
    void *data = SDL_calloc(nmemb, size);
    MEM_addDataTo(dataBlock, data, freeCallback);
    return data;
}






MEM_Block *MEM_newBlock(){
    MEM_Block *dataBlock = (MEM_Block *)SDL_calloc(sizeof(MEM_Chain), 1);
    MEM_addData(dataBlock, MEM_freeBlock);
    return dataBlock;
}

MEM_Chain *MEM_newChain(void *data, void (*freeCallback)(void *)){
    MEM_Chain *chain = (MEM_Chain *)SDL_calloc(sizeof(MEM_Chain), 1);
    chain->data = data;
    chain->freeCallback = freeCallback;
    return chain;
}
