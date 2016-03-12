#include <stdio.h>
#include "osm_types.h"

void fprintOSM_Bounds(FILE* stream, const OSM_Bounds bounds){
	fprintf(stream,"OSM_Bounds:\n");
	fprintf(stream,"\tminlat: %f\n", bounds.minlat);
	fprintf(stream,"\tminlon: %f\n", bounds.minlon);
	fprintf(stream,"\tmaxlat: %f\n", bounds.maxlat);
	fprintf(stream,"\tmaxlon: %f\n", bounds.maxlon);
}

void fprintOSM_Node(FILE* stream, const OSM_Node node){
	fprintf(stream, "OSM_Node:\n");
	fprintf(stream, "\tid: %d\n", node.id);
	fprintf(stream, "\tlat: %f\n", node.lat);
	fprintf(stream, "\tlon: %f\n", node.lon);
	fprintf(stream, "\tvisible: %d\n", node.visible);
}

void fprintOSM_Tag(FILE* stream, const OSM_Tag tag){
	fprintf(stream, "OSM_Tag:\n");
	fprintf(stream, "\tk: %s\n", tag.k);
	fprintf(stream, "\tv: %s\n", tag.v);
}

void fprintOSM_Way(FILE* stream, const OSM_Way way){
	fprintf(stream, "OSM_Way:\n");
	fprintf(stream, "\tid: %d\n", way.id);
	fprintf(stream, "\tvisible: %d\n", way.visible);
}

void printOSM_Bounds(const OSM_Bounds bounds){
	fprintOSM_Bounds(stdout, bounds);
}

void printOSM_Node(const OSM_Node node){
	fprintOSM_Node(stdout, node);
}

void printOSM_Tag(const OSM_Tag tag){
	fprintOSM_Tag(stdout, tag);
}

void printOSM_Way(const OSM_Way way){
	fprintOSM_Way(stdout, way);
}

