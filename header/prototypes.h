#ifndef PROTOTYPES
#define PROTOTYPES
 
#include "structs.h"
 
extern void cleanup(void);
extern void delay(unsigned int frameLimit);
extern void draw(void);
extern SDL_Renderer *getrenderer(void);
extern void init(char *);
 
#endif