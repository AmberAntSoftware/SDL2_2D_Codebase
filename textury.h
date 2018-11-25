#ifndef TEXTURY_H_INCLUDED
#define TEXTURY_H_INCLUDED

#include <resources.h>

#ifdef ALLOW_TEXTURE_MEMORY_DUMP
#if ALLOW_TEXTURE_MEMORY_DUMP == 1
eee
#endif // ALLOW_TEXTURE_MEMORY_DUMP
#endif // ALLOW_TEXTURE_MEMORY_DUMP
static struct TEX_MemDump{
    struct TEX_MemDump *node;
    TEX_MemDump *leaf;
} TEX_MemDump;

typedef struct TEX_XtraTexture {
    SDL_Surface *raw;
    SDL_Texture *img;
    SDL_Rect clip_rect;
    int w;
    int h;
} TEX_XtraTexture;

SDL_Texture *TEX_newTextureFromFile(const char *file);
SDL_Texture *TEX_newTexture(const SDL_Surface *src);

TEX_XtraTexture *TEX_newXtraTextureFromFile(const char *file);
TEX_XtraTexture *TEX_newXtraTexture(SDL_Surface *src);

void TEX_freeXtraTexture(TEX_XtraTexture *xtex);
void TEX_freeXtraTextureNotSurface(TEX_XtraTexture *xtex);

#endif // TEXTURY_H_INCLUDED
