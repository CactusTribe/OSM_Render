#include "graphic.h"
#include <string.h>
#include <SDL2/SDL_ttf.h>
#include <math.h>
#include <stdio.h>

SDL_Renderer *ren = NULL;
OSM_Bounds *bounds = NULL;
ABR_Node *abr_osm_node = NULL;

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
      _dico[i].weigth = atoi(argv[2]);

      RGBA_COLOR color_IN = {atoi(argv[3]), atoi(argv[4]), atoi(argv[5]), atoi(argv[6])};
      RGBA_COLOR color_OUT = {atoi(argv[7]), atoi(argv[8]), atoi(argv[9]), atoi(argv[10])};

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
	else
	{
			draw_closedWay(ren, way);
	}
}


/* Affichage d'une way ouverte */
void draw_openedWay(SDL_Renderer *ren, OSM_Way *way){

	char *key = way->tags[0].k;
	char *value = way->tags[0].v;

	STYLE_ENTRY *style = getStyleOf(key, value);

	int weigth = style->weigth;
	RGBA_COLOR *rgb_IN = &style->color_IN;
	RGBA_COLOR *rgb_OUT = &style->color_OUT;

  double latitude = 0;
  double longitude = 0;

	// Draw shape
  for(int i=0; i < (way->nb_node)-1 ; i++){

    latitude = way->nodes[i]->lat;
    longitude = way->nodes[i]->lon;

  	if(i > 0 && i < (way->nb_node)-1){
	  	filledCircleRGBA(ren, longitude, latitude, (weigth/2),
	  		rgb_OUT->r, rgb_OUT->g, rgb_OUT->b, rgb_OUT->a);
	  }

  	thickLineRGBA(ren, longitude, latitude, 
 			way->nodes[i+1]->lon, way->nodes[i+1]->lat, weigth, rgb_OUT->r, rgb_OUT->g, rgb_OUT->b, rgb_OUT->a);
  }

  // Draw inner shape
  for(int i=0; i < (way->nb_node)-1 ; i++){

    latitude = way->nodes[i]->lat;
    longitude = way->nodes[i]->lon;

  	if(i > 0 && i < (way->nb_node)-1){
	  	filledCircleRGBA(ren, longitude, latitude, ((weigth-3)/2),
	  		rgb_IN->r, rgb_IN->g, rgb_IN->b, rgb_IN->a);
	  }

  	thickLineRGBA(ren, longitude, latitude, 
 			way->nodes[i+1]->lon, way->nodes[i+1]->lat, weigth-3, rgb_IN->r, rgb_IN->g, rgb_IN->b, rgb_IN->a);
  }
}


/* Affichage d'une way fermée */
void draw_closedWay(SDL_Renderer *ren, OSM_Way *way){
	//char *key = way->tags[0].k;
	//char *value = way->tags[0].v;

	//STYLE_ENTRY *style = getStyleOf(key, value);
	//RGBA_COLOR *rgb_IN = &style->color_IN;

	int nb_nodes = way->nb_node-1;
	//short vx[nb_nodes];
	//short vy[nb_nodes];

	for(int i=0; i < nb_nodes; i++){
    if(way->nodes[i]->id != 0){
      OSM_Node *nd = searchNode(abr_osm_node, way->nodes[i]->id);
      if(nd != 0){
        //printf("%ld\n", nd->id);
        //printf("lon %f lat %f\n", nd->lon, nd->lat);

        //vx[i] = ((nd->lon - bounds->minlon) / interval_X) * SCREEN_W;
        //vy[i] = ((nd->lat - bounds->minlat) / interval_Y) * SCREEN_H;  
      }
    }

    //printf("lon(%f) lat(%f)\n", way->nodes[i].lon, way->nodes[i].lat);

    //printf("vx(%hd) vy(%hd)\n", vx[i], vy[i]);
	}

  //filledPolygonRGBA(ren, vx, vy, nb_nodes, rgb_IN->r, rgb_IN->g, rgb_IN->b, rgb_IN->a);
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
  //printf("id = %d lat = %f lon = %f\n", tree->id, tree->nd.lat, tree->nd.lon);
  if(tree->right) drawOSM_ABR(tree->right);
}


void OSM_Rendering(SDL_Window *pWindow, int w, int h, OSM_Data *data){

  abr_osm_node = data->abr_node;
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


	for(int i=0; i< data->nb_way; i++)
		drawWay(ren, &data->ways[i]);

	for(int i=0; i< data->nb_node; i++)
		drawNode(ren, &data->nodes[i]);

  /*
  for(int i= 0; i < data->nb_way; i++){
    if(data->ways[i].tags[0].k != NULL && data->ways[i].tags[0].v != NULL){
      //printf("[%s:%s]\n", data->ways[i].tags[0].k, data->ways[i].tags[0].v);
      drawWay(ren, &data->ways[i]);
    }
  }
*/
  // TEST AFFICHAGE WAY
 /* OSM_Node wn1 = {0, 30.0, 30.0, 1, 0};
  OSM_Node wn2 = {0, 100.0, 100.0, 1, 0};
  OSM_Node wn3 = {0, 350.0, 120.0, 1, 0};
  OSM_Node wn4 = {0, 360.0, 190.0, 1, 0};

  OSM_Node *wn_liste = malloc(4 * sizeof(OSM_Node));
  wn_liste[0] = wn1;
  wn_liste[1] = wn2;
  wn_liste[2] = wn3;
  wn_liste[3] = wn4;

  OSM_Tag wt1 = {"highway", "primary"};
  OSM_Tag wt2 = {"name", "East 22nd Avenue"};

  OSM_Tag *wt_liste = malloc(2 * sizeof(OSM_Tag));
  wt_liste[0] = wt1;
  wt_liste[1] = wt2;

  OSM_Way way1 = {000002, 1, 4, wn_liste, 1, wt_liste};

  drawWay(ren, &way1);

  free(wn_liste);
  free(wt_liste);*/
  // #################################################

/*
  // TEST AFFICHAGE AREA
  OSM_Node bn1 = {0, 150.0, 260.0, 1, 0};
  OSM_Node bn2 = {0, 200.0, 320.0, 1, 0};
  OSM_Node bn3 = {0, 320.0, 300.0, 1, 0};
  OSM_Node bn4 = {0, 270.0, 190.0, 1, 0};
  OSM_Node bn5 = {0, 150.0, 260.0, 1, 0};

  OSM_Node *bn_liste = malloc(5 * sizeof(OSM_Node));
  bn_liste[0] = bn1;
  bn_liste[1] = bn2;
  bn_liste[2] = bn3;
  bn_liste[3] = bn4;
  bn_liste[4] = bn5;

  OSM_Tag bt1 = {"landuse", "forest"};

  OSM_Tag *bt_liste = malloc(1 * sizeof(OSM_Tag));
  bt_liste[0] = bt1;

  OSM_Way building1 = {0, 1, 5, 1, bn_liste, bt_liste};

  drawWay(ren, &building1);

  free(bn_liste);
  free(bt_liste);
  // #################################################

  // TEST AFFICHAGE NODE
  OSM_Node n1 = {0, 39.749290, -104.973780, 1, 0};
  OSM_Node n2 = {0, 39.752461, -104.969381, 1, 0}; 

  //drawNode(ren, &n1);
  //drawNode(ren, &n2);

  // Affichage texte ------------------------------
  SDL_Color black = {0, 0, 0}; 
  //drawTexte(ren, 200, 200, 100, 50, "fonts/times.ttf", 80, "texte", &black);
  //  ----------------------------------------------*/

  freeDico(_dico);
  SDL_RenderPresent(ren); // Affiche les modifications
}


void OSM_DestroyRenderer(){
	SDL_DestroyRenderer(ren);
}
