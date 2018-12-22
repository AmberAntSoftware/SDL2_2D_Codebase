#ifndef STRINGYS_H_INCLUDED
#define STRINGYS_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>

#include <SDL2/SDL.h>

#include <SDL2/SDL.h>

#include "resources.h"

char* STR_concat(char* str0, char* str1);
char* STR_concatf0(char* str0_toFree, char* str1);
char* STR_concatf2(char* str0, char* str1_toFree);
char* STR_concatf3(char* str0_toFree, char* str1_toFree);
char* STR_concatAll(int char_pointer_count, ...);

#endif // STRINGYS_H_INCLUDED
