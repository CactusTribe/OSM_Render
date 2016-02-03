#include "SDL2/SDL.h"
#include <stdlib.h>
#include <stdio.h>
#include "graphic.h"

int main(int argc, char** argv){
    SDL_Window* pWindow = NULL;
    SDL_Renderer *ren = NULL;

    SDL_Event evenements;
    int terminer = 0;

    /* Initialisation */
    if (SDL_Init(SDL_INIT_VIDEO) != 0 ){
        fprintf(stdout,"Échec de l'initialisation de la SDL (%s)\n",SDL_GetError());
        exit(1);
    }

    /* Création de la fenêtre */
    pWindow = SDL_CreateWindow("OSM_Render",SDL_WINDOWPOS_UNDEFINED,
                                                              SDL_WINDOWPOS_UNDEFINED,
                                                              640,
                                                              480,
                                                              SDL_WINDOW_SHOWN);

    ren = SDL_CreateRenderer(pWindow, 0, 0);

    SDL_SetRenderDrawColor(ren, 255, 255, 255, 255);
    SDL_RenderClear(ren);

    SDL_Rect r;
    r.x = 50;
    r.y = 50;
    r.w = 200;
    r.h = 60;

    RGBA_COLOR color;
    color.r = 255;
    color.g = 0;
    color.b = 0;
    color.a = 255;
    
    drawRectangle(ren, &r, &color);

    if( pWindow ){
        //SDL_Delay(3000)
        while(!terminer){
          SDL_WaitEvent(&evenements);
          if(evenements.window.event == SDL_WINDOWEVENT_CLOSE)
            terminer = 1;
        }

        SDL_DestroyRenderer(ren);
        SDL_DestroyWindow(pWindow);
    }
    else{
        fprintf(stderr,"Erreur de création de la fenêtre: %s\n",SDL_GetError());
        SDL_Quit();
        exit(1);
    }

    SDL_Quit();
    exit(0);
}
