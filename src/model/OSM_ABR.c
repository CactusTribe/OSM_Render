#include <stdio.h>
#include <stdlib.h>
#include "OSM_ABR.h"

void addNode(ABR_Node **tree, OSM_Node nd){
  ABR_Node *tmpNode;
	ABR_Node *tmpTree = *tree;

	ABR_Node *elem = malloc(sizeof(ABR_Node));
	elem->id = nd.id;
  elem->nd = nd;
	elem->left = NULL;
	elem->right = NULL;

	if(tmpTree)
		do{
		  tmpNode = tmpTree;
		  if(nd.id > tmpTree->id ){
	      tmpTree = tmpTree->right;
	      if(!tmpTree) tmpNode->right = elem;
		  }
		  else{
	      tmpTree = tmpTree->left;
	      if(!tmpTree) tmpNode->left = elem;
		  }
		} while(tmpTree);
	else  *tree = elem;
}

OSM_Node* searchNode(ABR_Node *tree, int id){
  while(tree){
    if(id == tree->id) return &tree->nd;
    if(id > tree->id ) tree = tree->right;
    else tree = tree->left;
  }
  return 0;
}

void printTree(ABR_Node *tree){
  if(!tree) return;
  if(tree->left)  printTree(tree->left);
  printf("Cle = %d\n", tree->id);
  if(tree->right) printTree(tree->right);
}


void printReverseTree(ABR_Node *tree){
  if(!tree) return;
  if(tree->right) printReverseTree(tree->right);
  printf("Cle = %d\n", tree->id);
  if(tree->left)  printReverseTree(tree->left);
}


void clearTree(ABR_Node **tree){
  ABR_Node *tmpTree = *tree;

  if(!tree) return;
  if(tmpTree->left)  clearTree(&tmpTree->left);
  if(tmpTree->right) clearTree(&tmpTree->right);

  free(tmpTree);
  *tree = NULL;
}