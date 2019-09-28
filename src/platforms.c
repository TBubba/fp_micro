#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <unistd.h>
#include <libxml/parser.h>

struct dirent *get_next_file(DIR *dir)
{
  struct dirent *ent;
  
  // Get the next entity
  while ((ent = readdir(dir)) != NULL) {
    // Check if it is a regular file
    if (ent->d_type == DT_REG) {
      return ent;
    }
  }
  // Out of entities
  return NULL;
}

/** Find the first game (by checking both the initial node and it's "next" chain). */
xmlNode *get_first_game(xmlNode *node)
{
  // Look for the first game (including the first node)
  while (node != NULL) {
    if (strcmp(node->name, "Game") == 0) {
      return node;
    } else {
      node = node->next;
    }
  }
  return NULL;
}

/** Find the next game (in the node's "next" chain). */
xmlNode *get_next_game(xmlNode *node)
{
  // Look for the next game (excluding the current node)
  while ((node = node->next) != NULL) {
    if (strcmp(node->name, "Game") == 0) {
      return node;
    }
  }
  return NULL;
}

/** Get the path of the flashpoint folder. */
char *get_fp_path(char *buffer, int size)
{
  char *temp;

  // Try getting the environment variable
  if ((temp = getenv("fp_path")) != NULL) {
    strcpy(buffer, temp);
  }
  // Try getting the current working directory
  else if (getcwd(buffer, size) != NULL) {
    // Do nothing...
  }
  // All attempts failed
  else {
    return NULL;
  }
  // Success!
  return buffer;
}

/** Find the game with the specified ID */
xmlNode *find_game(char *fp_path, char *field, char *value)
{
  // @NOTE The XML document is never freed, that is probably not very good
  DIR *dir;
  struct dirent *ent;
  xmlDoc *document;
  xmlNode *node;
  char folder_name[1024] = {0}, filename[1024] = {0}, *node_value;

  // Prepare folder name
  strcpy(folder_name, fp_path);
  strcat(folder_name, "\\Data\\Platforms");
  // Get info about the folder
  if ((dir = opendir(folder_name)) != NULL) {
    // Look through all potential XML files
    while ((ent = get_next_file(dir)) != NULL) {
      // Get the full path string of the xml document
      sprintf(filename, "%s\\%s", folder_name, ent->d_name);
      // Read and parse the XML
      document = xmlReadFile(filename, NULL, 0);
      // Look through the games
      for (node = get_first_game(xmlDocGetRootElement(document)->children); node; node = get_next_game(node)) {
        // Compare the value of the node and the argument
        node_value = get_node_value(node, field);
        if ((node_value != NULL) && strcasecmp(node_value, value) == 0) {
          closedir(dir);
          return node; // Game found!
        }
      }
    }
    closedir(dir);
  }
  return NULL;
}
