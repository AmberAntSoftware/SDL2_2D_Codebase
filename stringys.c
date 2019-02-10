#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#include <SDL2/SDL.h>

char* STR_concat(const char* str0, const char* str1){

    int len0 = SDL_strlen(str0);
    int len1 = SDL_strlen(str1);
    int len = len0+len1;
    if(len > 999999999 || len < 1){
        return NULL;
    }
    len+=1;
    char* buffer = SDL_malloc(len);

    SDL_strlcpy(buffer,str0,len);
    SDL_strlcat(buffer,str1,len);

    return buffer;

}

char* STR_concatf0(char* str0_toFree, const char* str1){
    char *buffer = NULL;
    buffer = STR_concat(str0_toFree,str1);

    free(str0_toFree);

    return buffer;

}

char* STR_concatf1(const char* str0, char* str1_toFree){
    char *buffer = NULL;
    buffer = STR_concat(str0,str1_toFree);

    free(str1_toFree);

    return buffer;
}

char* STR_concatf2(char* str0_toFree, char* str1_toFree){
    char *buffer = NULL;
    buffer = STR_concat(str0_toFree,str1_toFree);

    free(str0_toFree);
    free(str1_toFree);

    return buffer;
}

//https://www.geeksforgeeks.org/variable-length-argument-c/
char* STR_concatAll(int char_pointer_count, ...){
    char* buffer;
    buffer = SDL_malloc(sizeof(char));
    if(buffer==NULL){
        return NULL;
    }
    buffer[0] = '\0';


    va_list params;
    va_start(params, char_pointer_count);
    ///

    int i;
    for(i = 0; i < char_pointer_count;i++){
        buffer = STR_concatf2(buffer,va_arg ( params, char* ));
    }
    ///
    va_end(params);

    return buffer;
}


char* STR_concatAllTermination(int limit, ...){

    limit = 1<<29;

    int termCount = 0;
    char *terminations[4] = {NULL,NULL+1,NULL+2,NULL};

    char *buffer;
    buffer = SDL_malloc(sizeof(char));
    if(buffer==NULL){
        return NULL;
    }
    buffer[0] = '\0';

    va_list params;
    va_start(params, limit);
    ///

    int i;
    for(i = 0; i < limit;i++){
        char *arg = va_arg ( params, char* );
        if(terminations[termCount]==arg){
            termCount++;
            if(termCount>4){
                break;
            }
        }else{
            termCount=0;
            buffer = STR_concatf2(buffer,arg);
        }
    }
    ///
    va_end(params);

    return buffer;
}
