#ifndef __FPM_PLATFORMS__
#define __FPM_PLATFORMS__

typedef struct _platform platform;
typedef platform *platformPtr;

typedef struct _platCol platCol;
typedef platCol *platColPtr;

char *get_fp_path();

platCol *load_platforms(char *fp_path);

xmlNode *find_game(platCol *platforms, char *field, char *value);

#endif
