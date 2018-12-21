#include "2DBase_defines.h"

#include <stdio.h>
#include <stdlib.h>

#include <SDL2/SDL.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "resources.h"
#include "xfonts.h"

static FNT_MemDump *FNT_dump = NULL;
static FNT_MemDump *FNT_first = NULL;
static void FNT_X_initDump(){
    if(FNT_dump==NULL){
        FNT_dump = calloc(sizeof(FNT_MemDump),1);
        FNT_first = FNT_dump;
    }
}
static void FNT_X_freeDump(){
    if(FNT_dump!=NULL){
        FNT_MemDump *cur = FNT_first;
        FNT_MemDump *next;
        while(cur!=NULL && cur->node!=NULL){
            next = cur->node;
            if(cur->leaf!=NULL){
                FNT_freeFont(cur->leaf);
            }
            free(cur);
            cur = next;
        }
    }
}
static void FNT_X_addToDump(FNT_Font *font){
    FNT_dump->leaf = font;
    FNT_dump->node = calloc(sizeof(FNT_MemDump),1);
    if(FNT_dump->node!=NULL){
        FNT_dump = FNT_dump->node;
    }else{
        printf("||| MEMORY ERROR: could not calloc 1 FNT_MemDump\n");
    }
}

static char cr = 255, cg = 255, cb = 255, ca = 255;


FNT_FontType *FNT_defFont = NULL;
int FNT_defPt = 12;
int FNT_defEffects = 0;



void FNT_init(){
#if ALLOW_FONTS
    if(!TTF_WasInit()){
        if(TTF_Init() == -1){
            printf("Couldn't init SDL TTF\n");
        }
    }
#if ALLOW_FONTS_MEMORY_DUMP
    FNT_X_initDump();
#endif // ALLOW_FONTS_MEMORY_DUMP

#endif // ALLOW_FONTS
}

void FNT_exit(){
#if ALLOW_FONTS

#if ALLOW_FONTS_MEMORY_DUMP
    FNT_X_freeDump();
#endif // ALLOW_FONTS_MEMORY_DUMP

    TTF_Quit();

#endif // ALLOW_FONTS


}

void FNT_setColor(char r, char g, char b, char a){
    cr = r;
    cb = b;
    cg = g;
    ca = a;
}

void FNT_setFontType(FNT_FontType *fontType){
    if(fontType != NULL){
        FNT_defFont = fontType;
    }
}
void FNT_setPt(int pt){
    if(pt > 0){
        FNT_defPt = pt;
    }
}
void FNT_setEffects(int effects){
    if(effects > -1){
        FNT_defEffects = effects;
    }
}

void FNT_XdrawText(char *text, FNT_FontType *type, int x, int y, int pt, int effects){
    if(pt < 1 || type == NULL || text == NULL){
        return;
    }
    //*
    int length = SDL_strlen(text);
    int i;
    SDL_Rect dest;
    dest.x = x;
    dest.w = 0;
    dest.y = y;
    dest.h = 0;
    float scale = pt / (1.0f * type->pt);
    scale = (pt / (1.0f * type->pt));
    dest.h = (int)(scale * type->letters[42]->glyph->h);
    //printf("i: %i\n",length);
    for(i = 0; i < length; i++){
        if(type->letters[(unsigned char)text[i]] != NULL){
            dest.w = (int)(scale * type->letters[(unsigned char)text[i]]->glyph->w);
            SDL_SetTextureColorMod(type->letters[(unsigned char)text[i]]->glyph->img, cr, cg, cb);
            SDL_SetTextureAlphaMod(type->letters[(unsigned char)text[i]]->glyph->img, ca);
            RES_drawImageRect(type->letters[(unsigned char)text[i]]->glyph, NULL, &dest);
            dest.x += dest.w;
        }
    }
    FNT_XdrawEffects(type, &dest, x, scale, effects);
    //*/
}

void FNT_XdrawText_kern(char *text, FNT_FontType *type, int x, int y, int pt, int effects){
    if(pt < 1 || type == NULL || text == NULL){
        return;
    }
    int length = SDL_strlen(text);
    int i;
    SDL_Rect dest;
    dest.x = x;
    dest.w = 0;
    dest.y = y;
    dest.h = 0;
    float scale = pt / (1.0f * type->pt);
    int last = 0;
    scale = (pt / (1.0f * type->pt));
    dest.h = (int)(scale * type->letters[32]->glyph->h);
    for(i = 0; i < length; i++){
        if(type->letters[(unsigned char)text[i]] != NULL){
            dest.w = (int)(scale * type->letters[(unsigned char)text[i]]->glyph->w);
            SDL_SetTextureColorMod(type->letters[(unsigned char)text[i]]->glyph->img, cr, cg, cb);
            SDL_SetTextureAlphaMod(type->letters[(unsigned char)text[i]]->glyph->img, ca);
            if(type->letters[(unsigned char)text[i]] != NULL){
                if(type->letters[last] != NULL){
                    dest.x += (int)(type->letters[last]->kernings[(unsigned char)text[i]] * scale);
                }
                last = text[i];
            }
            RES_drawImageRect(type->letters[(unsigned char)text[i]]->glyph, NULL, &dest);
        }
    }
    if(dest.x += type->letters[last] != NULL){
        dest.x += type->letters[last]->glyph->w;
    }
    FNT_XdrawEffects(type, &dest, x, scale, effects);
}

///FIXME color does not change back correctly for some reason when drawing other thing
///FIXME color shiould change back as it is break, not reutrn in no effects
void FNT_XdrawEffects(FNT_FontType *type, SDL_Rect *dest, int x, float scale, int effects){
    Uint8 r = 0, g = 0, b = 0, a = 255;
    SDL_GetRenderDrawColor(RES_renderer, &r, &g, &b, &a);
    SDL_SetRenderDrawColor(RES_renderer, cr, cg, cb, ca);
    switch(effects){
        case(FNT_PLAIN):
            break;
        case(FNT_UNDERLINE):
            SDL_RenderDrawLine(RES_renderer, x, dest->y + (int)(type->baseline * scale), dest->x, dest->y + (int)(type->baseline * scale));
            break;
        case(FNT_STRIKEOUT):
            SDL_RenderDrawLine(RES_renderer, x, dest->y + (int)(type->strikeline * scale), dest->x, dest->y + (int)(type->strikeline * scale));
            break;
        case(FNT_UNDERLINE|FNT_STRIKEOUT):
            SDL_RenderDrawLine(RES_renderer, x, dest->y + (int)(type->strikeline * scale), dest->x, dest->y + (int)(type->strikeline * scale));
            SDL_RenderDrawLine(RES_renderer, x, dest->y + (int)(type->baseline * scale), dest->x, dest->y + (int)(type->baseline * scale));
            break;
    }
    SDL_SetRenderDrawColor(RES_renderer,r,g,b,a);
}




FNT_Font *FNT_newFont(const char *fontFile, int maxPt){
    if(maxPt < 1){
        return NULL;
    }
    FNT_Font *ffont = SDL_malloc(sizeof(FNT_Font));
    if(ffont == NULL){
        return NULL;
    }
    TTF_Font *font = TTF_OpenFont(fontFile, maxPt);
    if(font == NULL){
        SDL_free(font);
        //printf("Could Not Load Font File\n");
        return NULL;
    }
    FNT_FontType *bold = FNT_newFontType(font, TTF_STYLE_BOLD, maxPt);
    if(bold == NULL){
        SDL_free(ffont);
        TTF_CloseFont(font);
        return NULL;
    }
    FNT_FontType *plain = FNT_newFontType(font, TTF_STYLE_NORMAL, maxPt);
    if(plain == NULL){
        FNT_freeFontType(bold);
        SDL_free(ffont);
        TTF_CloseFont(font);
        return NULL;
    }
    FNT_FontType *italic = FNT_newFontType(font, TTF_STYLE_ITALIC, maxPt);
    if(italic == NULL){
        FNT_freeFontType(bold);
        FNT_freeFontType(plain);
        SDL_free(ffont);
        TTF_CloseFont(font);
        return NULL;
    }
    FNT_FontType *boldItalic = FNT_newFontType(font, TTF_STYLE_BOLD | TTF_STYLE_ITALIC, maxPt);
    if(boldItalic == NULL){
        FNT_freeFontType(bold);
        FNT_freeFontType(plain);
        FNT_freeFontType(italic);
        SDL_free(ffont);
        TTF_CloseFont(font);
        return NULL;
    }
    ffont->bold = bold;
    ffont->plain = plain;
    ffont->italic = italic;
    ffont->boldItalic = boldItalic;
    ffont->font = font;
#if ALLOW_FONTS_MEMORY_DUMP
    FNT_X_addToDump(ffont);
#endif // ALLOW_FONTS_MEMORY_DUMP
    return ffont;
}

FNT_FontType *FNT_newFontType(TTF_Font *font, int style, int maxPt){
    FNT_FontType *ftype = SDL_calloc(sizeof(FNT_FontType), 1);
    if(ftype == NULL){
        return NULL;
    }
    TTF_SetFontStyle(font, style);
    int i;
    for(i = 0; i < 256; i++){
        ftype->letters[i] = NULL;
    }
    for(i = 32; i < 127; i++){
        ftype->letters[i] = FNT_newLetter(font, (char)i);
        if(ftype->letters[i] == NULL){
            //printf("NULL LETTER ");
        } else if(ftype->letters[i]->glyph == NULL){
            //printf("NULL GLYPH ");
        }
    }
    int topline = -TTF_FontAscent(font);//from top of font to baseline
    int baseline = -topline;
    int strikeline = baseline * 2 / 3;
    int linespace = TTF_FontLineSkip(font);
    SDL_Color dump;
    SDL_Surface *tmp = TTF_RenderGlyph_Blended(font, 'A', dump);
    int underscore = tmp->h;
    SDL_FreeSurface(tmp);
    ftype->underscore = underscore;
    ftype->linespace = linespace;
    ftype->strikeline = strikeline;
    ftype->baseline = baseline;
    ftype->topline = topline;
    ftype->pt = maxPt;
    for(i = 32; i < 127; i++){
        FNT_calcLetterSpace(font, ftype, ftype->letters[i], '\0' + i);
    }
    return ftype;
}

FNT_Letter *FNT_newLetter(TTF_Font *font, char letter){
    FNT_Letter *letr = SDL_calloc(sizeof(FNT_Letter), 1);
    if(letr == NULL){
        return NULL;
    }
    SDL_Color fgColor;
    fgColor.a = 0xFF;
    fgColor.r = 0xFF;
    fgColor.g = 0xFF;
    fgColor.b = 0xFF;
    //*
    SDL_Surface *glyph = TTF_RenderGlyph_Blended(font, (Uint16)letter, fgColor);
    if(glyph == NULL){
        SDL_free(letr);
        //printf("TTF ERROR \n");
        return NULL;
    }
    TEX_XtraTexture *xglyph = TEX_newXtraTexture(glyph);
    if(xglyph == NULL){
        SDL_free(letr);
        //printf("GLYPH ERROR\n");
        return NULL;
    }
    letr->glyph = xglyph;
    //*/
    SDL_FreeSurface(glyph);
    return letr;
}

void FNT_calcLetterSpace(TTF_Font *font, FNT_FontType *cache, FNT_Letter *store, char letter){
    if(store == NULL){
        return;
    }
    int i, w = 0, h = 0;
    char text[3];
    for(i = 0; i < 3; i++){
        text[i] = '\0';
    }
    text[1] = letter;
    for(i = 0; i < 256; i++){
        store->kernings[i] = 0;
        if(cache->letters[i] != NULL){
            text[0] = '\0' + i;
            TTF_SizeText(font, text, &w, &h);
            store->kernings[i] = w - cache->letters[i]->glyph->w;
        }
    }
}

void FNT_freeFont(FNT_Font *font){
    FNT_freeFontType(font->bold);
    FNT_freeFontType(font->plain);
    FNT_freeFontType(font->italic);
    FNT_freeFontType(font->boldItalic);
    if(font->font != NULL){
        TTF_CloseFont(font->font);
    }
    SDL_free(font);
}

void FNT_freeFontType(FNT_FontType *font){
    if(font == NULL){
        return;
    }
    int i;
    for(i = 0; i < 256; i++){
        FNT_freeLetter(font->letters[i]);
    }
    SDL_free(font);
}

void FNT_freeLetter(FNT_Letter *letter){
    if(letter == NULL){
        return;
    }
    TEX_freeXtraTexture(letter->glyph);
    SDL_free(letter);
}
