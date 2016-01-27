#include "../header/prototypes.h"
 
void draw(void){
  SDL_SetRenderDrawColor(getrenderer(), 255, 255, 255, 255);
  SDL_RenderClear(getrenderer());
  SDL_RenderPresent(getrenderer());

  SDL_Delay(1);
}
 
 
void delay(unsigned int frameLimit){
  unsigned int ticks = SDL_GetTicks();

  if (frameLimit < ticks){
    return;
  }

  if (frameLimit > ticks + 16){
    SDL_Delay(16);
  }

  else{
    SDL_Delay(frameLimit - ticks);
  }
}