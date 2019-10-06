#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <libxml/parser.h>

#include "xml_util.h"

typedef struct _platform platform;
typedef platform *platformPtr;
struct _platform {
  /** Parsed XML document. */
  xmlDoc *document;
  /** Full path and filename. */
  char *filename;
};

typedef struct _platCol platCol;
typedef platCol *platColPtr;
struct _platCol {
  /** Number of platforms in the array. */
  int length;
  /** Array of platforms (NOT null-terminated). */
  platform *platforms;
};

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
char *get_fp_path()
{
  char *buffer, *fp_path;

  // Try getting the environment variable
  if ((buffer = getenv("fp_path")) != NULL) {
    fp_path = malloc(strlen(buffer));
    strcpy(fp_path, buffer);
  }
  // Try getting the current working directory
  else if ((buffer = getcwd(buffer, 1024)) != NULL) {
    fp_path = malloc(strlen(buffer));
    strcpy(fp_path, buffer);
  }
  // All attempts failed
  else { return NULL; }
  // Success!
  return fp_path;
}

/** Load and parse all platform files in a folder and put them in a platform collection. */
platCol *load_platforms(char *fp_path)
{
  DIR *dir;
  struct dirent *ent;
  char folder_name[1024] = {0}, filename[1024] = {0};
  platCol *collection = NULL;
  xmlDoc *document = NULL;
  platform *platforms = NULL, *platforms_temp = NULL, *plat = NULL;
  int count = 0;

  // Allocate collection
  collection = malloc(sizeof(platCol));
  if (collection == NULL) { return NULL; } // Failed to allocate
  collection->length = 0;
  collection->platforms = NULL;
  // Allocate array
  platforms = malloc(sizeof(platform));
  // Prepare folder name
  strcpy(folder_name, fp_path);
  strcat(folder_name, "\\Data\\Platforms");
  // Get info about the folder
  if ((dir = opendir(folder_name)) != NULL) {
    // Look through all potential XML files
    while ((ent = get_next_file(dir)) != NULL) {
      // Get the full path string of the XML document
      sprintf(filename, "%s\\%s", folder_name, ent->d_name);
      // Read and parse XML document
      // @NOTE The XML documents are never freed from memory, this should probably be fixed
      document = xmlReadFile(filename, NULL, 0);
      // Add a new platform to the array (with the current doc and path)
      plat = &platforms[count];
      plat->document = document;
      plat->filename = malloc(strlen(filename));
      strcpy(plat->filename, filename);
      // Increment counter
      count += 1;
      // Re-allocate array (with more memory)
      platforms_temp = realloc(platforms, (count + 1) * sizeof(platform));
      if (platforms_temp != NULL) {
        platforms = platforms_temp;
      } else { // (Failed to allocate memory)
        printf("Failed to allocate memory for array.");
        free(platforms);
        return NULL;
      }
    }
    closedir(dir);
  }
  // Set collection values
  collection->length = count;
  collection->platforms = platforms;
  // Done
  return collection;
}

/** Search for a game in a platform collection by the value of one of its fields. */
xmlNode *find_game(platCol *collection, char *field, char *value)
{
  int i;
  platform *plat;
  xmlNode *node;
  char *node_value;

  // Look through all platforms
  plat = collection->platforms;
  for (i = collection->length; i > 0; i--) {
    // Look through each game
    for (node = get_first_game(xmlDocGetRootElement(plat->document)->children); node; node = get_next_game(node)) {
      // Compare the value of the node and the argument
      node_value = get_node_value(node, field);
      if ((node_value != NULL) && strcasecmp(node_value, value) == 0) {
        return node; // Game found!
      }
    }
    // Increment pointer
    plat++;
  }
  return NULL;
}
