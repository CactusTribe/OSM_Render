#include "graphic.h"

void drawRectangle(SDL_Renderer *ren, SDL_Rect *rect, RGBA_COLOR *rgb){

  SDL_SetRenderDrawBlendMode(ren, SDL_BLENDMODE_BLEND);
  SDL_SetRenderDrawColor(ren, rgb->r, rgb->g, rgb->b, rgb->a);
  SDL_RenderFillRect(ren, rect);
  SDL_RenderPresent(ren);
}

void drawPolygon(SDL_Renderer *ren, POLYGON *p, RGBA_COLOR *rgb) {
	SDL_SetRenderDrawBlendMode(ren, SDL_BLENDMODE_BLEND);
  SDL_SetRenderDrawColor(ren, rgb->r, rgb->g, rgb->b, rgb->a);
  SDL_RenderDrawLines(ren, p->points, p->nbPoints);
  SDL_RenderPresent(ren);
}

void fill_poly(SDL_Renderer *ren, POLYGON *p){
	SDL_Point topLeft = p->points[0], botRight = p->points[0];
	SDL_Point current;

	for(int i=0; i < p->nbPoints-1; i++){
		current = p->points[i];

		if(current.x < topLeft.x) topLeft.x = current.x;
		if(current.y < topLeft.y) topLeft.y = current.y;
		if(current.x > botRight.x) botRight.x = current.x;
		if(current.y > botRight.y) botRight.y = current.y;

		topLeft.y--;
		topLeft.x--;
		botRight.x++;
		botRight.y++;
	}

	for(int i=topLeft.y; i < botRight.y; i++){
		for(int i=topLeft.y; i < botRight.y; i++){

		}	
	}
}