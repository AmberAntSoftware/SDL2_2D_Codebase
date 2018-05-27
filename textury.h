#ifndef TEXTURY_H_INCLUDED
#define TEXTURY_H_INCLUDED

typedef struct XtraTexture {
    SDL_Texture *img;
    SDL_Rect clip_rect;
    int w;
    int h;
} XtraTexture;

/*typedef struct LayeredMap{
    XtraTexture *img;
    SDL_Surface *col;
    struct LayeredMap *next;
}LayeredMap;

typedef struct SpriteSheet{
    SDL_Rect *collision;//actual collision/hitbox
    SDL_Rect *scaled;//destination drawn
    SDL_Rect *clip;//image part to grab
    XtraTexture *img;
}SpriteSheet;*/

void TEX_init();
void TEX_exit();

SDL_Texture *TEX_loadImageIntoTextureFromFile(const char *file);
XtraTexture *TEX_loadImageIntoXtraTextureFromFile(const char *file);
SDL_Rect *TEX_newRect();
XtraTexture *TEX_newXtraTexture(SDL_Surface *src);
void TEX_freeXtraTexture(void *xtexx);

#endif // TEXTURY_H_INCLUDED
