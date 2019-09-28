#include <conio.h>
#include <stdio.h>
#include <string.h>
#include <libxml/parser.h>

xmlNode *find_child_node_by_name(xmlNode *node, char *child_name)
{
  xmlNode *child_node;

  // Look through all child-nodes (direct children only)
  for (child_node = node->children; child_node; child_node = child_node->next) {
    // Check if the name of the child node matches the argument
    if (strcmp(child_node->name, child_name) == 0) {
      return child_node;
    }
  }
}

/** Get the value of the child node with the given name (as long as it is a string). */
char *get_node_value(xmlNode *first, char *name)
{
  xmlNode *node = first->children;

  // Get the next entity
  while ((node = node->next) != NULL) {
    // Check if the name is the same as the argument
    if (strcasecmp(node->name, name) == 0) {
      // Return the value if it is a string, otherwise give up
      if (node->children->type == XML_TEXT_NODE) {
        return node->children->content;
      } else {
        return NULL;
      }
    }
  }
  // No value found
  return NULL;
}
