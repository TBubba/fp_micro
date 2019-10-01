#ifndef __FPM_XML_UTIL__
#define __FPM_XML_UTIL__

xmlNode *find_child_node_by_name(xmlNode *node, char *child_name);

char *get_node_value(xmlNode *first, char *name);

#endif
