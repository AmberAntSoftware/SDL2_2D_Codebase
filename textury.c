#include "2DBase_defines.h"

#include <stdio.h>
#include <stdlib.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "resources.h"
#include "textury.h"

#include "preutils.h"

MemoryDumpStaticLocals(TEX_MemDump,TEX_freeXtraTexture,TEX_XtraTexture,TEX_X_initDump,TEX_X_freeDump,TEX_X_addToDump,TEX_X_removeFromDump);

void TEX_freeTexturesByRenderer(SDL_Renderer *renderer){
    if(DMP_X_X_first!=NULL){
        TEX_MemDump *cur = DMP_X_X_first;
        TEX_MemDump *next;
        int hold = 0;
        while(cur!=NULL){
            next = cur->node;
            if(cur->leaf!=NULL && renderer==cur->leaf->renderer){
                TEX_X_removeFromDump(cur->leaf);
                return;
            }
            cur = next;
        }
    }
}


void TEX_init(){
    TEX_X_initDump();
#if ALLOW_TEXTURES

#endif // ALLOW_TEXTURES
}

void TEX_exit(){
    TEX_X_freeDump();
#if ALLOW_TEXTURES


#if ALLOW_TEXTURE_MEMORY_DUMP

#endif // ALLOW_RESOURCES_MEMORY_DUMP
#endif // ALLOW_TEXTURES

}


SDL_Texture *TEX_newTextureFromFile(const char *file){
///#if ALLOW_TEXTURES
    SDL_Surface *dump = IMG_Load(file);
    if(dump != NULL){
        SDL_Texture *dst = SDL_CreateTextureFromSurface(RES_renderer, dump);
        SDL_FreeSurface(dump);
        if(dst == NULL){
            printf("Could Not Load \"%s\" Into A Usable Format\n", file);
            return NULL;
        } else {
            TEX_XtraTexture *container = calloc(sizeof(TEX_XtraTexture),1);
            container->img = dst;
            container->raw = NULL;
            container->renderer = RES_renderer;
            TEX_X_addToDump(container);
            //MEM_addDataTo(texts,dst,SDL_DestroyTexture);
            return dst;
        }
    } else {
        printf("Error Loading File: %s\n", file);
    }
///#endif // ALLOW_TEXTURES
    return NULL;
}

SDL_Texture *TEX_newTexture(SDL_Surface *src){
    return SDL_CreateTextureFromSurface(RES_renderer, src);
}

TEX_XtraTexture *TEX_newXtraTextureFromFile(const char *file){
    SDL_Surface *dump = IMG_Load(file);
    if(dump != NULL){
        TEX_XtraTexture *dst = TEX_newXtraTexture(dump);
        SDL_FreeSurface(dump);
        if(dst == NULL || dst->img == NULL){
            printf("Could Not Load \"%s\" Into A Usable Format\n", file);
            TEX_freeXtraTexture(dst);
            return NULL;
        } else {
            TEX_X_addToDump(dst);
            //MEM_addDataTo(xtexts,dst,TEX_freeXtraTexture);
            return dst;
        }
    } else {
        printf("Could Not Locate: %s\n", file);
    }
    return NULL;
}

TEX_XtraTexture *TEX_newXtraTexture(SDL_Surface *src){
    if(src == NULL){
        return NULL;
    }
    TEX_XtraTexture *xtex = SDL_calloc(sizeof(TEX_XtraTexture), 1);
    if(xtex == NULL){
        return NULL;
    }
    xtex->img = SDL_CreateTextureFromSurface(RES_renderer, src);
    if(xtex->img == NULL){
        SDL_free(xtex);
        return NULL;
    }
    if((&(src->clip_rect)) != NULL){
        xtex->clip_rect.x = src->clip_rect.x;
        xtex->clip_rect.y = src->clip_rect.y;
        xtex->clip_rect.w = src->clip_rect.w;
        xtex->clip_rect.h = src->clip_rect.h;
        xtex->w = src->clip_rect.w;
        xtex->h = src->clip_rect.h;
    } else {
        xtex->clip_rect.x = 0;
        xtex->clip_rect.y = 0;
        xtex->clip_rect.w = src->w;
        xtex->clip_rect.h = src->h;
        xtex->w = src->w;
        xtex->h = src->h;
    }
    xtex->renderer = RES_renderer;
    SDL_SetTextureBlendMode(xtex->img, SDL_BLENDMODE_BLEND);
    return xtex;
}





void TEX_freeXtraTexture(TEX_XtraTexture *xtex){
    if(xtex == NULL){
        return;
    }
    if(xtex->img != NULL){
        SDL_DestroyTexture(xtex->img);
    }
    if(xtex->raw!=NULL){
        SDL_FreeSurface(xtex->raw);
    }
    SDL_free(xtex);
}

void TEX_freeXtraTextureNotSurface(TEX_XtraTexture *xtex){
    if(xtex == NULL){
        return;
    }
    if(xtex->img != NULL){
        SDL_DestroyTexture(xtex->img);
    }
    SDL_free(xtex);
}
