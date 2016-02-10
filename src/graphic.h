#ifndef _GRAPHIC_H_ 
#define _GRAPHIC_H_

#include <SDL2/SDL.h>

typedef struct{
	int r;
	int g;
	int b;
	int a;
} RGBA_COLOR;

// Remplissage de points dans le sens horraire (adoption de la convention xml plus tard)
typedef struct{
	SDL_Point *points; 
	int nbPoints;
} POLYGON;

extern void drawRectangle(SDL_Renderer *, SDL_Rect *, RGBA_COLOR *);
extern void drawPolygon(SDL_Renderer *ren, POLYGON *p, RGBA_COLOR *rgb);

#endif
