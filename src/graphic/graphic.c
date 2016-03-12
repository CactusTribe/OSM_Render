#include "graphic.h"

void drawWay(SDL_Renderer *ren, OSM_Way *way, RGBA_COLOR *rgb, int weigth){
	SDL_SetRenderDrawBlendMode(ren, SDL_BLENDMODE_BLEND);
  SDL_SetRenderDrawColor(ren, rgb->r, rgb->g, rgb->b, rgb->a);

  for(int i=0; i < (way->nb_nodes)-1 ; i++){
  	filledCircleRGBA(ren, way->nodeList[i].lon, way->nodeList[i].lat, (weigth/2), rgb->r, rgb->g, rgb->b, rgb->a);

  	thickLineRGBA(ren, way->nodeList[i].lon, way->nodeList[i].lat, 
 			way->nodeList[i+1].lon, way->nodeList[i+1].lat, weigth, rgb->r, rgb->g, rgb->b, rgb->a);
  }
	
}
