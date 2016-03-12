#ifndef _GRAPHIC_H_ 
#define _GRAPHIC_H_

#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include "../model/osm_bounds.h"

typedef struct{
	int r;
	int g;
	int b;
	int a;
} RGBA_COLOR;

extern void drawWay(SDL_Renderer *ren, OSM_Way *way, RGBA_COLOR *rgb, int weigth);

#endif
