#ifndef __FPM_PLATFORMS__
#define __FPM_PLATFORMS__

struct dirent *get_next_file(DIR *dir);

xmlNode *get_first_game(xmlNode *node);

xmlNode *get_next_game(xmlNode *node);

char *get_fp_path(char *buffer, int size);

xmlNode *find_game(char *fp_path, char *field, char *value);

#endif
