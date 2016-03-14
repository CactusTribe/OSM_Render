#ifndef _GRAPHIC_H_ 
#define _GRAPHIC_H_

#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include "../model/osm_types.h"

#define NB_STYLES 10

typedef struct{
	int r;
	int g;
	int b;
	int a;
} RGBA_COLOR;

typedef struct{
	char *key;
	int weigth;
	RGBA_COLOR color_IN;
	RGBA_COLOR color_OUT;
} STYLE_ENTRY;

extern void drawWay(SDL_Renderer *ren, OSM_Way *way);

extern void OSM_Rendering(SDL_Renderer *ren);
extern STYLE_ENTRY* getStyleOf(char *key); 

#endif
