#ifndef _OSM_BOUNDS_H_ 
#define _OSM_BOUNDS_H_

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
	int id;
	double lat;
	double lon;
	int visible;
	OSM_Tag *tagList;
} OSM_Node;

typedef struct{
	int id;
	int visible;
	OSM_Node *nodeList;
	OSM_Tag *tagList;
} OSM_Way;

extern void fprintOSM_Bounds(FILE* stream, const OSM_Bounds bounds);
extern void fprintOSM_Node(FILE* stream, const OSM_Node node);
extern void fprintOSM_Tag(FILE* stream, const OSM_Tag tag);
extern void fprintOSM_Way(FILE* stream, const OSM_Way way);

extern void printOSM_Bounds(const OSM_Bounds bounds);
extern void printOSM_Node(const OSM_Node node);
extern void printOSM_Tag(const OSM_Tag tag);
extern void printOSM_Way(const OSM_Way way);

#endif /* _OSM_BOUNDS_H_ */
