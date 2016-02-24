#include <stdio.h>
#include "osm_bounds.h"

void fprintOSM_Bounds(FILE* stream, const OSM_Bounds bounds)
{
	fprintf(stream,"OSM_Bounds:\n");
	fprintf(stream,"\tminlat: %f\n", bounds.minlat);
	fprintf(stream,"\tminlon: %f\n", bounds.minlon);
	fprintf(stream,"\tmaxlat: %f\n", bounds.maxlat);
	fprintf(stream,"\tmaxlon: %f\n", bounds.maxlon);
}

void printOSM_Bounds(const OSM_Bounds bounds)
{
	fprintOSM_Bounds(stdout, bounds);
}
