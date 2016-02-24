#ifndef _OSM_PARSER_H_ 
#define _OSM_PARSER_H_

#include <libxml/parser.h>
#include <libxml/xpath.h>
#include <libxml/xpathInternals.h>
#include <libxml/tree.h>
#include "error.h"
#include "../model/osm_bounds.h"

extern parser_error_t execute_xpath_expression(const char* filename, const xmlChar* xpathExpr, xmlXPathObjectPtr* xpathObj);

extern parser_error_t getOSM_Bounds(const char* filename, OSM_Bounds* bounds);

#endif /* _OSM_PARSER_H_ */
