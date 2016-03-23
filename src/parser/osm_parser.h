#ifndef _OSM_PARSER_H_ 
#define _OSM_PARSER_H_

#include "error.h"
#include "../model/osm_types.h"

parser_error_t getOSM_data(const char* filename, OSM_Data** dataOut);

#endif /* _OSM_PARSER_H_ */
