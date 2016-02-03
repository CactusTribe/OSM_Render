#include "graphic.h"
#include "SDL2/SDL.h"

void drawRectangle(SDL_Renderer *ren, SDL_Rect *rect, RGBA_COLOR *rgb){

  SDL_SetRenderDrawBlendMode(ren, SDL_BLENDMODE_BLEND);
  SDL_SetRenderDrawColor(ren, RGBA_COLOR->r, RGBA_COLOR->g, RGBA_COLOR->b, RGBA_COLOR->a);
  SDL_RenderFillRect(ren, &rect);
  SDL_RenderPresent(ren);
}