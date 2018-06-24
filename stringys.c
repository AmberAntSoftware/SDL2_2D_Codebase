#include <stdio.h>
#include <stdlib.h>

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

    SDL_strlcpy(buffer,str0,len);
    SDL_strlcat(buffer,str1,len);

    return buffer;

}
