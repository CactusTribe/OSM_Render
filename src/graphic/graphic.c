#include <string.h>
#include <SDL2/SDL_ttf.h>
#include <math.h>
#include <stdio.h>

#include "graphic.h"
#include "../model/minHeap.h"

SDL_Renderer *ren = NULL;
OSM_Bounds *bounds = NULL;
ABR_Node *abr_node = NULL;
ABR_Node *abr_way = NULL;

STYLE_ENTRY _dico[DICO_SIZE] = {};
int NB_STYLES = 0;

int SCREEN_W = 0;
int SCREEN_H = 0;
double interval_Y = 0;
double interval_X = 0;

void draw_openedWay(SDL_Renderer *ren, OSM_Way *way);
void draw_closedWay(SDL_Renderer *ren, OSM_Way *way);


// Retourne le style en fonction de la clé
STYLE_ENTRY* getStyleOf(char *key, char *value){
	for(int i=0; i<NB_STYLES; i++){
		if(strcmp(key, _dico[i].key) == 0 && strcmp(value, _dico[i].value) == 0){
			return &_dico[i];
		}
	}
	return &_dico[0];
}


void openStyleSheet(char *file){
  FILE* f = fopen(file, "r");

  if(f != NULL){
    char buff[1024];

    while(fgets(buff, sizeof(buff), f) != NULL){
      NB_STYLES++;
    }
    rewind(f);

    int i = 0;
    char* argv[64];

    while(fgets(buff, sizeof(buff), f) != NULL){
      tokenize_command(buff, argv);

      char *key = malloc(64 * sizeof(char));
      char *value = malloc(64 * sizeof(char));

      strcpy(key, argv[0]);
      strcpy(value, argv[1]);

      _dico[i].key = key;
      _dico[i].value = value;
      _dico[i].weigth_IN = atoi(argv[2]);
      _dico[i].weigth_OUT = atoi(argv[3]);

      RGBA_COLOR color_IN = {atoi(argv[4]), atoi(argv[5]), atoi(argv[6]), atoi(argv[7])};
      RGBA_COLOR color_OUT = {atoi(argv[8]), atoi(argv[9]), atoi(argv[10]), atoi(argv[11])};

      _dico[i].color_IN = color_IN;
      _dico[i].color_OUT = color_OUT;

      i++;
    }
  }
  else{
    fprintf(stderr, "%s\n", "Erreur lors de l'ouverture du fichier.");
  }
}


int tokenize_command(char* argl, char** argv) {
  int i;
  argv[0] = strtok(argl, ARGSEP);
  for (i = 0; argv[i] != NULL; ++i)
      argv[i+1] = strtok(NULL, ARGSEP);
  return i;
}

void freeDico(STYLE_ENTRY *dico){
  for(int i=0; i<DICO_SIZE; i++){
    free(_dico[i].key);
    free(_dico[i].value);
  }
}


void CreateRenderer(SDL_Window *pWindow){
    ren = SDL_CreateRenderer(pWindow, 0, 0);
    SDL_SetRenderDrawColor(ren, 255, 255, 255, 255);
}


/* Choisie la fonction d'affichage appropriée en fonction de la clé */ 
void drawWay(SDL_Renderer *ren, OSM_Way *way){
	
	if(way->nb_tag > 0)
	{
		char *key = way->tags[0].k;

		if(strcmp(key, "highway") == 0 || strcmp(key, "railway") == 0 || strcmp(key, "waterway") == 0){
			draw_openedWay(ren, way);
		  //printf("%s\n", " -> draw_openedWay");
		}
		else if(strcmp(key, "building") == 0 || strcmp(key, "natural") == 0 || strcmp(key, "landuse") == 0 || strcmp(key, "leisure") == 0){
			draw_closedWay(ren, way);
		  //printf("%s\n", " -> draw_closedWay");
		}
	}
}


/* Affichage d'une way ouverte */
void draw_openedWay(SDL_Renderer *ren, OSM_Way *way){

  char *key = "default";
  char *value = "default";

  if(way->nb_tag > 0){
    key = way->tags[0].k;
    value = way->tags[0].v;
  }

	STYLE_ENTRY *style = getStyleOf(key, value);

  int weigth_IN = style->weigth_IN;
  int weigth_OUT = style->weigth_OUT;
	RGBA_COLOR *rgb_IN = &style->color_IN;
	RGBA_COLOR *rgb_OUT = &style->color_OUT;

  double latitude = 0;
  double longitude = 0;
  int x,y, x_suiv, y_suiv;

	// Draw shape
  for(int i=0; i < (way->nb_node)-1 ; i++){

    latitude = way->nodes[i]->lat;
    longitude = way->nodes[i]->lon;

    x = ((longitude - bounds->minlon) / interval_X) * SCREEN_W;
    y = ((latitude - bounds->minlat) / interval_Y) * SCREEN_H; 

/*
  	if(i > 0 && i < (way->nb_node)-1){
	  	filledCircleRGBA(ren, x, y, (weigth_IN/2),
	  		rgb_OUT->r, rgb_OUT->g, rgb_OUT->b, rgb_OUT->a);
	  }
*/
    latitude = way->nodes[i+1]->lat;
    longitude = way->nodes[i+1]->lon;

    x_suiv = ((longitude - bounds->minlon) / interval_X) * SCREEN_W;
    y_suiv = ((latitude - bounds->minlat) / interval_Y) * SCREEN_H;  

  	thickLineRGBA(ren, x, SCREEN_H - y, 
 			x_suiv, SCREEN_H - y_suiv, weigth_IN, rgb_OUT->r, rgb_OUT->g, rgb_OUT->b, rgb_OUT->a);
  }


  // Draw inner shape
  for(int i=0; i < (way->nb_node)-1 ; i++){

    latitude = way->nodes[i]->lat;
    longitude = way->nodes[i]->lon;

    x = ((longitude - bounds->minlon) / interval_X) * SCREEN_W;
    y = ((latitude - bounds->minlat) / interval_Y) * SCREEN_H; 

    latitude = way->nodes[i+1]->lat;
    longitude = way->nodes[i+1]->lon;

    x_suiv = ((longitude - bounds->minlon) / interval_X) * SCREEN_W;
    y_suiv = ((latitude - bounds->minlat) / interval_Y) * SCREEN_H;  

    thickLineRGBA(ren, x, SCREEN_H - y, 
      x_suiv, SCREEN_H - y_suiv, (weigth_IN - weigth_OUT) , rgb_IN->r, rgb_IN->g, rgb_IN->b, rgb_IN->a);
  }
}


/* Affichage d'une way fermée */
void draw_closedWay(SDL_Renderer *ren, OSM_Way *way){

  char *key = "default";
  char *value = "default";

  if(way->nb_tag > 0){
    key = way->tags[0].k;
    value = way->tags[0].v;
  }

	STYLE_ENTRY *style = getStyleOf(key, value);
	RGBA_COLOR *rgb_IN = &style->color_IN;
  RGBA_COLOR *rgb_OUT = &style->color_OUT;

	int nb_nodes = way->nb_node-1;
	short vx[nb_nodes];
	short vy[nb_nodes];

	for(int i=0; i < nb_nodes; i++){
    if(way->nodes[i]->id != 0){

      OSM_Node *nd = searchNode(abr_node, way->nodes[i]->id);

      if(nd != 0){
        vx[i] = ((nd->lon - bounds->minlon) / interval_X) * SCREEN_W;
        vy[i] = SCREEN_H - (((nd->lat - bounds->minlat) / interval_Y) * SCREEN_H);  
      }
    }
	}

  filledPolygonRGBA(ren, vx, vy, nb_nodes, rgb_IN->r, rgb_IN->g, rgb_IN->b, rgb_IN->a);

  for(int i=0; i < nb_nodes-1; i++){
    thickLineRGBA(ren, vx[i], vy[i], 
      vx[i+1], vy[i+1], 1, rgb_OUT->r, rgb_OUT->g, rgb_OUT->b, rgb_OUT->a);
  }
}


void drawNode(SDL_Renderer *ren, OSM_Node *node){
	double x,y;

	x = ((node->lon - bounds->minlon) / interval_X) * SCREEN_W;
	y = ((node->lat - bounds->minlat) / interval_Y) * SCREEN_H;

	filledCircleRGBA(ren, x, SCREEN_H - y, 2, 50, 50, 50, 255);
}

void drawTexte(SDL_Renderer *ren, int x, int y, int w, int h, char *font, int size, char *texte, SDL_Color *color){
    TTF_Font* font_ttf = TTF_OpenFont(font, size);

    SDL_Surface* surfaceMessage = TTF_RenderText_Blended(font_ttf, texte, *color);
    SDL_Texture* Message = SDL_CreateTextureFromSurface(ren, surfaceMessage); 
    SDL_Rect Message_rect;
    Message_rect.x = x; 
    Message_rect.y = y;
    Message_rect.w = w; 
    Message_rect.h = h; 
    SDL_RenderCopy(ren, Message, NULL, &Message_rect);

    TTF_CloseFont(font_ttf);
}


void drawOSM_ABR(ABR_Node *tree){
	if(!tree) return;
  if(tree->left)  drawOSM_ABR(tree->left);
  drawNode(ren, (OSM_Node *)&tree->nd);
  if(tree->right) drawOSM_ABR(tree->right);
}


void OSM_Rendering(SDL_Window *pWindow, int w, int h, OSM_Data *data){

  abr_node = data->abr_node;
  abr_way = data->abr_way;
	bounds = data->bounds;
  interval_Y = bounds->maxlat - bounds->minlat;
  interval_X = bounds->maxlon - bounds->minlon;
	SCREEN_W = w;
	SCREEN_H = h;

  // Ouverture du fichier de styles
  openStyleSheet("styles.txt");

	/* Création du renderer */
  CreateRenderer(pWindow);
	SDL_RenderClear(ren); // Clear la fenêtre

  // Création du tas de priorités min
  minHeap priority_heap = initMinHeap(data->nb_way);
  CreateHeapPriority(&priority_heap, data);

  // Affichage des ways en fonction de leur priorité
  for(int i=0; i<data->nb_way; i++){
    OSM_Way* way = getHead(&priority_heap);
    drawWay(ren, way);
    deleteNode(&priority_heap);
  }

  // Affichage texte ------------------------------
  //SDL_Color black = {0, 0, 0}; 
  //drawTexte(ren, 200, 200, 100, 50, "fonts/times.ttf", 80, "texte", &black);
  //  ----------------------------------------------*/

  deleteMinHeap(&priority_heap);
  freeDico(_dico);
  SDL_RenderPresent(ren); // Affiche les modifications
}

void CreateHeapPriority(minHeap *hp, OSM_Data* data){
  int priorite = 0;
  char *tag = NULL;
  char *value = NULL;

  for(int i=0; i< data->nb_way; i++){
    priorite = 99;
    tag = data->ways[i].tags[0].k;
    value = data->ways[i].tags[0].v;

    if(tag != NULL && value != NULL){
      //printf("[%s:%s]\n", tag, value);

      if(strcmp(tag, "landuse") == 0){
        if(strcmp(value, "grass") == 0) priorite = 0;
        if(strcmp(value, "forest") == 0) priorite = 1;
      }

      if(strcmp(tag, "leisure") == 0) priorite = 2;

      if(strcmp(tag, "waterway") == 0){
        if(strcmp(value, "canal") == 0) priorite = 3;
        if(strcmp(value, "river") == 0) priorite = 4;
      }

      if(strcmp(tag, "building") == 0) priorite = 5;

      if(strcmp(tag, "railway") == 0) priorite = 6;

      if(strcmp(tag, "highway") == 0){
        if(strcmp(value, "pedestrian") == 0) priorite = 7;
        if(strcmp(value, "living_street") == 0) priorite = 8;
        if(strcmp(value, "service") == 0) priorite = 9;
        if(strcmp(value, "residential") == 0) priorite = 10;
        if(strcmp(value, "unclassified") == 0) priorite = 11;
        if(strcmp(value, "tertiary") == 0) priorite = 12;
        if(strcmp(value, "secondary") == 0) priorite = 13;
        if(strcmp(value, "primary") == 0) priorite = 14;
      }
    }

    insertNode(hp, priorite, &data->ways[i]);
  }
}

void OSM_DestroyRenderer(){
	SDL_DestroyRenderer(ren);
}
