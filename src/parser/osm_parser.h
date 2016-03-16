#ifndef _OSM_PARSER_H_ 
#define _OSM_PARSER_H_

#include <libxml/parser.h>
#include <libxml/xpath.h>
#include <libxml/xpathInternals.h>
#include <libxml/tree.h>
#include "error.h"
#include "../model/osm_types.h"

//extern parser_error_t execute_xpath_expression(const char* filename, const xmlChar* xpathExpr, parser_error_t (*parse_function)(xmlXPathObjectPtr xpathObj));

typedef struct osmParserFile osmParserFile;
typedef osmParserFile* osmParserFilePtr;

typedef struct osmParserDataSet osmParserDataSet;
typedef osmParserDataSet* osmParserDataSetPtr;

typedef xmlNodePtr osmParserIterator;



parser_error_t open_OSM_ParserFile(const char* filename, osmParserFilePtr* fileOut);

parser_error_t getOSM_Bounds(osmParserFilePtr file, osmParserDataSetPtr* dataSetOut);
parser_error_t getOSM_Node(osmParserFilePtr File, osmParserDataSetPtr* dataSetOut);
parser_error_t getOSM_Way(osmParserFilePtr File, osmParserDataSetPtr* dataSetOut);
parser_error_t getOSM_Relation(osmParserFilePtr File, osmParserDataSetPtr* dataSetOut);

void freeDataSet(osmParserDataSetPtr osmDataSet);

int getDataSet_lenght(osmParserDataSetPtr dataOut);

osmParserIterator getDataSet_begin( osmParserDataSetPtr osmDataSet  );
osmParserIterator getDataSet_end( osmParserDataSetPtr osmDataSet);
osmParserIterator getDataSet_element( osmParserDataSetPtr osmDataSet, int index);

OSM_Bounds bind_OSM_Bounds(osmParserIterator node);
OSM_Node bind_OSM_Node(osmParserIterator node);
OSM_Way bind_OSM_Way(osmParserIterator node);

/*
parser_error_t new_osmParserContext(const char* filename, osmParserContextPtr* context);

parser_error_t del_osmParserContext(osmParserContextPtr context);

parser_error_t getOSM_Bounds(OSM_Bounds* bounds, osmParserContextPtr context);
*/

//extern parser_error_t getOSM_Bounds(const char* filename, OSM_Bounds* bounds);
//extern parser_error_t getOSM_Node(const char* filename, OSM_Node* node);

#endif /* _OSM_PARSER_H_ */
