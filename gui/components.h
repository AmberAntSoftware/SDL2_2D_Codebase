#include "../2DBase_defines.h"
#ifndef COMPONENTS_H_INCLUDED
#define COMPONENTS_H_INCLUDED

#define GUI_ElementType int

///NOTE all float paddings may be redundant in most case, but not in partial component widths

enum GUI_ACTION_TYPES{
    GUI_MOUSE_PRESS,
    GUI_MOUSE_RELEASE,
    GUI_MOUSE_DRAG,
    GUI_MOUSE_MOVE,
    GUI_KEY_PRESSED,
    GUI_KEY_HELD,
    GUI_KEY_RELEASED
};

enum GUI_ELEMENT_TYPES{
    GUI_TYPE_BUTTON,
    GUI_TYPE_RADIOBUTTON,
    GUI_TYPE_TEXTFIELD,
    GUI_TYPE_TEXTAREA,
    GUI_TYPE_LABEL,
    GUI_TYPE_SWITCH,
    GUI_TYPE_SLIDER,
    GUI_TYPE_IMAGE,
    GUI_TYPE_SPANS,///literally spacers
    GUI_TYPE_RULE,///literally lines
    GUI_TYPE_SCROLLPANEL,
    GUI_TYPE_LIST,
    GUI_TYPE_CONTAINER
};

typedef struct GUI_RectFloat{
	float x;
	float y;
	float w;
	float h;
} GUI_RectFloat;

typedef struct GUI_OffsetInt{
	int top;
	int bottom;
	int left;
	int right;
} GUI_OffsetInt;

typedef struct GUI_OffsetFloat{
	float top;
	float bottom;
	float left;
	float right;
} GUI_OffsetFloat;

typedef struct GUI_OffsetBounds{

	GUI_OffsetInt padding;
	//for decimal widths
	GUI_OffsetFloat paddingFloat;

	char isRatio;
	GUI_OffsetFloat paddingRatio;

} GUI_OffsetBounds;

typedef struct GUI_Text{

    int length;
    char *text;

    int pt;
    int pixelWidth;

    int isKern;
    int drawFlags;

} GUI_Text;

////////////////////////////////////////////
// //
////////////////////////////////////////////

typedef struct GUI_BaseElement{

    //purely for details
    SDL_Rect location;
    //attach first elements here in series with neighbors
    struct GUI_Element *base;

}GUI_BaseElement;

////////////////////////////////////////////
// //
////////////////////////////////////////////

typedef struct GUI_ElementBounds{

    //contains the values thereof width and height and x, y in actual draw space
	SDL_Rect boundsCalcuated;
	//contains the values thereof width and height and x, y in relative draw space
	GUI_RectFloat boundsFloat;

    //contains and controls the actual size if used
    //any 0 values are treated as not used
	char isRatio;
	GUI_RectFloat boundsRatio;

} GUI_ElementBounds;

typedef struct GUI_HTML_Element{

    GUI_ElementType type;

    GUI_ElementBounds margin;
    GUI_ElementBounds border;
    GUI_ElementBounds padding;
    GUI_ElementBounds internal;

    struct GUI_Element *parent;///the container of this element
    struct GUI_Element *child;///used for elements in this element
    struct GUI_Element *neighbor;///used for local elements in the parent

} GUI_HTML_Element;

typedef struct GUI_Element{

    char dirtyFlag;
    SDL_Color backgroundColor;

    GUI_ElementType type;

    GUI_ElementBounds margin;
    GUI_ElementBounds internal;

    struct GUI_Element *parent;///the container of this element
    struct GUI_Element *child;///used for elements in this element
    struct GUI_Element *neighbor;///used for local elements in the parent

} GUI_Element;







GUI_Element *GUI_elementLastFocused;
GUI_Element *GUI_elementHasFocus;

void GUI_setRenderer(SDL_Renderer *renderer);
void GUI_removeFromDirtyQueue(GUI_Element *element);
void GUI_addToDirtyQueue(GUI_Element *element);
void GUI_markElementDirty(GUI_Element *element);

void GUI_removeFromPaintedQueue(GUI_Element *element);
void GUI_addToPaintedQueue(GUI_Element *element);
void GUI_paintSingleElement(GUI_Element *element);



void GUI_RenderElement(GUI_Element *element);

//Default Mouse Abstractions
void GUI_MouseEnteredOnElement(GUI_Element *element);
void GUI_MouseExitedOnElement(GUI_Element *element);
void GUI_MousePressedOnElement(GUI_Element *element);
void GUI_MouseDraggedOnElement(GUI_Element *element);
void GUI_MouseReleasedOnElement(GUI_Element *element);
void GUI_MouseMovedOnElement(GUI_Element *element);

//Default Key Abstractions
void GUI_KeyPressedOnElement(GUI_Element *element);
void GUI_KeyReleasedOnElement(GUI_Element *element);
void GUI_KeyHeldOnElement(GUI_Element *element);


#endif // COMPONENTS_H_INCLUDED
