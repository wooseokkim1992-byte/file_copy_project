#ifndef FILE_SEARCH
#define FILE_SEARCH
    void list_directory(const char *dir_path);
    unsigned short search_and_open_dir(const char *src_dir_name,const char *dst_dir_name,const char *filename);
    char* reconstruct_path_1(const char* dir_path,const char* file_name);
    int copy_file(const char* src,const char* dest);
#endif