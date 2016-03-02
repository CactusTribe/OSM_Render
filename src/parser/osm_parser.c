#include "osm_parser.h"
#include "osm_parser.h"

#define ATTR_BINDING_DOUBLE(field,attrname) \
	if(xmlStrEqual( xmlCharStrdup(attrname) , attr->name)) \
		field= atof( (char *) value);

#define ATTR_BINDING_INT(field,attrname) \
	if(xmlStrEqual( xmlCharStrdup(attrname) , attr->name)) \
		field= atoi( (char *) value);

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include <libxml/parser.h>
#include <libxml/xpath.h>
#include <libxml/xpathInternals.h>
#include <libxml/tree.h>

struct osmParserFile {
	xmlDocPtr	doc;
};

struct osmParserDataSet {
	xmlXPathContextPtr	xpathCtx; 
	xmlXPathObjectPtr		xpathObj;
	xmlNodePtr					current_node;
};





parser_error_t open_OSM_ParserFile(const char* filename, osmParserFilePtr* fileOut)
{
	osmParserFilePtr		file			= NULL;
	xmlDocPtr 					doc				= NULL;

	assert(filename);
	assert(fileOut);

	// Load XML document
	doc = xmlParseFile(filename);
	if (doc == NULL) 
		return(PARSER_ERROR_PARSE_FILE);

	// Save osmParserFile
	file= (osmParserFilePtr) malloc(sizeof(struct osmParserFile));
	file->doc= doc;
	*fileOut = file;

	return(PARSER_SUCESS);
}

static parser_error_t _execute_xpath(osmParserFilePtr file, const xmlChar* xpathExpr, osmParserDataSetPtr* dataOut)
{
	osmParserDataSetPtr	data	= NULL;
	data= (osmParserDataSetPtr) malloc(sizeof(struct osmParserDataSet));

	// Create xpath evaluation context
	data->xpathCtx = xmlXPathNewContext(file->doc);
	if(data->xpathCtx == NULL) 
		return(PARSER_ERROR_XPATH_CONTEXT);


	// Execute xpath evaluation
	data->xpathObj = xmlXPathEvalExpression(xpathExpr, data->xpathCtx);
	if(data->xpathObj == NULL) 
		return(PARSER_ERROR_EVALUATE_XPATH);

	*dataOut = data;
	return PARSER_SUCESS;
}


osmParserIterator getDataSet_begin( osmParserDataSetPtr osmDataSet  )
{
	return osmDataSet->xpathObj->nodesetval->nodeTab[0];
}

osmParserIterator getDataSet_end( osmParserDataSetPtr osmDataSet)
{
	return getDataSet_element( osmDataSet, getDataSet_lenght(osmDataSet)-1 );
}

osmParserIterator getDataSet_element( osmParserDataSetPtr osmDataSet, int index)
{
	return osmDataSet->xpathObj->nodesetval->nodeTab[index];
}

parser_error_t getOSM_Bounds(osmParserFilePtr file, osmParserDataSetPtr* dataOut)
{
  parser_error_t error= _execute_xpath( file, (xmlChar*)"/osm/bounds",  dataOut);
	if(error != PARSER_SUCESS)
		return error;
	return(PARSER_SUCESS);
}

parser_error_t getOSM_Node(osmParserFilePtr file, osmParserDataSetPtr* dataOut)
{
  parser_error_t error= _execute_xpath( file, (xmlChar*)"/osm/node",  dataOut);
	if(error != PARSER_SUCESS)
		return error;
	return(PARSER_SUCESS);
}

parser_error_t getOSM_Way(osmParserFilePtr file, osmParserDataSetPtr* dataOut)
{
  parser_error_t error= _execute_xpath( file, (xmlChar*)"/osm/way",  dataOut);
	if(error != PARSER_SUCESS)
		return error;
	return(PARSER_SUCESS);
}

parser_error_t getOSM_Relation(osmParserFilePtr file, osmParserDataSetPtr* dataOut)
{
  parser_error_t error= _execute_xpath( file, (xmlChar*)"/osm/relation",  dataOut);
	if(error != PARSER_SUCESS)
		return error;
	return(PARSER_SUCESS);
}


int getDataSet_lenght(osmParserDataSetPtr data)
{
	xmlNodeSetPtr node_set=  data->xpathObj->nodesetval;
	return (node_set) ? node_set->nodeNr : 0;
}






void freeDataSet(osmParserDataSetPtr data)
{
	xmlXPathFreeObject(data->xpathObj);
	free(data);
}



OSM_Bounds bind_OSM_Bounds(osmParserIterator node)
{
	OSM_Bounds bounds;
	xmlAttr* attr= node->properties;

	while(attr && attr->name && attr->children)
	{
		xmlChar* value= xmlNodeListGetString(node->doc, attr->children, 1);

		ATTR_BINDING_DOUBLE(bounds.minlat, "minlat")
		ATTR_BINDING_DOUBLE(bounds.minlon, "minlon")
		ATTR_BINDING_DOUBLE(bounds.maxlat, "maxlat")
		ATTR_BINDING_DOUBLE(bounds.maxlon, "maxlon")

		xmlFree(value);
		attr= attr->next;
	}
	return bounds;
}




OSM_Node bind_OSM_Node(osmParserIterator node)
{
	OSM_Node osm_node;
	xmlAttr* attr= node->properties;

	while(attr && attr->name && attr->children)
	{
		xmlChar* value= xmlNodeListGetString(node->doc, attr->children, 1);

		ATTR_BINDING_INT(osm_node.id, "id")
		ATTR_BINDING_DOUBLE(osm_node.lat, "lat")
		ATTR_BINDING_DOUBLE(osm_node.lon, "lon")
		ATTR_BINDING_INT(osm_node.visible, "visible")

		xmlFree(value);
		attr= attr->next;
	}
	return osm_node;
}



/*
parser_error_t del_osmParserContext(osmParserContextPtr context)
{
	assert(context);

	if(context->xpathObj != NULL) 
		xmlXPathFreeObject(context->xpathObj);

	xmlXPathFreeContext(context->xpathCtx);
	xmlFreeDoc(context->doc); 
	free(context);
	return(PARSER_SUCESS);
}
*/

