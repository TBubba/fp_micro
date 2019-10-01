#ifndef __FPM_GAME_LAUNCHER__
#define __FPM_GAME_LAUNCHER__

void start_process(char *command);

char *replace_application_path(char *applicationPath, char *platform);

void create_command(char* buffer, char* fp_path, char* filename, char* args);

void launch_game(xmlNode *game, char *fp_path);

#endif
