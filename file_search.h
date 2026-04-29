#ifndef FILE_SEARCH
#define FILE_SEARCH
#include <stdlib.h>

#define MAX_FILE_PATH 64
    #define BUF_SIZE 4096
    #define MAX_FILE_NAME 256
    int list_directory(const char *dir_path);
    int is_directory(const char *path);
    void get_input(char *buf, size_t size);
    unsigned short search_and_open_dir(const char *src_dir_name,const char *dst_dir_name,const char *filename);
    char* reconstruct_path_1(const char* dir_path,const char* file_name);
    int copy_file(const char* src,const char* dest);
    int move_file(const char *src_path,const char *dest_path);
#endif