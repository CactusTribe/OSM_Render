#ifndef _GRAPHIC_H_ 
#define _GRAPHIC_H_

#include <SDL2/SDL.h>

typedef struct{
	int r;
	int g;
	int b;
	int a;
} RGBA_COLOR;

extern void drawRectangle(SDL_Renderer *, SDL_Rect *, RGBA_COLOR *);

#endif
