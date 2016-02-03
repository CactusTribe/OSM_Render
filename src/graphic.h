#ifndef _GRAPHIC_H_ 
#define _GRAPHIC_H_

typedef struct{
	int r;
	int g;
	int b;
	int a;
} RGBA_COLOR;

void drawRectangle(SDL_Renderer *ren, SDL_Rect *rect, RGBA_COLOR *rgb);

#endif