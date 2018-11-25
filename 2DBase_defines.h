/**
Due note that the dependencies caused by linkage must be manually removed in link settings
in order to remove crashes made by missing .so or .dll files that are not required
**/

///NOTE THAT ALL 'INIT' DEFINE BEHAVIOR
///UNDEFINING OR SETTING TO FALSE WILL CAUSE
///ALL FUNIONS TO BECOME STUBS THAT RETURN NULL POINTERS OR DO NOTHING
///
///EXCEPTIONS: ""WindowGraphics"", ""WindowEvents""
///WILL STILL BE USUABLE STRUCTURES ON CREATION OR FREE
///
///This is in effort to minimize changes made to isolate components
///as long as the internal functions are used to manage the data



#define ALLOW_RESOURCES 1
#define ALLOW_RESOURCES_MEMORY_DUMP 1

#define ALLOW_EVENTS 1
#define ALLOW_EVENTS_MEMORY_DUMP 1

#define ALLOW_TINY_FONT 1
#define ALLOW_TTF 1
#define ALLOW_TTF_MEMORY_DUMP 1

#define ALLOW_AUDIO 1
#define ALLOW_AUDIO_MEMORY_DUMP 1

#define ALLOW_TEXTURES 1
#define ALLOW_TEXTURE_MEMORY_DUMP 1



///Sets of Default Headers To Add
#define ADD_HEADERS_SYSTEM 1
#define ADD_HEADERS_CODEBASE 2

///Edit Or Takeaway To Change Default Headers
#ifdef ADD_HEADERS_SYSTEM
#if ADD_HEADERS_SYSTEM

#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <math.h>

#endif // ALLOW_ALL_HEADERS
#endif // ALLOW_ALL_HEADERS

#ifdef ADD_HEADERS_CODEBASE
#if ADD_HEADERS_CODEBASE

#include <SDL2/SDL.h>
#include "resources.h"
#include "events.h"
#include "audios.h"
#include "textury.h"
#include "xfonts.h"

#endif // ALLOW_ALL_HEADERS
#endif // ALLOW_ALL_HEADERS

#ifndef 2DBASE_DEFINES_H_INCLUDED
#define 2DBASE_DEFINES_H_INCLUDED

#endif // 2DBASE_DEFINES_H_INCLUDED
