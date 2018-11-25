#ifndef XFONTS_H_INCLUDED
#define XFONTS_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>

#include <SDL2/SDL.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "resources.h"
#include "textury.h"
#include "memorys.h"

#define FNT_PLAIN 0
#define FNT_UNDERLINE 1
#define FNT_STRIKEOUT 2

#define FNT_drawText(textPtr, x, y) {FNT_drawText(text, x, y, FNT_defPt);}
#define FNT_drawText_kern(textPtr, x, y) {FNT_drawText_ker(text, x, y, FNT_defPt);}

#define FNT_drawText_underline(textPtr, x, y) {FNT_XdrawText(textPtr,FNT_defFont, x, y, FNT_defPt, FNT_UNDERLINE);}
#define FNT_drawText_underline_kern(textPtr, x, y) {FNT_XdrawText_kern(textPtr,FNT_defFont, x, y, FNT_defPt, FNT_UNDERLINE);}
#define FNT_drawText_strike(textPtr, x, y) {FNT_XdrawText(textPtr,FNT_defFont, x, y, FNT_defPt, FNT_STRIKEOUT);}
#define FNT_drawText_strike_kern(textPtr, x, y) {FNT_XdrawText_kern(textPtr,FNT_defFont, x, y, FNT_defPt, FNT_STRIKEOUT);}
#define FNT_drawText_strike_underline(textPtr, x, y) {FNT_XdrawText(textPtr,FNT_defFont, x, y, FNT_defPt, FNT_STRIKEOUT|FNT_UNDERLINE);}
#define FNT_drawText_strike_underline_kern(textPtr, x, y) {FNT_XdrawText_kern(textPtr,FNT_defFont, x, y, FNT_defPt, FNT_STRIKEOUT|FNT_UNDERLINE);}

#define FNT_drawText_underline(textPtr, x, y, pt) {FNT_XdrawText(textPtr,FNT_defFont, x, y, pt < 1 ? FNT_defPt : pt, FNT_UNDERLINE);}
#define FNT_drawText_underline_kern(textPtr, x, y, pt) {FNT_XdrawText_kern(textPtr,FNT_defFont, x, y, pt < 1 ? FNT_defPt : pt, FNT_UNDERLINE);}
#define FNT_drawText_strike(textPtr, x, y, pt) {FNT_XdrawText(textPtr,FNT_defFont, x, y, pt < 1 ? FNT_defPt : pt, FNT_STRIKEOUT);}
#define FNT_drawText_strike_kern(textPtr, x, y, pt) {FNT_XdrawText_kern(textPtr,FNT_defFont, x, y, pt < 1 ? FNT_defPt : pt, FNT_STRIKEOUT);}
#define FNT_drawText_strike_underline(textPtr, x, y, pt) {FNT_XdrawText(textPtr,FNT_defFont, x, y, pt < 1 ? FNT_defPt : pt, FNT_STRIKEOUT|FNT_UNDERLINE);}
#define FNT_drawText_strike_underline_kern(textPtr, x, y, pt) {FNT_XdrawText_kern(textPtr,FNT_defFont, x, y, pt < 1 ? FNT_defPt : pt, FNT_STRIKEOUT|FNT_UNDERLINE);}

static struct FNT_MemDump{
    struct FNT_MemDump *node;
    TEX_MemDump *leaf;
} FNT_MemDump;


typedef struct FNT_Letter {
    TEX_XtraTexture *glyph;
    int kernings[256];//95 32-126 visible characters [inclusive]
} FNT_Letter;

typedef struct FNT_FontType {
    int pt;
    int baseline; //this is 0 -- e.g where the text is drawn at, but since texture, will be moved accordingly
    int topline;
    int strikeline;
    int underscore;
    int linespace;
    FNT_Letter *letters[256];
} FNT_FontType;

///Approximately 140KB not including textures and Font
typedef struct FNT_Font {
    TTF_Font *font;

    FNT_FontType *plain;
    FNT_FontType *bold;
    FNT_FontType *italic;
    FNT_FontType *boldItalic;

} FNT_Font;


FNT_FontType *FNT_defFont;
int FNT_defPt;
int FNT_defEffects;


void FNT_init();
void FNT_exit();

void FNT_drawText(char *text, int x, int y, int pt);
void FNT_drawText_kern(char *text, int x, int y, int pt);

void FNT_XdrawText(char *text, FNT_FontType *type, int x, int y, int pt, int effects);
void FNT_XdrawText_kern(char *text, FNT_FontType *type, int x, int y, int pt, int effects);
void FNT_XdrawEffects(FNT_FontType *type, SDL_Rect *dest, int x, float scale, int effects);

//void FNT_drawText(char *text,FNT_FontType *type, int x, int y, int pt, int effect);
//void FNT_drawText_kern(char *text,FNT_FontType *type, int x, int y, int pt, int effect);

void FNT_setColor(char r, char g, char b, char a);
void FNT_setFontType(FNT_FontType *fontType);
void FNT_setPt(int pt);
void FNT_setEffects(int effects);


///does nothing
//void FNT_newFontSpec(const char* plain, const char* italic, const char* bold, const char* boldItalic,int maxPt);
FNT_Font *FNT_newFont(const char *fontFile, int maxPt);
FNT_FontType *FNT_newFontType(TTF_Font *font, int style, int maxPt);
FNT_Letter *FNT_newLetter(TTF_Font *font, char letter);
void FNT_calcLetterSpace(TTF_Font *font, FNT_FontType *cache, FNT_Letter *store, char letter);

void FNT_freeFont(FNT_Font *font);
void FNT_freeFontType(FNT_FontType *fontt);
void FNT_freeLetter(FNT_Letter *letter);


#endif // XFONTS_H_INCLUDED
