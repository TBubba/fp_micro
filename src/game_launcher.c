#include <string.h>
#include <libxml/parser.h>
#include <windows.h>

#include "xml_util.h"

/** Start a new process with a command. */
void start_process(char *command)
{
  // @TODO Add linux (and mac?) support
  STARTUPINFO si;
  PROCESS_INFORMATION pi;

  // Initialize process and startup info containers
  ZeroMemory(&si, sizeof(si));
  si.cb = sizeof(si);
  ZeroMemory(&pi, sizeof(pi));
  // Start a new process
  CreateProcessA(
    NULL,
    command,
    NULL,
    NULL,
    FALSE,
    0,
    NULL,
    NULL,
    &si,
    &pi
  );
  // Log error (if any)
  //printf( "CreateProcess failed (%d).\n", GetLastError() );
}

/**
 * Replace the application path for specific platform / OS combinations.
 * This is a temporary solution for using the .sh files for linux.
 */
char *replace_application_path(char *applicationPath, char *platform)
{
#ifdef OS_LINUX
  if (strcmp(platform, "Java") == 0) {
    return "FPSoftware/startJava.sh";
  } else if (strcmp(platform, "Unity") == 0) {
    return "FPSoftware/startUnity.sh";
  } else {
    return applicationPath;
  }
#else
  return applicationPath;
#endif
}

/** Create a command line string that will launch a game. */
void create_command(char* buffer, char* fp_path, char* filename, char* args)
{
  char *escFilename = NULL, *escArgs = NULL;

  // Escape filename
  escFilename = filename;
  // Escape arguments
  #ifdef OS_WINDOWS
    escArgs = args;
    // @TODO Escape arguments for windows
  #elif defined(OS_LINUX)
    escArgs = args;
    // @TODO Escape arguments for linux
  #else // Not-yet-supproted
    escArgs = args;
  #endif
  // Apply Wine
  // @TODO add Wine support
  // if (wine) {
  //   escArgs = `start /unix "${escFilename}" ${escArgs}`;
  //   escFilename = 'wine';
  // }
  // Combine and return
  sprintf(buffer, "\"%s\\%s\" %s", fp_path, escFilename, escArgs);
}

/** Launch a game. */
void launch_game(xmlNode *game, char *fp_path)
{
  xmlNode *node;
  char *platform = NULL, *commandLine = NULL, *applicationPath = NULL;
  char str[2048] = {0}; // (idk how sound this number is, maybe it's too small?)

  // @TODO Find all add-apps for this game, and run the ones with "AutoRunBefore"
  //       set to true before the game.

  // Get the Platform value
  node = find_child_node_by_name(game, "Platform");
  if (node) { platform = node->children->content; }
  // Get the Application Path value
  node = find_child_node_by_name(game, "ApplicationPath");
  if (node) {
    applicationPath = replace_application_path(node->children->content, platform);
  }
  // Get the Launch Command value
  node = find_child_node_by_name(game, "CommandLine");
  if (node) { commandLine = node->children->content; }
  // Check if all necessary values were extracted
  if (applicationPath && commandLine) {
    // Create the launch command
    create_command(str, fp_path, applicationPath, commandLine);
    // Log
    printf("Launching:\n");
    printf("  Application Path: \"%s\"\n", applicationPath);
    printf("  Command Line:     \"%s\"\n", commandLine);
    printf("  Run: \"%s\"\n", str);
    // Launch the game
    start_process(str);
  } else {
    // Log error
    printf("Failed to launch game. Property \"ApplicationPath\" and/or \"CommandLine\" is missing.\n");
  }
}
