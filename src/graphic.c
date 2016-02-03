#include "graphic.h"

void drawRectangle(SDL_Renderer *ren, SDL_Rect *rect, RGBA_COLOR *rgb){

  SDL_SetRenderDrawBlendMode(ren, SDL_BLENDMODE_BLEND);
  SDL_SetRenderDrawColor(ren, rgb->r, rgb->g, rgb->b, rgb->a);
  SDL_RenderFillRect(ren, rect);
  SDL_RenderPresent(ren);
}
