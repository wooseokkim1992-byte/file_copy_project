#include <stdio.h>
#include <stdlib.h>
#include "file_search.h"
#include <limits.h>

int main(int argc,char **argv){
    if(argc<3){
        fprintf(stderr,"Usage: %s <src_dir> <dst_dir>\n",argv[0]);
        return 1;
    }
    const char*src_dir=argv[1];
    const char*dest_dir=argv[2];
    if(list_directory(src_dir)!=0){
        fprintf(stderr,"Failed to list up files under src directory\n");
        return 1;
    }
    char filename[NAME_MAX];
    get_input(filename,NAME_MAX);
    printf("filename : %s\n",filename);
    char *src_path = reconstruct_path_1(argv[1],filename);
    char *dest_path = reconstruct_path_1(argv[2],filename);
    int type = is_directory(filename);
    printf("src_path : %s\n",src_path);
    printf("dest_path : %s\n",dest_path);
    if(type==1){
        fprintf(stderr,"Directory copy not implemented yet\n");
        return 1;
    }
    
    
    copy_file(src_path,dest_path);
    free(src_path);
    free(dest_path);
    return 0;
}