#include <string.h>
#include <SDL2/SDL_ttf.h>
#include <math.h>
#include <stdio.h>

#include "graphic.h"
#include "../model/minHeap.h"

SDL_Renderer *ren = NULL;
OSM_Bounds *bounds = NULL;

STYLE_ENTRY _dico[DICO_SIZE] = {};
int NB_STYLES = 0;

int SCREEN_W = 0;
int SCREEN_H = 0;
double SCALE = 1;

// Retourne le style en fonction de la clé
STYLE_ENTRY* getStyleOf(char *key, char *value){
	for(int i=0; i<NB_STYLES; i++){
		if(strcmp(key, _dico[i].key) == 0 && strcmp(value, _dico[i].value) == 0){
			return &_dico[i];
		}
	}
	return NULL;
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
    int priority = 0;

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

      RGBA_COLOR color_IN = {atoi(argv[4]), atoi(argv[5]), atoi(argv[6]), 255};
      RGBA_COLOR color_OUT = {atoi(argv[7]), atoi(argv[8]), atoi(argv[9]), 255};

      _dico[i].color_IN = color_IN;
      _dico[i].color_OUT = color_OUT;

      _dico[i].priority = priority;

      priority++;
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

void OSM_DestroyRenderer(){
  SDL_DestroyRenderer(ren);
}


/* Choisie la fonction d'affichage appropriée en fonction de la clé */ 
void drawWay(SDL_Renderer *ren, OSM_Way *way){
  char *key = "";
  char *value = "";
  STYLE_ENTRY *style = NULL;

  if(way->nb_tag > 0){
    for(int i=0; i < way->nb_tag; i++){
      key = way->tags[i].k;
      value = way->tags[i].v;
      style = getStyleOf(key, value);
      if(style != NULL) break;
    }
  }
	
  // Si le style existe alors on affiche
  if(style != NULL){
    if(strcmp(key, "highway") == 0 || strcmp(key, "railway") == 0 || strcmp(key, "waterway") == 0){
      draw_openedWay(ren, way, style);
    }
    else if(strcmp(key, "building") == 0 || strcmp(key, "natural") == 0 || strcmp(key, "landuse") == 0 || strcmp(key, "leisure") == 0){
      draw_closedWay(ren, way, style);
    }
    else if(strcmp(key, "amenity") == 0){
      if(strcmp(value, "parking") == 0) 
        draw_closedWay(ren, way, style);
    }   
  }
}


/* Affichage d'une way ouverte */
void draw_openedWay(SDL_Renderer *ren, OSM_Way *way, STYLE_ENTRY *style){
  if(style != NULL){

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

      x = (lon2x_m(longitude) - lon2x_m(bounds->minlon)) * SCALE;
      y = (lat2y_m(latitude) - lat2y_m(bounds->minlat)) * SCALE; 

    	if(i > 0 && i < (way->nb_node)-1){
  	  	filledCircleRGBA(ren, x, SCREEN_H - y, (weigth_IN/2),
  	  		rgb_IN->r, rgb_IN->g, rgb_IN->b, rgb_IN->a);
  	  }

      latitude = way->nodes[i+1]->lat;
      longitude = way->nodes[i+1]->lon;

      x_suiv = (lon2x_m(longitude) - lon2x_m(bounds->minlon)) * SCALE;
      y_suiv = (lat2y_m(latitude) - lat2y_m(bounds->minlat)) * SCALE; 

    	thickLineRGBA(ren, x, SCREEN_H - y, 
   			x_suiv, SCREEN_H - y_suiv, weigth_IN, rgb_IN->r, rgb_IN->g, rgb_IN->b, rgb_IN->a);
    }
  }
}


/* Affichage d'une way fermée */
void draw_closedWay(SDL_Renderer *ren, OSM_Way *way, STYLE_ENTRY *style){

  if(style != NULL){
  	RGBA_COLOR *rgb_IN = &style->color_IN;
    RGBA_COLOR *rgb_OUT = &style->color_OUT;

  	int nb_nodes = (way->nb_node)-1;
  	short vx[nb_nodes];
  	short vy[nb_nodes];

  	for(int i=0; i < nb_nodes; i++){

      OSM_Node *nd = way->nodes[i];

      vx[i] = (lon2x_m(nd->lon) - lon2x_m(bounds->minlon)) * SCALE;
      vy[i] = SCREEN_H - ((lat2y_m(nd->lat) - lat2y_m(bounds->minlat)) * SCALE);  
      
  	}

    filledPolygonRGBA(ren, vx, vy, nb_nodes, rgb_IN->r, rgb_IN->g, rgb_IN->b, rgb_IN->a);
    _aapolygonRGBA(ren, vx, vy, nb_nodes, rgb_OUT->r, rgb_OUT->g, rgb_OUT->b, rgb_OUT->a);
  }
}


void drawNode(SDL_Renderer *ren, OSM_Node *node){
	double x,y;

	x = ((node->lon - bounds->minlon) / INTER_X) * SCREEN_W;
	y = ((node->lat - bounds->minlat) / INTER_Y) * SCREEN_H;

	filledCircleRGBA(ren, x, SCREEN_H - y, 2, 50, 50, 50, 255);
}

void drawRelation(SDL_Renderer *ren, OSM_Relation *rel){

  char *key = "";
  char *value = "";
  STYLE_ENTRY *style = NULL;
  int multipoly = 0;

  if(rel->nb_tag > 0){
    for(int i=0; i < rel->nb_tag; i++){
      key = rel->tags[i].k;
      value = rel->tags[i].v;

      if(style == NULL)
        style = getStyleOf(key, value);

      if(strcmp(key, "type") == 0 && strcmp(value, "multipolygon") == 0) 
        multipoly = 1;
    }
  }

  if(multipoly == 1){
    //printf("multipoly id = %lu\n", rel->id);
    if(style != NULL) printf("  [%s:%s]\n", style->key, style->value);

    // Affichage membres OUTER
    for(int i=0; i < rel->nb_member; i++){
      if(rel->members[i].ref != NULL){
        OSM_Way *way = rel->members[i].ref;

        if(strcmp(rel->members[i].role, "outer") == 0){
          draw_closedWay(ren, way, style);
        }
      }
    }  

    // Affichage membres INNER
    for(int i=0; i < rel->nb_member; i++){
      if(rel->members[i].ref != NULL){
        OSM_Way *way = rel->members[i].ref;

        if(strcmp(rel->members[i].role, "inner") == 0){
          drawWay(ren, way);
        }
      }
    }  
  }

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

	bounds = data->bounds;
	SCREEN_W = w;
	SCREEN_H = h;

  //##################################

  printf("LAT : [%f:%f]\n", bounds->minlat, bounds->maxlat);
  printf("LON : [%f:%f]\n", bounds->minlon, bounds->maxlon); 

  printf("Y : [%f:%f]\n", lat2y_m(bounds->minlat), lat2y_m(bounds->maxlat));
  printf("X : [%f:%f]\n", lon2x_m(bounds->minlon), lon2x_m(bounds->maxlon));

  // Calcul du ratio permetant une couverture complète de la fenêtre
  double Inter_X = lon2x_m(bounds->maxlon) - lon2x_m(bounds->minlon);
  double Inter_Y = lat2y_m(bounds->maxlat) - lat2y_m(bounds->minlat);

  double ratio_X = ceil(Inter_X / SCREEN_W);
  double ratio_Y = ceil(Inter_Y / SCREEN_H);

  if(ratio_X > ratio_Y) SCALE = ratio_X;
  else SCALE = ratio_Y;

  printf("SCALE = %f\n", SCALE);

  //##################################

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

/*
  // Affichage des relations
  for(int i=0; i < data->nb_relation; i++)
    drawRelation(ren, &data->relations[i]);
*/
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
  char *tag = "";
  char *value = "";

  for(int i=0; i< data->nb_way; i++){
    priorite = 99;

    if(data->ways[i].nb_tag > 0){
      for(int j=0; j < data->ways[i].nb_tag; j++){

        tag = data->ways[i].tags[j].k;
        value = data->ways[i].tags[j].v;
        STYLE_ENTRY *style = getStyleOf(tag, value);

        if(style != NULL){
          priorite = style->priority;
          break;
        }
      }
    }
    insertNode(hp, priorite, &data->ways[i]);
  }
}


double y2lat_m(double y) { return rad2deg(2 * atan(exp( (y / earth_radius ) )) - M_PI/2); }
double x2lon_m(double x) { return rad2deg(x / earth_radius); }
double lat2y_m(double lat) { return earth_radius * log(tan(M_PI/4+ deg2rad(lat)/2)); }
double lon2x_m(double lon) { return deg2rad(lon) * earth_radius; }

void _aapolygonRGBA(SDL_Renderer *renderer, const Sint16 *vx, const Sint16 *vy, int n, Uint8 r, Uint8 g, Uint8 b, Uint8 a){
  for(int i=0; i < n-1; i++){
    aalineRGBA(renderer, vx[i], vy[i], vx[i+1], vy[i+1], r, g, b, a);
  }
  aalineRGBA(renderer, vx[n-1], vy[n-1], vx[0], vy[0], r, g, b, a);
}
