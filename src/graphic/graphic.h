#ifndef _GRAPHIC_H_ 
#define _GRAPHIC_H_

#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include "../model/osm_types.h"
#include "../model/OSM_ABR.h"
#include "../model/minHeap.h"

#define ARGSEP " :\t\n"
#define DICO_SIZE 1024

typedef struct{
	int r;
	int g;
	int b;
	int a;
} RGBA_COLOR;

typedef struct{
	char *key;
	char *value;
	int weigth_IN;
	int weigth_OUT;
	RGBA_COLOR color_IN;
	RGBA_COLOR color_OUT;
	int priority;
} STYLE_ENTRY;

void openStyleSheet(char *file);
void freeDico(STYLE_ENTRY *dico);
int tokenize_command(char* argl, char** argv);
void CreateHeapPriority(minHeap *hp, OSM_Data* data);

void drawWay(SDL_Renderer *ren, OSM_Way *way);
void drawNode(SDL_Renderer *ren, OSM_Node *node);
void drawTexte(SDL_Renderer *ren, int x, int y, int w, int h, 
	char *font, int size, char *texte, SDL_Color *color);
void drawOSM_ABR(ABR_Node *tree);

void OSM_Rendering(SDL_Window *pWindow, int w, int h, OSM_Data *data);
void CreateWindow(int w, int h);
void CreateRenderer();
void OSM_DestroyRenderer();
STYLE_ENTRY* getStyleOf(char *key, char *value); 

#endif
