#include <conio.h>
#include <dirent.h>
#include <stdio.h>
#include <libxml/parser.h>

#include "xml_util.h"
#include "platforms.h"
#include "game_launcher.h"

#include "os.c"

// @NOTES:
// * The game doesn't seem to get proxied properly when launched from a command prompt (unless it's in debug mode for some reason?)
// * This program only launches games, then immediately exits (so it doesn't run the router or redirector in the background)

/** Wait for input before continuing (mostly used for debugging). */
void wait_for_input()
{
  //printf("Press any key to continue...");
  //getch();
}

int main(int argc, char **argv)
{
  xmlNode *game;
  char *target_field, *target_value, fp_path[1024] = {0};

  // Extract command line arguments
  if (argc < 3) {
    printf("Error: Too few arguments.\n\n");
    printf("Usage: fp_micro <field> <value>\n");
    printf("Arguments:\n");
    printf("  <field> - Name of the field to grab the values from (ex. \"ID\" or \"Title\")\n");
    printf("  <value> - Field value of the game you are looking for (ex. \"Age of War\")\n\n");
    printf("Note: All arguments are case insensitive (\"id\" is the same as \"ID\").\n\n");
    printf("Locating Flashpoint:\n");
    printf("  The program will use the env var \"fp_path\" (or the cwd if it does not exist)\n");
    printf("  as the \"Flashpoint root path\".\n");
    printf("\n");
    wait_for_input();
    return 1;
  }
  target_field = argv[1];
  target_value = argv[2];
  // Get the Flashpoint path
  if (get_fp_path(fp_path, sizeof(fp_path)) == NULL) {
    printf("Error: Failed to get the flashpoint path\n");
    return 0;
  }
  printf("%s\n", fp_path);
  // Look for the game
  printf("Looking for the first game where:\n");
  printf("  \"%s\" = \"%s\"\n", target_field, target_value);
  game = find_game(fp_path, target_field, target_value);
  if (game) {
    printf("Game found!\n");
    printf("Game Meta (partial):\n");
    printf("  Title: %s\n", get_node_value(game, "Title"));
    printf("  ID:    %s\n", get_node_value(game, "ID"));
    // Launch it
    launch_game(game, fp_path);
  } else {
    printf("Game not found.\n");
  }
  wait_for_input();
}
