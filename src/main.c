#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <SDL2/SDL_ttf.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include <libxml/parser.h>
#include <libxml/xpath.h>
#include <libxml/xpathInternals.h>
#include <libxml/tree.h>

#include "parser/osm_parser.h"
#include "graphic/graphic.h"

int main(int argc, char** argv){

    // ################# PARSER ##################

	osmParserFile* osmFile;
	osmParserDataSet* osmDataSet;

	open_OSM_ParserFile("./src/model/test.osm", &osmFile);

    getOSM_Bounds(osmFile, &osmDataSet);
    OSM_Bounds bounds;
    bounds = bind_OSM_Bounds( getDataSet_element( osmDataSet, 0)    );
    printOSM_Bounds(bounds);

    getOSM_Node(osmFile, &osmDataSet);
    printf("nb nodes: %d\n", getDataSet_lenght(osmDataSet));
    OSM_Node node;
    int index;
    for(index=0; index< getDataSet_lenght(osmDataSet) ; index++)
    {
    	node = bind_OSM_Node( getDataSet_element( osmDataSet, index)    );
    	printOSM_Node(node);
    }
    freeDataSet(osmDataSet);


    getOSM_Way(osmFile, &osmDataSet);
    printf("nb ways: %d\n", getDataSet_lenght(osmDataSet));
    OSM_Way way;
    for(index=0; index< getDataSet_lenght(osmDataSet) ; index++)
    {
        way = bind_OSM_Way( getDataSet_element( osmDataSet, index)    );
        printOSM_Way(way);
    }
    freeDataSet(osmDataSet);

    getOSM_Relation(osmFile, &osmDataSet);
    printf("nb relations: %d", getDataSet_lenght(osmDataSet));

    freeDataSet(osmDataSet);


    // ################# AFFICHAGE ##################

    SDL_Window* pWindow = NULL;
    SDL_Renderer *ren = NULL;
    SDL_Event evenements;
    int terminer = 0;

    /* Initialisation */
    if (SDL_Init(SDL_INIT_VIDEO) != 0 ){
        fprintf(stdout,"Échec de l'initialisation de la SDL (%s)\n",SDL_GetError());
        exit(1);
    }
    if (TTF_Init() != 0){
        fprintf(stdout,"Échec de l'initialisation de la SDL_ttf (%s)\n",SDL_GetError());
        exit(1); 
    }

    /* Création de la fenêtre */
    pWindow = SDL_CreateWindow("OSM_Render",SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED, 640, 480, SDL_WINDOW_SHOWN);

    ren = SDL_CreateRenderer(pWindow, 0, 0);

    SDL_SetRenderDrawColor(ren, 255, 255, 255, 255);
    SDL_RenderClear(ren); // Clear la fenêtre

    OSM_Rendering(ren);
    
    // Affichage texte ------------------------------
    SDL_Color black = {0, 0, 0}; 
    drawTexte(ren, 200, 200, 100, 25, "fonts/times.ttf", 24, "texte", &black);
    //  ----------------------------------------------

    SDL_RenderPresent(ren); // Affiche les modifications

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

    TTF_Quit();
    SDL_Quit();
    exit(0);
}
