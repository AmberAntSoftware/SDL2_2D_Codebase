#ifndef STRINGYS_H_INCLUDED
#define STRINGYS_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <SDL2/SDL.h>


#define CAT(...) STR_concatAllTermination(0, __VA_ARGS__ ,NULL,NULL+1,NULL+2,NULL)



char* STR_concat(const char* str0, const char* str1);
char* STR_concatf0(char* str0_toFree, const char* str1);
char* STR_concatf1(const char* str0, char* str1_toFree);
char* STR_concatf2(char* str0_toFree, char* str1_toFree);
char* STR_concatAll(int char_pointer_count, ...);
char* STR_concatAllTermination(int nothing, ...);

#endif // STRINGYS_H_INCLUDED
