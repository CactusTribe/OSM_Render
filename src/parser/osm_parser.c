#include "osm_parser.h"
#include "osm_parser.h"


#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include <libxml/parser.h>
#include <libxml/xpath.h>
#include <libxml/xpathInternals.h>
#include <libxml/tree.h>

parser_error_t execute_xpath_expression(const char* filename, const xmlChar* xpathExpr, xmlXPathObjectPtr* xpathObj)
{
	xmlDocPtr doc;
	xmlXPathContextPtr xpathCtx; 

	assert(filename);
	assert(xpathExpr);

	/* Load XML document */
	doc = xmlParseFile(filename);
	if (doc == NULL) 
		return(PARSER_ERROR_PARSE_FILE);

	/* Create xpath evaluation context */
	xpathCtx = xmlXPathNewContext(doc);
	if(xpathCtx == NULL) {
		xmlFreeDoc(doc); 
		return(PARSER_ERROR_XPATH_CONTEXT);
	}

	/* Evaluate xpath expression */
	*xpathObj = xmlXPathEvalExpression(xpathExpr, xpathCtx);
	if(xpathObj == NULL) {
		xmlXPathFreeContext(xpathCtx); 
		xmlFreeDoc(doc); 
		return(PARSER_ERROR_EVALUATE_XPATH);
	}

	/* Cleanup */
	//xmlXPathFreeContext(xpathCtx); 
	//xmlFreeDoc(doc); 
	return(PARSER_SUCESS);
}

parser_error_t getOSM_Bounds(const char* filename, OSM_Bounds* bounds)
{
	xmlXPathObjectPtr xpathObj;
	xmlNodeSetPtr nodes;
  xmlNodePtr bounds_node;
  parser_error_t error;
  int size;
	
	error= execute_xpath_expression(filename, "/osm/bounds",  &xpathObj);
	if(error != PARSER_SUCESS)
	{
		return error;
	}
	
	nodes=  xpathObj->nodesetval;
	size= (nodes) ? nodes->nodeNr : 0;
	
	if(size != 1)
	{
		xmlXPathFreeObject(xpathObj);
		return PARSER_ERROR_BOUNDS_NUMBER;
	}

	bounds_node= nodes->nodeTab[0]; 
	if(bounds_node->type != XML_ELEMENT_NODE) 
	{
		xmlXPathFreeObject(xpathObj);
		return PARSER_ERROR_BOUNDS_TYPE;
	}
	
	xmlAttr* attr= bounds_node->properties;
	while(attr && attr->name && attr->children)
	{
		xmlChar* value= xmlNodeListGetString(bounds_node->doc, attr->children, 1);
		
		if(strcmp (attr->name , "minlat") == 0 )
			bounds->minlat = atof(value);
		else if(strcmp (attr->name , "minlon") == 0 )
			bounds->minlon = atof(value);
		else if(strcmp (attr->name , "maxlat") == 0 )
			bounds->maxlat = atof(value);
		else if(strcmp (attr->name , "maxlon") == 0 )
			bounds->maxlon = atof(value);
			
		xmlFree(value);
		attr= attr->next;
	}
	
	xmlXPathFreeObject(xpathObj);
	return(PARSER_SUCESS);
}

parser_error_t getOSM_Node(const char* filename, OSM_Node* node){

	xmlXPathObjectPtr xpathObj;
	xmlNodeSetPtr nodes;
  xmlNodePtr node_node;
  parser_error_t error;
  int size;
	
	error= execute_xpath_expression(filename, "/osm/node",  &xpathObj);
	if(error != PARSER_SUCESS){
		return error;
	}

	nodes =  xpathObj->nodesetval;
	size = (nodes) ? nodes->nodeNr : 0;

	if(size != 1)
	{
		xmlXPathFreeObject(xpathObj);
		return PARSER_ERROR_BOUNDS_NUMBER;
	}

	node_node= nodes->nodeTab[0]; 
	if(node_node->type != XML_ELEMENT_NODE) 
	{
		xmlXPathFreeObject(xpathObj);
		return PARSER_ERROR_BOUNDS_TYPE;
	}
	
	xmlAttr* attr= node_node->properties;

	while(attr && attr->name && attr->children){
		
		xmlChar* value = xmlNodeListGetString(node_node->doc, attr->children, 1);

		printf("fsfsef %s\n", value);

		if(strcmp (attr->name , "id") == 0 )
			node->id = atoi(value);
		else if(strcmp (attr->name , "lat") == 0 )
			node->lat = atof(value);
		else if(strcmp (attr->name , "lon") == 0 ){
			printf("fsfsef %s\n", value);
			node->lon = atof(value);
		}
		else if(strcmp (attr->name , "visbile") == 0 )
			node->visible = (strcmp (value, "true")) ? 0 : 1;
			
		xmlFree(value);
		attr = attr->next;
	}
	
	xmlXPathFreeObject(xpathObj);
	return(PARSER_SUCESS);
}




