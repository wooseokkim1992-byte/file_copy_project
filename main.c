#include <stdio.h>
#include <stdlib.h>
#include "file_search.h"

int main(int argc,char **argv){
    if(argc<4){
        fprintf(stderr,"Usage: %s <src_dir> <dst_dir> <keyword>\n",argv[0]);
        return 1;
    }
    if(search_and_open_dir(argv[1],argv[2],argv[3])){
        fprintf(stderr,"dir open failed\n");
        return 1;
    }
    char *src_path = reconstruct_path_1(argv[1],argv[3]);
    char *dest_path = reconstruct_path_1(argv[2],argv[3]);
    printf("src_path : %s\n",src_path);
    printf("dest_path : %s\n",dest_path);
    copy_file(src_path,dest_path);
    return 0;
}