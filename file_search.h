#ifndef FILE_SEARCH
#define FILE_SEARCH
#include <stdlib.h>
    #define MAX_FILE_PATH 4096
    #define BUF_SIZE 4096
    #define MAX_FILE_NAME 256
    #define FILE_TYPE_COUNT 9
    #include <dirent.h>

    typedef enum e_filetype {
        FILETYPE_UNKNOWN = DT_UNKNOWN,
        FILETYPE_FIFO    = DT_FIFO,
        FILETYPE_CHR     = DT_CHR,
        FILETYPE_DIR     = DT_DIR,
        FILETYPE_BLK     = DT_BLK,
        FILETYPE_REG     = DT_REG,
        FILETYPE_LNK     = DT_LNK,
        FILETYPE_SOCK    = DT_SOCK,
        FILETYPE_WHT     = DT_WHT
    } t_filetype;

    static const char *filetype_str[] = {
        [DT_UNKNOWN] = "UNKNOWN",
        [DT_FIFO]    = "FIFO",
        [DT_CHR]     = "CHAR_DEVICE",
        [DT_DIR]     = "DIRECTORY",
        [DT_BLK]     = "BLOCK_DEVICE",
        [DT_REG]     = "REGULAR_FILE",
        [DT_LNK]     = "SYMLINK",
        [DT_SOCK]    = "SOCKET",
        [DT_WHT]     = "WHITEOUT"
    };
    int list_directory(const char *dir_path);
    int is_directory(const char *path);
    void get_input(char *buf, size_t size);
    unsigned short search_and_open_dir(const char *src_dir_name,const char *dst_dir_name,const char *filename);
    char* reconstruct_path_1(const char* dir_path,const char* file_name);
    //return value is made by malloc
    int copy_file(const char* src,const char* dest);
    int move_file(const char *src_path,const char *dest_path);
    const char* check_file_or_dir(const char *path);
#endif
