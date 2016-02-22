#include "graphic.h"

void drawRectangle(SDL_Renderer *ren, SDL_Rect *rect, RGBA_COLOR *rgb){
  SDL_SetRenderDrawBlendMode(ren, SDL_BLENDMODE_BLEND);
  SDL_SetRenderDrawColor(ren, rgb->r, rgb->g, rgb->b, rgb->a);
  SDL_RenderFillRect(ren, rect);
}

void drawPolygon(SDL_Renderer *ren, POLYGON *p, RGBA_COLOR *rgb) {
	SDL_SetRenderDrawBlendMode(ren, SDL_BLENDMODE_BLEND);
  SDL_SetRenderDrawColor(ren, rgb->r, rgb->g, rgb->b, rgb->a);
  SDL_RenderDrawLines(ren, p->points, p->nbPoints);
  SDL_RenderDrawLine(ren, p->points[0].x, p->points[0].y, p->points[p->nbPoints-1].x, p->points[p->nbPoints-1].y);
  fill_poly(ren, p, rgb);
}

void drawPoint(SDL_Renderer *ren, SDL_Point *p, RGBA_COLOR *rgb) {
  SDL_SetRenderDrawColor(ren, rgb->r, rgb->g, rgb->b, rgb->a);
  SDL_RenderDrawPoint(ren, p->x, p->y);
}

// Retourne si 1 si le point est dans le poly et 0 sinon
int pointInPolygon(SDL_Point *pt, POLYGON *p){
	int j = p->nbPoints-1;
	int oddNodes = 0;

	for(int i=0; i < p->nbPoints; i++){
		if((p->points[i].y < pt->y && p->points[j].y >= pt->y) || (p->points[j].y < pt->y && p->points[i].y >= pt->y)){
			if( p->points[i].x + (pt->y - p->points[i].y) / (p->points[j].y - p->points[i].y) * (p->points[j].x - p->points[i].x) < pt->x){
				oddNodes = (oddNodes+1)%2;
			}
		}
		j=i;
	}
	return oddNodes;
}

void fill_poly(SDL_Renderer *ren, POLYGON *p, RGBA_COLOR *rgb){
	int Y_TOP = p->points[0].y;
	int Y_BOT = p->points[0].y;
	int X_RIGHT = p->points[0].x;
	int X_LEFT = p->points[0].x;
	SDL_Point current;

	// On récupère les extremums
	for(int i=0; i < p->nbPoints; i++){
		current = p->points[i];

		if(current.y < Y_TOP) Y_TOP = current.y;
		if(current.y > Y_BOT) Y_BOT = current.y;
		if(current.x > X_RIGHT) X_RIGHT = current.x;
		if(current.x < X_LEFT) X_LEFT = current.x;
	}
	printf("Y[min;max] -> [%d;%d]\n", Y_TOP, Y_BOT);
	printf("X[min;max] -> [%d;%d]\n", X_LEFT, X_RIGHT);

	int pixelX, pixelY;
	int nodeX[p->nbPoints];
	// Parcours du poly de haut en bas
	for(pixelY = Y_TOP; pixelY < Y_BOT; pixelY++){
		
		// Création de la liste des points qui intersectent le poly
		int nodes = 0; 
		int j = p->nbPoints-1;

		for (int i=0; i < p->nbPoints; i++) {
			if ( (p->points[i].y < (double) pixelY && p->points[j].y >= (double) pixelY) ||  (p->points[j].y < (double) pixelY && p->points[i].y >= (double) pixelY) ) {
				nodeX[nodes++] = (int) (p->points[i].x + (pixelY - p->points[i].y) / (p->points[j].y - p->points[i].y) * (p->points[j].x - p->points[i].x)); 
			}
			j=i;
		} 

		printf("[");
		for(int i=0; i < nodes; i++){
			printf("%d,", nodeX[i]);
		}
		printf("]\n");
 
		// Tri du tableau nodeX
		int i = 0;
		int swap;
		while (i < nodes-1) {
			if (nodeX[i] > nodeX[i+1]){
				swap = nodeX[i]; 
				nodeX[i] = nodeX[i+1]; 
				nodeX[i+1] = swap; 
				if (i) i--; 
			}
			else
				i++; 
		}

		// On trace les lignes entre les noeuds paires
		for (int i=0; i < nodes; i+=2){
			if(nodeX[i] >= X_RIGHT) break;
			if(nodeX[i+1] > X_LEFT){
				if(nodeX[i] < X_LEFT) nodeX[i] = X_LEFT;
				if(nodeX[i+1] > X_RIGHT) nodeX[i+1] = X_RIGHT;
				for(pixelX = nodeX[i]; pixelX < nodeX[i+1]; pixelX++){
					SDL_Point pt = {pixelX, pixelY};
					drawPoint(ren, &pt, rgb);
				}
			}
		}
	}
}

/*
void fill_poly(SDL_Renderer *ren, POLYGON *p){
	SDL_Point topLeft = p->points[0], botRight = p->points[0];
	SDL_Point current;

	for(int i=0; i < p->nbPoints-1; i++){
		current = p->points[i];

		if(current.x < topLeft.x) topLeft.x = current.x;
		if(current.y < topLeft.y) topLeft.y = current.y;
		if(current.x > botRight.x) botRight.x = current.x;
		if(current.y > botRight.y) botRight.y = current.y;

		topLeft.y--;
		topLeft.x--;
		botRight.x++;
		botRight.y++;
	}

	for(int i=topLeft.y; i < botRight.y; i++){
		for(int i=topLeft.y; i < botRight.y; i++){

		}	
	}
}*/