#ifndef TEXTURY_H_INCLUDED
#define TEXTURY_H_INCLUDED

#include <SDL2/SDL.h>

#include "resources.h"
#include "preutils.h"

#if ALLOW_TEXTURE_MEMORY_DUMP
//eee
#endif // ALLOW_TEXTURE_MEMORY_DUMP

MemoryDumpStrucure(TEX_MemDump,TEX_XtraTexture);

/*
typedef struct TEX_MemDump{
    struct TEX_MemDump *node;
    struct TEX_XtraTexture *leaf;
} TEX_MemDump;*/

void TEX_freeTexturesByRenderer(SDL_Renderer *renderer);

typedef struct TEX_XtraTexture {
    SDL_Surface *raw;
    SDL_Texture *img;
    SDL_Rect clip_rect;
    int w;
    int h;
    SDL_Renderer *renderer;
} TEX_XtraTexture;


void TEX_init();
void TEX_exit();


SDL_Texture *TEX_newTextureFromFile(const char *file);
SDL_Texture *TEX_newTexture(SDL_Surface *src);

TEX_XtraTexture *TEX_newXtraTextureFromFile(const char *file);
TEX_XtraTexture *TEX_newXtraTexture(SDL_Surface *src);

void TEX_freeXtraTexture(TEX_XtraTexture *xtex);
void TEX_freeXtraTextureNotSurface(TEX_XtraTexture *xtex);

#endif // TEXTURY_H_INCLUDED
