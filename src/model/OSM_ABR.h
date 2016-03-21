#ifndef _OSM_ABR_H_
#define _OSM_ABR_H_

#include "../model/osm_types.h"
#include "../graphic/graphic.h"

typedef struct ABR_Node{
	int id;
	OSM_Node nd;
	struct ABR_Node *left;
	struct ABR_Node *right;
} ABR_Node;

void addNode(ABR_Node **tree, OSM_Node nd);
int searchNode(ABR_Node *tree, int id);
void printTree(ABR_Node *tree);
void printReverseTree(ABR_Node *tree);
void clearTree(ABR_Node **tree);


#endif