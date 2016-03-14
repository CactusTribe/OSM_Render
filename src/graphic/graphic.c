#include "graphic.h"
#include <string.h>

// Dictionnaire contenant les styles
STYLE_ENTRY _styles[NB_STYLES] = {
	{"default", 15, {190,190,190,255}, {100,100,100,255}},
	{"primary", 25, {254,216,155,255}, {176,124,0,255}},
	{"secondary", 25, {243,254,181,255}, {117,144,0,255}},
	{"tertiary", 25, {255,255,255,255}, {174,174,174,255}},
	{"unclassified", 15, {255,255,255,255}, {174,174,174,255}},
	{"residential", 15, {255,255,255,255}, {174,174,174,255}},
	{"service", 8, {255,255,255,255}, {174,174,174,255}},
	{"living_street", 15, {237,238,237,255}, {197,197,197,255}},
	{"pedestrian", 15, {223,220,234,255}, {168,166,167,255}},
	{"tram", 9, {69,69,69,255}, {69,69,69,255}}
};

// Retourne le style en fonction de la clé
STYLE_ENTRY* getStyleOf(char *key){
	for(int i=0; i<NB_STYLES; i++){
		if(strcmp(key, _styles[i].key) == 0){
			return &_styles[i];
		}
	}
	return &_styles[0];
}

void draw_highway(SDL_Renderer *ren, OSM_Way *way);
void draw_railway(SDL_Renderer *ren, OSM_Way *way);

/* Choisie la fonction d'affichage appropriée en fonction de la clé */ 
void drawWay(SDL_Renderer *ren, OSM_Way *way){
	char *key = way->tagList[0].k;

	if(strcmp(key, "highway") == 0){
		draw_highway(ren, way);
	}
	else if(strcmp(key, "railway") == 0){
		draw_railway(ren, way);
	}
}

/* Affichage d'une way de type HIGHWAY */
void draw_highway(SDL_Renderer *ren, OSM_Way *way){
	char *value = way->tagList[0].v;
	STYLE_ENTRY *style = getStyleOf(value);


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

/* Affichage d'une way de type RAILWAY */
void draw_railway(SDL_Renderer *ren, OSM_Way *way){
	char *value = way->tagList[0].v;
	STYLE_ENTRY *style = getStyleOf(value);


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


void OSM_Rendering(SDL_Renderer *ren){

  // WAY
  OSM_Node node1 = {0001, 30.0, 30.0, 1, 0};
  OSM_Node node2 = {0001, 100.0, 100.0, 1, 0};
  OSM_Node node3 = {0001, 350.0, 120.0, 1, 0};
  OSM_Node node4 = {0001, 360.0, 190.0, 1, 0};

  OSM_Node *nodes_liste = malloc(4 * sizeof(OSM_Node));
  nodes_liste[0] = node1;
  nodes_liste[1] = node2;
  nodes_liste[2] = node3;
  nodes_liste[3] = node4;

  OSM_Tag t1 = {"highway", "primary"};
  //OSM_Tag t1 = {"highway", "secondary"};
  //OSM_Tag t1 = {"highway", "tertiary"};
  //OSM_Tag t1 = {"highway", "residential"};
  //OSM_Tag t1 = {"highway", "unclassified"};
  //OSM_Tag t1 = {"highway", "service"};
  //OSM_Tag t1 = {"highway", "living_street"};
  //OSM_Tag t1 = {"highway", "pedestrian"};
  //OSM_Tag t1 = {"railway", "tram"};

  OSM_Tag *tags_liste = malloc(1 * sizeof(OSM_Tag));
  tags_liste[0] = t1;

  OSM_Way myWAY = {000002, 1, 4, 1, nodes_liste, tags_liste};

  drawWay(ren, &myWAY);

  free(nodes_liste);
  free(tags_liste);

  //short vx[6] = {50, 100, 150, 100, 50 , 0};
  //short vy[6] = {50, 50, 100, 150, 150, 100};
  //filledPolygonColor(ren, vx, vy, 6, 0xFF0000FF);

}
