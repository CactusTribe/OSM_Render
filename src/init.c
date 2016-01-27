#include "../header/prototypes.h"
 
SDL_Window *screen;
SDL_Renderer *renderer;
 
 
SDL_Renderer *getrenderer(void){
  return renderer;
}

void init(char *title){
  screen = SDL_CreateWindow(title,
                                SDL_WINDOWPOS_CENTERED,
                                SDL_WINDOWPOS_CENTERED,
                                SCREEN_WIDTH, SCREEN_HEIGHT,
                                SDL_WINDOW_SHOWN);

  renderer = SDL_CreateRenderer(screen, -1, SDL_RENDERER_PRESENTVSYNC);

  if (screen == NULL || renderer == NULL){
    printf("Impossible d'initialiser le mode écran à %d x %d: %s\n", SCREEN_WIDTH,
                                                                SCREEN_HEIGHT, SDL_GetError());
    exit(1);
  }

  // On initialise les images png
  int imgFlags = IMG_INIT_PNG;
  if( !( IMG_Init( imgFlags ) & imgFlags ) ){
    printf( "SDL_image n'a pu être initialisée! SDL_image Error: %s\n", IMG_GetError() );
    exit(1);
  }

  //On initialise SDL_TTF 2 (texte)
  if (TTF_Init() < 0){
    printf("Impossible d'initialiser SDL TTF: %s\n", TTF_GetError());
    exit(1);
  }
}



void cleanup(){
  SDL_DestroyRenderer(renderer);
  renderer = NULL;
  SDL_DestroyWindow(screen);
  screen = NULL;

  TTF_Quit();
  SDL_Quit();
}