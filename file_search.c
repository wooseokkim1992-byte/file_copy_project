#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <string.h>
#include "file_search.h"

#define MAX_FILE_PATH 64
#define BUF_SIZE 4096

void list_directory(const char *dir_path){
    DIR *dir = opendir(dir_path);
    if(!dir){
        perror("open dir failed!");
        return ;
    }
    struct dirent *entry;
    while((entry=readdir(dir))!=NULL){
        if(strcmp(entry->d_name,".")||strcmp(entry->d_name,"..")){
            continue;
        }
        printf("inode:%d  name:%s type:%s\n",entry->d_ino,entry->d_name,entry->d_type);
    }
    closedir(dir);
}


unsigned short search_and_open_dir(const char *src_dir_name,const char *dst_dir_name,const char *filename){
    DIR *src_dir = opendir(src_dir_name);
    if(src_dir==NULL){
        perror("failed to open source directory!");
        return -1;
    }
    DIR *dst_dir = opendir(dst_dir_name);
    if(dst_dir==NULL){
        perror("failed to open source directory!");
        closedir(src_dir);
        return -1;
    }

    struct dirent *entry;
    unsigned short flag = 0;
    while((entry=readdir(src_dir))!=NULL){
        size_t len = sizeof(entry->d_name)+1;
        if(!strncmp(filename,entry->d_name,len)){
            flag = 1;
            break;
        }
    }
    closedir(dst_dir);
    closedir(src_dir);
    if(!flag){
        fprintf(stderr,"file name (%s) doesn't exist\n",filename);
        return -1;
    }
    return 0;
}

int copy_file(const char* src,const char* dest){
    int in_fd = open(src,O_RDONLY);
    if(in_fd<0){
        printf("open src file failed\n");
        perror("open src file failed");
        return -1;
    }
    int out_fd = open(dest, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if(out_fd<0){
        perror("open dest file failed\n");
        close(in_fd);
        return -1;
    }
    char buf[BUF_SIZE];
    ssize_t n;
    printf("src file fd : %d\n",in_fd);
    printf("dest file fd : %d\n",out_fd);
    while((n=read(in_fd,buf,sizeof(buf)))>0){
        ssize_t total=0;
        printf("%zd was read to fd : %d\n",n,in_fd);

        while(total<n){
            ssize_t written_size = write(out_fd,buf+total,n-total);
            printf("%zd written to fd : %d\n",written_size,out_fd);
            if(written_size<=0){
                perror("write failed\n");
                close(in_fd);
                close(out_fd);
                return -1;
            }
            total+=written_size;
        }
    }
    if(n<0){
        perror("read failed\n");
    }
    close(in_fd);
    close(out_fd);
    return 0;
}

char* reconstruct_path_1(const char* dir_path,const char* file_name){
    char *final_path;
    size_t len = strlen(dir_path)+strlen(file_name)+2;
    final_path=malloc(len);
    snprintf(final_path,MAX_FILE_PATH,"%s/%s",dir_path,file_name);
    return final_path;
}

