#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#include <SDL2/SDL.h>

#include <SDL2/SDL.h>

#include "resources.h"
#include "memorys.h"

char* STR_concat(char* str0, char* str1){
    int len0 = SDL_strlen(str0);
    int len1 = SDL_strlen(str1);
    int len = len0+len1;
    if(len > 999999999){
        return NULL;
    }
    len+=1;
    char* buffer = SDL_malloc(len);

    SDL_strlcpy(buffer,str0,len0);
    SDL_strlcat(buffer,str1,len1);

    return buffer;

}

char* STR_concatf0(char* str0_toFree, char* str1){
    char *buffer = NULL;
    buffer = STR_concat(str0_toFree,str1);

    SDL_free(str0_toFree);

    return buffer;

}

char* STR_concatf2(char* str0, char* str1_toFree){
    char *buffer = NULL;
    buffer = STR_concat(str0,str1_toFree);

    SDL_free(str0_toFree);
    SDL_free(str1_toFree);

    return buffer;
}

char* STR_concatf3(char* str0_toFree, char* str1_toFree){
    char *buffer = NULL;
    buffer = STR_concat(str0_toFree,str1_toFree);

    SDL_free(str0_toFree);
    SDL_free(str1_toFree);

    return buffer;
}

//https://www.geeksforgeeks.org/variable-length-argument-c/
char* STR_concatAll(int char_pointer_count, ...){
    char* buffer;
    buffer = SDL_malloc(sizeof(char));
    if(buffer==NULL){
        return NULL;
    }
    buffer[0] = NULL;


    va_list params;
    va_start(params, char_pointer_count);
    ///

    int i;
    for(i = 0; i < char_pointer_count;i++){
        buffer = STR_concatf3(buffer,va_arg ( arguments, char* ));
    }
    ///
    va_end(params);

    return buffer;
}



