#ifndef _OSM_BOUNDS_H_ 
#define _OSM_BOUNDS_H_

typedef struct{
	double minlat;
	double minlon;
	double maxlat;
	double maxlon;
} OSM_Bounds;

extern void fprintOSM_Bounds(FILE* stream, const OSM_Bounds bounds);
extern void printOSM_Bounds(const OSM_Bounds bounds);

#endif /* _OSM_BOUNDS_H_ */
