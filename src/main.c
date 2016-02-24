#include <SDL2/SDL.h>
#include <stdlib.h>
#include <stdio.h>
#include "graphic.h"
#include "parser/osm_parser.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include <libxml/parser.h>
#include <libxml/xpath.h>
#include <libxml/xpathInternals.h>
#include <libxml/tree.h>
int main(int argc, char** argv){
    SDL_Window* pWindow = NULL;
    SDL_Renderer *ren = NULL;

    SDL_Event evenements;
    int terminer = 0;

OSM_Bounds bounds;

getOSM_Bounds("./src/model/test.osm", &bounds);

printOSM_Bounds( bounds);


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
    SDL_RenderClear(ren); // Clear la fenêtre

    SDL_Rect r = {200,200,200,60};

    RGBA_COLOR red = {255, 0, 0, 255};
    RGBA_COLOR green = {0, 255, 0, 255};
    RGBA_COLOR blue = {0, 0, 255, 255};
    RGBA_COLOR black = {0, 0, 0, 255};
    
    drawRectangle(ren, &r, &blue);

    SDL_Point pt1 = {50,50};
    SDL_Point pt2 = {100,50};
    SDL_Point pt3 = {100,200};
    SDL_Point points[] = {pt1, pt2, pt3};
    POLYGON p = {points, 3};

    drawPolygon(ren, &p, &red);

    SDL_Point pt4 = {60,51};
    drawPoint(ren, &pt4, &black);

    SDL_RenderPresent(ren); // Affiche les modifications

    printf("Point in poly = %d\n", pointInPolygon(&pt4, &p));

    if( pWindow ){
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
