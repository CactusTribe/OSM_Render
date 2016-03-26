#ifndef _OSM_TYPES_H_ 
#define _OSM_TYPES_H_

#include "OSM_ABR.h"

typedef struct{
	double minlat;
	double minlon;
	double maxlat;
	double maxlon;
} OSM_Bounds;

typedef struct{
	char *k;
	char *v;
} OSM_Tag;

typedef struct{
	unsigned long int id;
	double lat;
	double lon;
	int visible;
	int nb_tag;
	OSM_Tag *tags;
} OSM_Node;

typedef struct{
	unsigned long int id;
	int visible;
	int nb_node;
	OSM_Node** nodes;
	int nb_tag;
	OSM_Tag* tags;
} OSM_Way;

typedef struct{
	OSM_Bounds* bounds;
	int nb_node;
	OSM_Node* nodes;
	ABR_Node* abr_node;
	int nb_way;
	OSM_Way* ways;
	ABR_Node* abr_way;
}	OSM_Data;

typedef OSM_Data* OSM_DataPtr;

extern void fprintOSM_Bounds(FILE* stream, const OSM_Bounds bounds);
extern void fprintOSM_Node(FILE* stream, const OSM_Node node);
extern void fprintOSM_Tag(FILE* stream, const OSM_Tag tag);
extern void fprintOSM_Way(FILE* stream, const OSM_Way way);
extern void fprintOSM_Data(FILE* stream, const OSM_Data data);

extern void printOSM_Bounds(const OSM_Bounds bounds);
extern void printOSM_Node(const OSM_Node node);
extern void printOSM_Tag(const OSM_Tag tag);
extern void printOSM_Way(const OSM_Way way);
extern void printOSM_Data(const OSM_Data data);

OSM_Node* seachNodeById(const OSM_Node* nodes, int size, const long int id);
#endif /* _OSM_TYPES_H_ */
