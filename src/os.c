/**
 * Define more uniform macros for checking which OS this is compiled for.
 * 
 * Based on some code from some response here:
 * https://stackoverflow.com/questions/142508/how-do-i-check-os-with-a-preprocessor-directive
 */
#if defined(_WIN32) || defined(_WIN64) || defined(__WINDOWS__)
    #define OS_WINDOWS 1
#elif defined(__linux__)
    #define OS_LINUX 1
#elif defined(__APPLE__) && defined(__MACH__)
    #define OS_MAC 1
#elif defined(unix) || defined(__unix__) || defined(__unix)
    #define OS_UNIX 1
#else
    #define OS_UNKNOWN
#endif
