#include "osm_parser.h"
#include "osm_bind.h"
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

#define TRUE_STR "true"
#define TRUE_VAL 1
#define FALSE_STR "false"
#define FALSE_VAL 0

#define ATTRbindING_DOUBLE(field,attrname) \
	if(xmlStrEqual( xmlCharStrdup(attrname) , attr->name)) \
		field= atof( (char *) value);

#define ATTRbindING_INT(field,attrname) \
	if(xmlStrEqual( xmlCharStrdup(attrname) , attr->name)) \
		field= atoi( (char *) value);

#define ATTRbindING_LONG_INT(field,attrname) \
	if(xmlStrEqual( xmlCharStrdup(attrname) , attr->name)) \
		field= strtoul ( (char *) value , NULL, 10);

#define ATTRbindING_BOOL(field,attrname) \
	if(xmlStrEqual( xmlCharStrdup(attrname) , attr->name)) \
		field= strcmp(TRUE_STR, (char *) value) ? FALSE_VAL : TRUE_VAL;

#define ATTRbindING_STRING(field, attrname) \
	if(xmlStrEqual( xmlCharStrdup(attrname) , attr->name)) \
		*field= (char *) xmlStrdup(value);

#define FOREACH_ALL_ATTR \
	for(xmlAttr* attr= node->properties; attr && attr->name && attr->children; attr= attr->next) \
	{ \
		xmlChar* value= xmlNodeListGetString(node->doc, attr->children, 1);

#define END_FOREACH_ALL_ATTR \
		xmlFree(value); \
	} 

#define FOREACH_ALL_CHILDREN_NODE \
	for(xmlNodePtr currentNode = data->children; currentNode; currentNode=currentNode->next) \
	{


#define END_FOREACH_ALL_CHILDREN_NODE \
	}

#define IF_NODE_ELEMENT_NAME_EQUAL( NODE_NAME ) \
		if(currentNode->type == XML_ELEMENT_NODE)  \
		{ \
			if(strcmp( (char *) currentNode->name, NODE_NAME) == 0) \
			{ 

#define ENDIF_NODE_ELEMENT_NAME_EQUAL \
			} \
		}

#define CHILDREN_CURRENT_NODE  currentNode

OSM_Bounds bind_OSM_Bounds(xmlNodePtr node)
{
	OSM_Bounds	bounds;

	FOREACH_ALL_ATTR
		ATTRbindING_DOUBLE(bounds.minlat, "minlat")
		ATTRbindING_DOUBLE(bounds.minlon, "minlon")
		ATTRbindING_DOUBLE(bounds.maxlat, "maxlat")
		ATTRbindING_DOUBLE(bounds.maxlon, "maxlon")
	END_FOREACH_ALL_ATTR

	return bounds;
}

OSM_Tag bind_OSM_Tag(xmlNodePtr node)
{
	OSM_Tag osm_tag;

	FOREACH_ALL_ATTR
		ATTRbindING_STRING(&osm_tag.k, "k")
		ATTRbindING_STRING(&osm_tag.v, "v")
	END_FOREACH_ALL_ATTR

	return osm_tag;
}


OSM_Node bind_OSM_Node(xmlNodePtr node)
{
	OSM_Node osm_node;

	FOREACH_ALL_ATTR
		ATTRbindING_LONG_INT(osm_node.id, "id")
		ATTRbindING_DOUBLE(osm_node.lat, "lat")
		ATTRbindING_DOUBLE(osm_node.lon, "lon")
		ATTRbindING_BOOL(osm_node.visible, "visible")
	END_FOREACH_ALL_ATTR

	osm_node.nb_tag = countChildrenTag(node);
	osm_node.tags		= getTagList(node);
	return osm_node;
}

OSM_Way bind_OSM_Way(OSM_Data* osm_data, xmlNodePtr node)
{
	OSM_Way osm_way;

	FOREACH_ALL_ATTR
		ATTRbindING_LONG_INT(osm_way.id, "id")
		ATTRbindING_BOOL(osm_way.visible, "visible")
	END_FOREACH_ALL_ATTR

	osm_way.nb_node = countChildrenNode(node);
	osm_way.nodes		= getRefNodeList(osm_data, node);
	osm_way.nb_tag 	= countChildrenTag(node);
	osm_way.tags		= getTagList(node);
	return osm_way;
}

OSM_Member bind_OSM_Member(OSM_Data* osm_data, xmlNodePtr node)
{
	OSM_Member osm_member;
	char* member_type;
	long int id;

	FOREACH_ALL_ATTR
		ATTRbindING_STRING(&member_type, "type")
		ATTRbindING_LONG_INT(id, "ref")
		ATTRbindING_STRING(&osm_member.role, "role")
	END_FOREACH_ALL_ATTR

	if(strcmp(member_type, OSM_MEMBER_NODE_STR) == 0)
	{
		osm_member.type = OSM_MEMBER_NODE;
		osm_member.ref 	= searchNode(osm_data->abr_node, id);
	}
	else
	{
		osm_member.type = OSM_MEMBER_WAY;
		osm_member.ref	= searchNode(osm_data->abr_way, id);
	}
	return osm_member;
}


OSM_Relation bind_OSM_Relation(OSM_Data* osm_data, xmlNodePtr node)
{
	OSM_Relation osm_relation;

	FOREACH_ALL_ATTR
		ATTRbindING_LONG_INT(osm_relation.id, "id")
		ATTRbindING_BOOL(osm_relation.visible, "visible")
	END_FOREACH_ALL_ATTR

	osm_relation.nb_member	=	countChildrenMember(node);
	//osm_relation.members		=	getWayNodesList(osm_data, node);
	osm_relation.nb_tag			=	countChildrenTag(node);
	osm_relation.tags				=	getTagList(node);
	return osm_relation;
}

//
// Bind ref
//


OSM_Node* bind_Ref_Node(OSM_Data* osm_data, xmlNodePtr node)
{
	long int id;

	FOREACH_ALL_ATTR
		ATTRbindING_LONG_INT(id, "ref")
	END_FOREACH_ALL_ATTR

	return (OSM_Node*) searchNode(osm_data->abr_node, id);
}

//
// Get list
//

OSM_Node* getNodeList(osmParserDataSetPtr data)
{
	OSM_Node* nodesList= calloc( getDataSet_lenght(data), sizeof(OSM_Node));

	OSM_Node*  current = nodesList;

	for(int i=0; i < getDataSet_lenght(data); i++, current++)
	{
		*current= bind_OSM_Node(  getXmlNodeByIndex(data, i) );
	}
	return nodesList;
}

OSM_Way* getWayList(OSM_Data* osm_data, osmParserDataSetPtr data)
{
	int 			i;
	OSM_Way* list= calloc( getDataSet_lenght(data), sizeof(OSM_Way));
	OSM_Way*  current = list;

	for(i=0; i < getDataSet_lenght(data); i++, current++)
	{
		*current= bind_OSM_Way(osm_data,  getXmlNodeByIndex(data, i) );
	}
	return list;
}


OSM_Node** getRefNodeList(OSM_Data* osm_data, xmlNodePtr data)
{

	OSM_Node** 	tag_list= (OSM_Node**) calloc( countChildrenNode(data), sizeof(OSM_Node**));
	OSM_Node** 		currentTag = tag_list;

	FOREACH_ALL_CHILDREN_NODE
		IF_NODE_ELEMENT_NAME_EQUAL("nd")
			*currentTag= bind_Ref_Node(osm_data, CHILDREN_CURRENT_NODE);
			currentTag++;
		ENDIF_NODE_ELEMENT_NAME_EQUAL
	END_FOREACH_ALL_CHILDREN_NODE

	return tag_list;
}


OSM_Tag* getTagList(xmlNodePtr data)
{
	OSM_Tag* 		tag_list= (OSM_Tag*) calloc( countChildrenTag(data), sizeof(OSM_Tag));
	OSM_Tag* 		currentTag = tag_list;

	FOREACH_ALL_CHILDREN_NODE
		IF_NODE_ELEMENT_NAME_EQUAL("tag")
			*currentTag= bind_OSM_Tag(CHILDREN_CURRENT_NODE);
			currentTag++;
		ENDIF_NODE_ELEMENT_NAME_EQUAL
	END_FOREACH_ALL_CHILDREN_NODE
			
	return tag_list;
}

OSM_Relation* getRelationList(OSM_Data* osm_data, osmParserDataSetPtr data)
{
	int 			i;
	OSM_Relation* list= calloc( getDataSet_lenght(data), sizeof(OSM_Relation));
	OSM_Relation* current = list;

	for(i=0; i < getDataSet_lenght(data); i++, current++)
	{
		*current= bind_OSM_Relation(osm_data,  getXmlNodeByIndex(data, i) );
	}
	return list;
}

