#include <stdio.h>
#include <stdlib.h>
#include "file_search.h"
#include <limits.h>

int main(int argc,char **argv){
    if(argc<3){
        fprintf(stderr,"Usage: %s <src_dir> <dst_dir>\n",argv[0]);
        return 1;
    }
    if(list_directory(argv[1])!=0){
        fprintf(stderr,"Failed to list up files under src directory\n");
        return 1;
    }
    char filename[NAME_MAX];
    get_input(filename,NAME_MAX);
    printf("filename : %s\n",filename);
    char *src_path = reconstruct_path_1(argv[1],filename);
    char *dest_path = reconstruct_path_1(argv[2],filename);
    move_file(src_path,dest_path);
    free(src_path);
    free(dest_path);
    return 0;
}
