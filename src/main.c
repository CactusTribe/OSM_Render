#include "../header/prototypes.h"
 
int main(int argc, char *argv[]){
    SDL_Event evenements;
    int terminer = 0;
 
    // Initialisation
    init("OpenStreetMap Render");
    
    /*
    // ESSAIS -------------------------
    SDL_Rect position;
    position.x = 0;
    position.y = 0;

    SDL_Surface *rectangle = NULL;
    rectangle = SDL_CreateRGBSurface(SDL_SWSURFACE, 220, 180, 32, 0, 0, 0, 0);
    SDL_FillRect(rectangle, NULL, SDL_MapRGB(ecran->format, 0, 255, 255));
    SDL_BlitSurface(rectangle, NULL, ecran, &position);
    //---------------------------------
    */

    while(!terminer){
      SDL_WaitEvent(&evenements);
      if(evenements.window.event == SDL_WINDOWEVENT_CLOSE)
        terminer = 1;

      draw();
    }

    //SDL_FreeSurface(rectangle);

    atexit(cleanup);
    exit(0);
}