#include "osm_parser.h"
#include "../model/osm_types.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>

#include <libxml/parser.h>
#include <libxml/tree.h>
#include <libxml/xpath.h>
#include <libxml/xpathInternals.h>


struct osmParserFile {
	xmlDocPtr	doc;
};

struct osmParserDataSet {
	xmlXPathContextPtr	xpathCtx; 
	xmlXPathObjectPtr		xpathObj;
	xmlNodePtr					current_node;
};

typedef struct osmParserFile osmParserFile;
typedef osmParserFile* osmParserFilePtr;

typedef struct osmParserDataSet osmParserDataSet;
typedef osmParserDataSet* osmParserDataSetPtr;

typedef  osmParserDataPtr;


static parser_error_t _open_OSM_ParserFile(const char* filename, osmParserFilePtr* fileOut)
{
	osmParserFilePtr	file	= NULL;
	xmlDocPtr					doc		= NULL;

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

static void _close_OSM_ParserFile(osmParserFilePtr file)
{
	xmlFreeDoc(file->doc); 
	free(file);
}

static void _freeDataSet(osmParserDataSetPtr data)
{
	assert(data);

	if(data->xpathObj != NULL) 
		xmlXPathFreeObject(data->xpathObj);

	xmlXPathFreeContext(data->xpathCtx);

	free(data);
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

static int _countChildrenElementByName(xmlNodePtr node, const char* name)
{
	xmlNodePtr currentNode= NULL;
	int count = 0;
	for(currentNode = node->children; currentNode; currentNode=currentNode->next)
		if(currentNode->type == XML_ELEMENT_NODE) 
			if(xmlStrEqual(currentNode->name, xmlCharStrdup(name)))
				count++;
	return count;
}

static int _countChildrenTag(xmlNodePtr node)
{
	return _countChildrenElementByName(node, "tag");
}

static int _countChildrenNode(xmlNodePtr node)
{
	return _countChildrenElementByName(node, "nd");
}

static xmlNodePtr _getXmlNodeByIndex( osmParserDataSetPtr osmDataSet, int index)
{
	return osmDataSet->xpathObj->nodesetval->nodeTab[index];
}
/*
osmParserDataPtr getDataSet_end( osmParserDataSetPtr osmDataSet)
{
	return getDataSet_element( osmDataSet, getDataSet_lenght(osmDataSet)-1 );
}
*/



static parser_error_t _getOSM_Bounds(osmParserFilePtr file, osmParserDataSetPtr* dataOut)
{
  parser_error_t error= _execute_xpath( file, (xmlChar*)"/osm/bounds",  dataOut);
	if(error != PARSER_SUCESS)
		return error;
	return(PARSER_SUCESS);
}

static parser_error_t _getOSM_Node(osmParserFilePtr file, osmParserDataSetPtr* dataOut)
{
  parser_error_t error= _execute_xpath( file, (xmlChar*)"/osm/node",  dataOut);
	if(error != PARSER_SUCESS)
		return error;
	return(PARSER_SUCESS);
}

static parser_error_t _getOSM_Way(osmParserFilePtr file, osmParserDataSetPtr* dataOut)
{
  parser_error_t error= _execute_xpath( file, (xmlChar*)"/osm/way",  dataOut);
	if(error != PARSER_SUCESS)
		return error;
	return(PARSER_SUCESS);
}

static parser_error_t _getOSM_Relation(osmParserFilePtr file, osmParserDataSetPtr* dataOut)
{
  parser_error_t error= _execute_xpath( file, (xmlChar*)"/osm/relation",  dataOut);
	if(error != PARSER_SUCESS)
		return error;
	return(PARSER_SUCESS);
}

static int _getDataSet_lenght(osmParserDataSetPtr data)
{
	xmlNodeSetPtr node_set=  data->xpathObj->nodesetval;
	return (node_set) ? node_set->nodeNr : 0;
}



#define TRUE_STR "true"
#define TRUE_VAL 1
#define FALSE_STR "false"
#define FALSE_VAL 0


#define ATTR_BINDING_DOUBLE(field,attrname) \
	if(xmlStrEqual( xmlCharStrdup(attrname) , attr->name)) \
		field= atof( (char *) value);

#define ATTR_BINDING_INT(field,attrname) \
	if(xmlStrEqual( xmlCharStrdup(attrname) , attr->name)) \
		field= atoi( (char *) value);

#define ATTR_BINDING_LONG_INT(field,attrname) \
	if(xmlStrEqual( xmlCharStrdup(attrname) , attr->name)) \
		field= strtoul ( (char *) value , NULL, 0);

#define ATTR_BINDING_BOOL(field,attrname) \
	if(xmlStrEqual( xmlCharStrdup(attrname) , attr->name)) \
		field= strcmp(TRUE_STR, (char *) value) ? FALSE_VAL : TRUE_VAL;

#define ATTR_BINDING_STRING(field, attrname) \
	if(xmlStrEqual( xmlCharStrdup(attrname) , attr->name)) \
		*field= xmlStrdup(value);


static long int _bind_Ways_Nodes(xmlNodePtr node)
{
	long int osm_tag;
	xmlAttr* attr= node->properties;
	int index=0;
	while(attr && attr->name && attr->children)
	{
		xmlChar* value= xmlNodeListGetString(node->doc, attr->children, 1);
		ATTR_BINDING_LONG_INT(osm_tag, "ref")
		xmlFree(value);
		attr= attr->next;
	}
	return osm_tag;
}

static OSM_Tag _bind_OSM_Tag(xmlNodePtr node)
{
	OSM_Tag osm_tag;
	xmlAttr* attr= node->properties;
	int index=0;
	while(attr && attr->name && attr->children)
	{
		xmlChar* value= xmlNodeListGetString(node->doc, attr->children, 1);
		ATTR_BINDING_STRING(&osm_tag.k, "k")
		ATTR_BINDING_STRING(&osm_tag.v, "v")
		xmlFree(value);
		attr= attr->next;
	}
	return osm_tag;
}

static long int* _getWaysNodesList(xmlNodePtr data)
{
	long int* 		tag_list= (OSM_Tag*) calloc( _countChildrenTag(data), sizeof(long int*));
	long int* 		currentTag = tag_list;
	xmlNodePtr 	currentNode= NULL;

	for(currentNode = data->children; currentNode; currentNode=currentNode->next)
		if(currentNode->type == XML_ELEMENT_NODE) 
			if(xmlStrEqual(currentNode->name, xmlCharStrdup("nd")))
			{
				*currentTag= _bind_Ways_Nodes(currentNode);
				currentTag++;
			}
	return tag_list;
}


static OSM_Tag* _getTagsList(xmlNodePtr data)
{
	OSM_Tag* 		tag_list= (OSM_Tag*) calloc( _countChildrenTag(data), sizeof(OSM_Tag*));
	OSM_Tag* 		currentTag = tag_list;
	xmlNodePtr 	currentNode= NULL;

	for(currentNode = data->children; currentNode; currentNode=currentNode->next)
		if(currentNode->type == XML_ELEMENT_NODE) 
			if(xmlStrEqual(currentNode->name, xmlCharStrdup("tag")))
			{
				*currentTag= _bind_OSM_Tag(currentNode);
				currentTag++;
			}
	return tag_list;
}


OSM_Node _bind_OSM_Node(xmlNodePtr node)
{
	OSM_Node osm_node;
	xmlAttr* attr= node->properties;
	int index=0;
	while(attr && attr->name && attr->children)
	{
		xmlChar* value= xmlNodeListGetString(node->doc, attr->children, 1);
		ATTR_BINDING_LONG_INT(osm_node.id, "id")
		ATTR_BINDING_DOUBLE(osm_node.lat, "lat")
		ATTR_BINDING_DOUBLE(osm_node.lon, "lon")
		ATTR_BINDING_BOOL(osm_node.visible, "visible")
		xmlFree(value);
		attr= attr->next;
	}
	osm_node.nb_tag = _countChildrenTag(node);
	osm_node.tags= _getTagsList(node);
	return osm_node;
}

OSM_Bounds _bind_OSM_Bounds(xmlNodePtr node)
{
	OSM_Bounds	bounds;
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

OSM_Way _bind_OSM_Way(xmlNodePtr node)
{
	OSM_Way osm_way;
	xmlAttr* attr= node->properties;
	int index=0;
	while(attr && attr->name && attr->children)
	{
		xmlChar* value= xmlNodeListGetString(node->doc, attr->children, 1);
		ATTR_BINDING_LONG_INT(osm_way.id, "id")
		xmlFree(value);
		attr= attr->next;
	}
	osm_way.nb_node = _countChildrenNode(node);
	osm_way.nodes= 		_getWaysNodesList(node);
	osm_way.nb_tag = 	_countChildrenTag(node);
	osm_way.tags= 		_getTagsList(node);
	return osm_way;
}

static OSM_Node* _getNodesList(osmParserDataSetPtr data)
{
	int 			i;
	OSM_Node* nodesList= calloc( _getDataSet_lenght(data), sizeof(OSM_Node));

	OSM_Node*  current = nodesList;

	for(i=0; i < _getDataSet_lenght(data); i++, current++)
	{
		*current= _bind_OSM_Node(  _getXmlNodeByIndex(data, i) );
	}
	return nodesList;
}

static OSM_Way* _getWaysList(osmParserDataSetPtr data)
{
	int 			i;
	OSM_Way* list= calloc( _getDataSet_lenght(data), sizeof(OSM_Way));

	OSM_Way*  current = list;

	for(i=0; i < _getDataSet_lenght(data); i++, current++)
	{
		*current= _bind_OSM_Way(  _getXmlNodeByIndex(data, i) );
	}
	return list;
}

parser_error_t getOSM_data(const char* filename, OSM_Data** dataOut)
{
	osmParserFile* 		osmFile;
	osmParserDataSet* osmDataSet;
	parser_error_t 		error;
	int 							i;

	OSM_Data* data= (OSM_Data*) malloc(sizeof(OSM_Data));
	error= _open_OSM_ParserFile(filename, &osmFile);
	if(error != PARSER_SUCESS)
		return error;

	// === Bounds ===
	error= _getOSM_Bounds(osmFile, &osmDataSet);
	if(error != PARSER_SUCESS)
		return error;
	data->bounds = malloc( sizeof(OSM_Bounds));
	*data->bounds =_bind_OSM_Bounds( _getXmlNodeByIndex(osmDataSet, 0));
	_freeDataSet(osmDataSet);

	// === Nodes ===
	error=_getOSM_Node(osmFile, &osmDataSet);
	if(error != PARSER_SUCESS)
		return error;
	data->nb_node = _getDataSet_lenght(osmDataSet);
	data->nodes = _getNodesList(osmDataSet);
	_freeDataSet(osmDataSet);

	// === Ways ===
	error=_getOSM_Way(osmFile, &osmDataSet);
	if(error != PARSER_SUCESS)
		return error;
	data->nb_way = _getDataSet_lenght(osmDataSet);
	data->ways = _getWaysList(osmDataSet);
	_freeDataSet(osmDataSet);

	*dataOut = data;

	_close_OSM_ParserFile(osmFile);
	return(PARSER_SUCESS);
}




