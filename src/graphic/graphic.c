#include "graphic.h"
#include <string.h>
#include <SDL2/SDL_ttf.h>

// Dictionnaire contenant les styles
STYLE_ENTRY _styles[NB_STYLES] = {
	{"default", "default", 15, {190,190,190,255}, {100,100,100,255}},
	{"highway", "primary", 25, {254,216,155,255}, {176,124,0,255}},
	{"highway", "secondary", 25, {243,254,181,255}, {117,144,0,255}},
	{"highway", "tertiary", 25, {255,255,255,255}, {174,174,174,255}},
	{"highway", "unclassified", 15, {255,255,255,255}, {174,174,174,255}},
	{"highway", "residential", 15, {255,255,255,255}, {174,174,174,255}},
	{"highway", "service", 8, {255,255,255,255}, {174,174,174,255}},
	{"highway", "living_street", 15, {237,238,237,255}, {197,197,197,255}},
	{"highway", "pedestrian", 15, {223,220,234,255}, {168,166,167,255}},
	{"railway", "tram", 9, {69,69,69,255}, {69,69,69,255}},
	{"waterway", "river", 15, {136,189,214,255}, {136,189,214,255}},
	{"waterway", "canal", 8, {136,189,214,255}, {136,189,214,255}},
	{"natural", "water", 0, {136,189,214,255}, {136,189,214,255}},
	{"natural", "coastline", 0, {136,189,214,255}, {136,189,214,255}},
	{"building", "yes", 0, {190,190,190,255}, {190,190,190,255}},
	{"landuse", "grass", 0, {197,236,148,255}, {197,236,148,255}},
	{"landuse", "forest", 0, {157,202,138,255}, {157,202,138,255}},
	{"leisure", "park", 0, {205,247,201,255}, {205,247,201,255}}
};

// Retourne le style en fonction de la clé
STYLE_ENTRY* getStyleOf(char *key, char *value){
	for(int i=0; i<NB_STYLES; i++){
		if(strcmp(key, _styles[i].key) == 0 && strcmp(value, _styles[i].value) == 0){
			return &_styles[i];
		}
	}
	return &_styles[0];
}

void draw_openedWay(SDL_Renderer *ren, OSM_Way *way);
void draw_closedWay(SDL_Renderer *ren, OSM_Way *way);

/* Choisie la fonction d'affichage appropriée en fonction de la clé */ 
void drawWay(SDL_Renderer *ren, OSM_Way *way){
	char *key = way->tagList[0].k;

	if(strcmp(key, "highway") == 0 || strcmp(key, "railway") == 0 || strcmp(key, "waterway") == 0){
		draw_openedWay(ren, way);
	}
	else if(strcmp(key, "building") == 0 || strcmp(key, "natural") == 0 || strcmp(key, "landuse") == 0 || strcmp(key, "leisure") == 0){
		draw_closedWay(ren, way);
	}

}

/* Affichage d'une way ouverte */
void draw_openedWay(SDL_Renderer *ren, OSM_Way *way){

	char *key = way->tagList[0].k;
	char *value = way->tagList[0].v;

	STYLE_ENTRY *style = getStyleOf(key, value);

	int weigth = style->weigth;
	RGBA_COLOR *rgb_IN = &style->color_IN;
	RGBA_COLOR *rgb_OUT = &style->color_OUT;

	// Draw shape
  for(int i=0; i < (way->nb_nodes)-1 ; i++){
  	if(i > 0 && i < (way->nb_nodes)-1){
	  	filledCircleRGBA(ren, way->nodeList[i].lon, way->nodeList[i].lat, (weigth/2),
	  		rgb_OUT->r, rgb_OUT->g, rgb_OUT->b, rgb_OUT->a);
	  }

  	thickLineRGBA(ren, way->nodeList[i].lon, way->nodeList[i].lat, 
 			way->nodeList[i+1].lon, way->nodeList[i+1].lat, weigth, rgb_OUT->r, rgb_OUT->g, rgb_OUT->b, rgb_OUT->a);
  }

  // Draw inner shape
  for(int i=0; i < (way->nb_nodes)-1 ; i++){
  	if(i > 0 && i < (way->nb_nodes)-1){
	  	filledCircleRGBA(ren, way->nodeList[i].lon, way->nodeList[i].lat, ((weigth-3)/2),
	  		rgb_IN->r, rgb_IN->g, rgb_IN->b, rgb_IN->a);
	  }

  	thickLineRGBA(ren, way->nodeList[i].lon, way->nodeList[i].lat, 
 			way->nodeList[i+1].lon, way->nodeList[i+1].lat, weigth-3, rgb_IN->r, rgb_IN->g, rgb_IN->b, rgb_IN->a);
  }
}


/* Affichage d'une way fermée */
void draw_closedWay(SDL_Renderer *ren, OSM_Way *way){
	char *key = way->tagList[0].k;
	char *value = way->tagList[0].v;

	STYLE_ENTRY *style = getStyleOf(key, value);
	RGBA_COLOR *rgb_IN = &style->color_IN;

	int nb_nodes = way->nb_nodes-1;
	short vx[nb_nodes];
	short vy[nb_nodes];

	for(int i=0; i < nb_nodes; i++){
		vx[i] = way->nodeList[i].lon;
		vy[i] = way->nodeList[i].lat;
	}

  filledPolygonRGBA(ren, vx, vy, nb_nodes, rgb_IN->r, rgb_IN->g, rgb_IN->b, rgb_IN->a);
}

void drawNode(SDL_Renderer *ren, OSM_Node *node){
	filledCircleRGBA(ren, node->lon, node->lat, 4, 100, 100, 100, 255);
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

void OSM_Rendering(SDL_Renderer *ren){
	SDL_RenderClear(ren); // Clear la fenêtre

  // TEST AFFICHAGE WAY
  OSM_Node wn1 = {0, 30.0, 30.0, 1, 0};
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

  OSM_Way way1 = {000002, 1, 4, 1, wn_liste, wt_liste};

  drawWay(ren, &way1);

  free(wn_liste);
  free(wt_liste);
  // #################################################

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
  OSM_Node n1 = {0, 100.0, 200.0, 1, 0};
  drawNode(ren, &n1);

  // Affichage texte ------------------------------
  SDL_Color black = {0, 0, 0}; 
  drawTexte(ren, 200, 200, 100, 25, "fonts/times.ttf", 24, "texte", &black);
  //  ----------------------------------------------

  SDL_RenderPresent(ren); // Affiche les modifications
}
