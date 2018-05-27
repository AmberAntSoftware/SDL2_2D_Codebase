#include <stdio.h>
#include <stdlib.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "resources.h"
#include "memorys.h"

//static MEM_Block* rects = NULL;
//static MEM_Block* texts = NULL;
//static MEM_Block* xtexts = NULL;
//static MEM_Block* surfs = NULL;

void TEX_init(){
    //rects = MEM_newBlock();
    //texts = MEM_newBlock();
    //xtexts = MEM_newBlock();
    //surfs = MEM_newBlock();
}

void TEX_exit(){ //handled in auto delete -- dereferences could cause problems otherwise
    //MEM_freeBlock(rects);
    //MEM_freeBlock(texts);
    //MEM_freeBlock(xtexts);
    //MEM_freeBlock(surfs);
}

SDL_Rect *TEX_newRect(){
    /*SDL_Rect* rect = MEM_mallocTo(rects,malloc(sizeof(SDL_Rect)),NULL);
    if(rect!=NULL){
        rect->x=0;rect->y=0;rect->w=0;rect->h=0;
        return rect;
    }
    printf("Could Not Generate empty SDL_Rect");*/
    return NULL;
}


SDL_Texture *TEX_loadImageIntoTextureFromFile(const char *file){
    SDL_Surface *dump = IMG_Load(file);
    if(dump != NULL){
        SDL_Texture *dst = SDL_CreateTextureFromSurface(RES_renderer, dump);
        SDL_FreeSurface(dump);
        if(dst == NULL){
            printf("Could Not Load \"%s\" Into A Usable Format\n", file);
            return NULL;
        } else {
            //MEM_addDataTo(texts,dst,SDL_DestroyTexture);
            return dst;
        }
    } else {
        printf("Error Loading File: %s\n", file);
    }
    return NULL;
}

XtraTexture *TEX_loadImageIntoXtraTextureFromFile(const char *file){
    SDL_Surface *dump = IMG_Load(file);
    if(dump != NULL){
        XtraTexture *dst = TEX_newXtraTexture(dump);
        SDL_FreeSurface(dump);
        if(dst == NULL || dst->img == NULL){
            printf("Could Not Load \"%s\" Into A Usable Format\n", file);
            TEX_freeXtraTexture(dst);
            return NULL;
        } else {
            //MEM_addDataTo(xtexts,dst,TEX_freeXtraTexture);
            return dst;
        }
    } else {
        printf("Could Not Locate: %s\n", file);
    }
    return NULL;
}

XtraTexture *TEX_newXtraTexture(SDL_Surface *src){
    if(src == NULL){
        return NULL;
    }
    XtraTexture *xtex = SDL_calloc(sizeof(XtraTexture), 1);
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
    SDL_SetTextureBlendMode(xtex->img, SDL_BLENDMODE_BLEND);
    return xtex;
}

void TEX_freeXtraTexture(void *xtexx){
    XtraTexture *xtex = (XtraTexture *)xtexx;
    if(xtex == NULL){
        return;
    }
    if(xtex->img != NULL){
        SDL_DestroyTexture(xtex->img);
    }
    SDL_free(xtex);
}
