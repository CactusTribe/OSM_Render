Parser
======

Fonctionnement globale
----------------------

* Ouverture du fichier XML (open_OSM_ParserFile)

* Localisation du <bounds> avec une expression XPath (getOSM_Bounds)
* Analyse syntaxique et enregistrement du <bounds> dans OSM_Data (bind_OSM_Bounds) 

* Localisation des <node> avec une expression XPath (getOSM_Node)
* Analyse syntaxique et enregistrement du <node> dans OSM_Data (getNodeList) 
* Peuplement de l'ABR de node (addNode)

* Localisation des <way> avec une expression XPath (getOSM_Way)
* Analyse syntaxique et enregistrement du <way> dans OSM_Data (getWayList) 
* Peuplement de l'ABR de way (addNode)

* Localisation des <relation> avec une expression XPath (getOSM_Relation)
* Analyse syntaxique et enregistrement du <relation> dans OSM_Data (getRelationList) 
* Peuplement de l'ABR de relation (addNode)
* Mise à jour des references contenu dans les relation (linkRelationMembers)

* Fermeture du fichier XML (close_OSM_ParserFile)


Enregistrement
--------------

### id des elements (Nodes / Ways / Relation)

typedef unsigned long int osm_element_id_t; 

### Data

typedef struct{
	OSM_Bounds* bounds;
	// nodes
	unsigned int 	nb_node;
	OSM_Node* 		nodes;
	ABR_Node* 		abr_node;
	// ways
	unsigned int	nb_way;
	OSM_Way* 			ways;
	ABR_Node* 		abr_way;
	// relations
	unsigned int 	nb_relation;
	OSM_Relation*	relations;
	ABR_Node* 		abr_relation;

}	OSM_Data;

### Bounds

typedef struct{
	double minlat;
	double minlon;
	double maxlat;
	double maxlon;
} OSM_Bounds;

### Ect ...


### Type de OSM_Member

Le bit 7 contient l'information sur le type de pointer
* '0': pointer vers une struct (OSM_Node, OSM_Way, OSM_Relation)
* '1': pointer vers un id (osm_element_id_t)

#define OSM_MEMBER_REF_ID_BIT						(0x80)
#define OSM_MEMBER_REF_ID_MASK					(~OSM_MEMBER_REF_ID_BIT)

Les autres bits (6 downto 0) servent à distinguer le type d'element

#define OSM_MEMBER_WAY_TYPE							0x01
#define OSM_MEMBER_NODE_TYPE						0x02
#define OSM_MEMBER_RELATION_TYPE				0x03
#define OSM_MEMBER_UNDEFINED_TYPE				0x08

Enregistrement des elements
---------------------------

getNodeList : Analyse syntaxiquement le resultat XPath et retourne une liste de node
getWayList: [....]

bind_OSM_Bounds : Analyse syntaxiquement le node XML et retourne un OSM_Bounds
bind_OSM_Tag : Analyse syntaxiquement le node XML et retourne un OSM_Bounds
bind_OSM_Node : [...]

linkRelationMembers : met à jour les pointer des membre de relation (voir Enregistrement des relations)


Enregistrement des relations
----------------------------

L'enregistrement des relations à une particularité, elle se fait en deux fois car il existe des relations contenant des relations.

D'abord, on procède à l'analyse syntaxique et enregistrement (getRelationList) 
Les members de chaque relation sont de type pointer vers un id 

Ensuite on peuple l'ABR de relation (addNode)

A la fin nous effectuons la mise à jour des pointer contenu dans les membre de relations (linkRelationMembers)

Les id existant dans les ABR deviennent alors des pointer vers struct et les inconnue reste inchangé.

