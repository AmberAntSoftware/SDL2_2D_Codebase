#include "../2DBase_defines.h"
#include "components.h"

#define pcase(constFunctionPointer) ((long long int)(& constFunctionPointer))
#define ccase(constFunctionPointer) ((const long long int)(& constFunctionPointer))


static void GUI_X_DefaultElementRender(GUI_Element *element){

    if(element == NULL){
        return;
    }

    switch(element->type){
        //*
        case(GUI_TYPE_CONTAINER):
            break;
        case(GUI_TYPE_BUTTON):
            break;
        case(GUI_TYPE_IMAGE):
            break;
        case(GUI_TYPE_LABEL):
            break;
        case(GUI_TYPE_RADIOBUTTON):
            break;
        case(GUI_TYPE_TEXTFIELD):
            break;
        case(GUI_TYPE_TEXTAREA):
            break;
        case(GUI_TYPE_SWITCH):
            break;
        case(GUI_TYPE_LIST):
            break;
        //*/
    }

}


void GUI_setRenderer(SDL_Renderer *renderer){
    //
}

//add element to dirty render queue
//if it terminates early, remove the afflicted element from the queue
void GUI_removeFromDirtyQueue(GUI_Element *element){
    //
}
void GUI_addToDirtyQueue(GUI_Element *element){
    //
}

void GUI_markElementDirty(GUI_Element *element){
    if(element==NULL||element->dirtyFlag){//this render tree is already marked
        return;
    }

    GUI_Element *child = element;
    GUI_Element *neighbor;
    while(child!=NULL){
        if(child->dirtyFlag){
            GUI_removeFromDirtyQueue(child);
            break;
        }
        neighbor = child->neighbor;
        while(neighbor!=NULL){
            if(neighbor->dirtyFlag){
                GUI_removeFromDirtyQueue(neighbor);
                break;
            }
            neighbor->dirtyFlag = 1;
            neighbor = neighbor->neighbor;
        }
        child->dirtyFlag = 1;
        child = child->child;
    }

    GUI_addToDirtyQueue(element);

}

void GUI_removeFromPaintedQueue(GUI_Element *element){
    //
}
void GUI_addToPaintedQueue(GUI_Element *element){
    //
}

void GUI_paintSingleElement(GUI_Element *element){

    if(element == NULL){
        return;
    }

    int w=0,h=0;
    SDL_GetRendererOutputSize(RES_renderer, &w, &h);
    SDL_Texture *copiedDisplay = SDL_CreateTexture(RES_renderer,SDL_PIXELFORMAT_ARGB8888,SDL_TEXTUREACCESS_STREAMING,w,h);
    SDL_SetRenderTarget(RES_renderer,copiedDisplay);
    SDL_RenderCopy(RES_renderer,RES_texture,NULL,NULL);
    SDL_SetRenderTarget(RES_renderer,RES_texture);

    GUI_RenderElement(element);

    GUI_Element *neighbor = element->child;
    while(neighbor!=NULL){
        SDL_Rect *tmp = &(neighbor->internal.boundsCalcuated);
        SDL_RenderCopy(RES_renderer,copiedDisplay,tmp,tmp);
        neighbor = neighbor->neighbor;
    }

    SDL_DestroyTexture(copiedDisplay);

}


void GUI_RenderElement(GUI_Element *element){

    if(element == NULL){
        return;
    }

    if(element->backgroundColor.a == 255){
        //regular render
    }else if(element->backgroundColor.a != 0){
        //reverse tree render until opaque color is reached
    }

    //function call to case of default render for elements
    GUI_X_DefaultElementRender(element);

}

void GUI_MouseEnteredOnElement(GUI_Element *element){
    /**void *data = NULL;
    switch(pcase(data)){
    case(pcase(GUI_MouseExitedOnElement)):
        break;
    }**/
    //
}
void GUI_MouseExitedOnElement(GUI_Element *element){
    //
}
void GUI_MousePressedOnElement(GUI_Element *element){
    //
}
void GUI_MouseDraggedOnElement(GUI_Element *element){
    //
}
void GUI_MouseReleasedOnElement(GUI_Element *element){
    //
}
void GUI_MouseMovedOnElement(GUI_Element *element){
    //
}

void GUI_KeyPressedOnElement(GUI_Element *element){
    //
}
void GUI_KeyReleasedOnElement(GUI_Element *element){
    //
}
void GUI_KeyHeldOnElement(GUI_Element *element){
    //
}
