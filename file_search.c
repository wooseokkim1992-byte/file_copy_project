#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>
#include "file_search.h"
#include <limits.h>

int list_directory(const char *dir_path){
    DIR *dir = opendir(dir_path);
    if(!dir){
        perror("open dir failed!");
        return 1;
    }
    struct dirent *entry;
    while((entry=readdir(dir))!=NULL){
        if(strcmp(entry->d_name,".")==0||strcmp(entry->d_name,"..")==0){
            continue;
        }
        printf("inode:%llu  name:%s type:%hhu\n",entry->d_ino,entry->d_name,entry->d_type);
    }
    closedir(dir);
    return 0;
}

void get_input(char *buf,size_t size){
    printf("Enter file or directory name to copy(maximum 256 charactors): ");
    //fgets 는 size-1 만큼 줄 바꾸기 문자(\n)까지 혹은 size-1 과 같을 때 까지 읽고, 
    // buf에 쓴 다음 맨 뒤에는 "\0"을 채워 넣는다.
    fgets(buf,size,stdin);
    // 문자열에서 \n 을 찾아서 그 위치를 \0 으로 바꿔줌!
    buf[strcspn(buf,"\n")]=0;
}

int check_file_or_dir(const char *path){
    struct stat st;
    if(stat(path,&st)<0){
        perror("stat failed");
        return -1;
    }
    if(S_ISDIR(st.st_mode)){
        return 1;
    }
    if(S_ISREG(st.st_mode)){
        return 2;
    }
    return -1;
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

int copy_resursive(const char *src_path,const char *dest_path){
    struct stat st;
    if(stat(src_path,&st)<0){
        perror("stat failed");
        return -1;
    }
    if(S_ISREG(st.st_mode)){
        return copy_file(src_path, dest_path);
    }
    if(S_ISDIR(st.st_mode)){
        if(mkdir(dest_path, 0755)<0){
            perror("mkdir failed");
            return -1;
        }
        DIR *dir = opendir(src_path);
        if(!dir){
            perror("opening src dir failed!");
            return -1;
        }
        struct dirent *entry;
        while((entry=readdir(dir))!=NULL){
            if(strcmp(entry->d_name, ".")==0||strcmp(entry->d_name, "..")==0){
                continue;
            }
            char *src = reconstruct_path_1(src_path, entry->d_name);
            char *dest = reconstruct_path_1(dest_path, entry->d_name);
            printf("src : %s\n",src);
            printf("dest : %s\n",dest);
            copy_resursive(src, dest);
            free(src);
            free(dest);
        }
        closedir(dir);
    }
    return 0;
}

int move_file(const char *src_path,const char *dest_path){
    int file_type = check_file_or_dir(src_path);
    if(file_type==1){
        copy_resursive(src_path, dest_path);
    }else if(file_type==2){
        copy_file(src_path, dest_path);
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
        while(total<n){
            ssize_t written_size = write(out_fd,buf+total,n-total);
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
    snprintf(final_path,PATH_MAX,"%s/%s",dir_path,file_name);
    return final_path;
}

