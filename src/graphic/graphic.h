#ifndef _GRAPHIC_H_ 
#define _GRAPHIC_H_

#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include "../model/osm_types.h"
#include "../model/OSM_ABR.h"
#include "../model/minHeap.h"

#define ARGSEP " :\t\n"
#define DICO_SIZE 1024

#define deg2rad(d) (((d)*M_PI)/180)
#define rad2deg(d) (((d)*180)/M_PI)
#define earth_radius 6378137

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

// Gestion des styles
void openStyleSheet(char *file);
void freeDico(STYLE_ENTRY *dico);
int tokenize_command(char* argl, char** argv);
void CreateHeapPriority(minHeap *hp, OSM_Data* data);
STYLE_ENTRY* getStyleOf(char *key, char *value);

// Affichage des différents éléments
void drawWay(SDL_Renderer *ren, OSM_Way *way);
void draw_openedWay(SDL_Renderer *ren, OSM_Way *way, STYLE_ENTRY *style);
void draw_closedWay(SDL_Renderer *ren, OSM_Way *way, STYLE_ENTRY *style);
void drawNode(SDL_Renderer *ren, OSM_Node *node);
void drawRelation(SDL_Renderer *ren, OSM_Relation *rel);
void drawTexte(SDL_Renderer *ren, int x, int y, int w, int h, char *font, int size, char *texte, SDL_Color *color);
void drawOSM_ABR(ABR_Node *tree);
void _aapolygonRGBA(SDL_Renderer *renderer, const Sint16 *vx, const Sint16 *vy, int n, Uint8 r, Uint8 g, Uint8 b, Uint8 a);

// Mercator functions
double y2lat_m(double y);
double x2lon_m(double x);
double lat2y_m(double lat);
double lon2x_m(double lon);

// Gestion du rendu
void OSM_Rendering(SDL_Window *pWindow, int w, int h, OSM_Data *data);
void CreateWindow(int w, int h);
void CreateRenderer();
void OSM_DestroyRenderer();
 
#endif
