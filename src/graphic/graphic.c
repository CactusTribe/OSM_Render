#include "graphic.h"
#include <string.h>

void drawWay(SDL_Renderer *ren, OSM_Way *way){

	int weigth;
	RGBA_COLOR rgb_IN;
	RGBA_COLOR rgb_OUT;

	// HIGHWAY COLORS
	RGBA_COLOR primary_IN = {254,216,155,255};
	RGBA_COLOR primary_OUT = {176,124,0,255};
	RGBA_COLOR secondary_IN = {243,254,181,255};
	RGBA_COLOR secondary_OUT = {117,144,0,255};
	RGBA_COLOR tertiary_IN = {255,255,255,255};
	RGBA_COLOR tertiary_OUT = {174,174,174,255};
	RGBA_COLOR unclassified_IN = {255,255,255,255};
	RGBA_COLOR unclassified_OUT = {174,174,174,255};
	RGBA_COLOR residential_IN = {255,255,255,255};
	RGBA_COLOR residential_OUT = {174,174,174,255};
	RGBA_COLOR service_IN = {255,255,255,255};
	RGBA_COLOR service_OUT = {174,174,174,255};
	RGBA_COLOR living_street_IN = {237,238,237,255};
	RGBA_COLOR living_street_OUT = {197,197,197,255};
	RGBA_COLOR pedestrian_IN = {223,220,234,255};
	RGBA_COLOR pedestrian_OUT = {168,166,167,255};

	// RAILWAY COLORS
	RGBA_COLOR tram_IN = {69,69,69,255};
	RGBA_COLOR tram_OUT = {69,69,69,255};

	char *key = way->tagList[0].k;
	char *value = way->tagList[0].v;

	if(strcmp(key, "highway") == 0){
		if(strcmp(value, "primary") == 0 || strcmp(value, "secondary") == 0 || strcmp(value, "tertiary") == 0
			|| strcmp(value, "unclassified") == 0 || strcmp(value, "residential") == 0 || strcmp(value, "service") == 0
			|| strcmp(value, "living_street") == 0 || strcmp(value, "pedestrian") == 0){

			
			if(strcmp(value, "primary") == 0){
				weigth = 25;
				rgb_IN = primary_IN;
				rgb_OUT = primary_OUT;
			}
			else if(strcmp(value, "secondary") == 0){
				weigth = 25;
				rgb_IN = secondary_IN;
				rgb_OUT = secondary_OUT;
			}
			else if(strcmp(value, "tertiary") == 0){
				weigth = 25;
				rgb_IN = tertiary_IN;
				rgb_OUT = tertiary_OUT;
			}
			else if(strcmp(value, "unclassified") == 0){
				weigth = 15;
				rgb_IN = unclassified_IN;
				rgb_OUT = unclassified_OUT;
			}
			else if(strcmp(value, "residential") == 0){
				weigth = 15;
				rgb_IN = residential_IN;
				rgb_OUT = residential_OUT;
			}
			else if(strcmp(value, "service") == 0){
				weigth = 8;
				rgb_IN = service_IN;
				rgb_OUT = service_OUT;
			}
			else if(strcmp(value, "living_street") == 0){
				weigth = 15;
				rgb_IN = living_street_IN;
				rgb_OUT = living_street_OUT;
			}
			else if(strcmp(value, "pedestrian") == 0){
				weigth = 15;
				rgb_IN = pedestrian_IN;
				rgb_OUT = pedestrian_OUT;
			}

			// Draw shape
		  for(int i=0; i < (way->nb_nodes)-1 ; i++){
		  	if(i > 0 && i < (way->nb_nodes)-1){
			  	filledCircleRGBA(ren, way->nodeList[i].lon, way->nodeList[i].lat, (weigth/2),
			  		rgb_OUT.r, rgb_OUT.g, rgb_OUT.b, rgb_OUT.a);
			  }

		  	thickLineRGBA(ren, way->nodeList[i].lon, way->nodeList[i].lat, 
		 			way->nodeList[i+1].lon, way->nodeList[i+1].lat, weigth, rgb_OUT.r, rgb_OUT.g, rgb_OUT.b, rgb_OUT.a);
		  }

		  // Draw inner shape
		  for(int i=0; i < (way->nb_nodes)-1 ; i++){
		  	if(i > 0 && i < (way->nb_nodes)-1){
			  	filledCircleRGBA(ren, way->nodeList[i].lon, way->nodeList[i].lat, ((weigth-3)/2),
			  		rgb_IN.r, rgb_IN.g, rgb_IN.b, rgb_IN.a);
			  }

		  	thickLineRGBA(ren, way->nodeList[i].lon, way->nodeList[i].lat, 
		 			way->nodeList[i+1].lon, way->nodeList[i+1].lat, weigth-3, rgb_IN.r, rgb_IN.g, rgb_IN.b, rgb_IN.a);
		  }
		}
	}
	else if(strcmp(key, "railway") == 0){
		if(strcmp(value, "tram") == 0){
			weigth = 9;
			rgb_IN = tram_IN;
			rgb_OUT = tram_OUT;
		}

		// Draw shape
	  for(int i=0; i < (way->nb_nodes)-1 ; i++){
	  	if(i > 0 && i < (way->nb_nodes)-1){
		  	filledCircleRGBA(ren, way->nodeList[i].lon, way->nodeList[i].lat, (weigth/2),
		  		rgb_OUT.r, rgb_OUT.g, rgb_OUT.b, rgb_OUT.a);
		  }

	  	thickLineRGBA(ren, way->nodeList[i].lon, way->nodeList[i].lat, 
	 			way->nodeList[i+1].lon, way->nodeList[i+1].lat, weigth, rgb_OUT.r, rgb_OUT.g, rgb_OUT.b, rgb_OUT.a);
	  }

	  // Draw inner shape
	  for(int i=0; i < (way->nb_nodes)-1 ; i++){
	  	if(i > 0 && i < (way->nb_nodes)-1){
		  	filledCircleRGBA(ren, way->nodeList[i].lon, way->nodeList[i].lat, ((weigth-3)/2),
		  		rgb_IN.r, rgb_IN.g, rgb_IN.b, rgb_IN.a);
		  }

	  	thickLineRGBA(ren, way->nodeList[i].lon, way->nodeList[i].lat, 
	 			way->nodeList[i+1].lon, way->nodeList[i+1].lat, weigth-3, rgb_IN.r, rgb_IN.g, rgb_IN.b, rgb_IN.a);
	  }
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

  OSM_Way primary = {000002, 1, 4, 1, nodes_liste, tags_liste};

  drawWay(ren, &primary);

  free(nodes_liste);
  free(tags_liste);

  //short vx[6] = {50, 100, 150, 100, 50 , 0};
  //short vy[6] = {50, 50, 100, 150, 150, 100};
  //filledPolygonColor(ren, vx, vy, 6, 0xFF0000FF);

}
