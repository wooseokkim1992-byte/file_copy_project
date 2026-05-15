#include <arpa/inet.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <unistd.h>
#include <regex.h>
#include "socket_common.h"
#include "file_search.h"

unsigned short get_port_num(const char *port_str,ssize_t *ptr){
    ssize_t port = atoi(port_str);
    if(port<=0||port>MAX_PORT_NUM){
        fprintf(stderr,"port number shoud be between 0 and %d",MAX_PORT_NUM);
        return -1;
    }
    *ptr=port;
    return 0;
}

unsigned short check_ip_regex(const char *ip){
    regex_t reg_ex;
    const char *pattern =
        "^("
        "(25[0-5]|"        // 250-255
        "2[0-4][0-9]|"    // 200-249
        "1[0-9]{2}|"      // 100-199
        "[1-9]?[0-9])"    // 0-99
        "\\.){3}"
        "(25[0-5]|"
        "2[0-4][0-9]|"
        "1[0-9]{2}|"
        "[1-9]?[0-9])$";
    if(regcomp(&reg_ex, pattern, REG_EXTENDED)!=0){
        fprintf(stderr,"Not proper IPv4 format!");
        return 1;
    }
    int result = regexec(&reg_ex, ip, 0, NULL, 0);
    return result==0;
}

unsigned short append_file_name(char*path,size_t path_cap){
    printf("\nselect file name to download\n");
    char buf[FILENAME_MAX];
    size_t path_len = strlen(path);
    if(fgets(buf,FILENAME_MAX, stdin)==NULL){
        fprintf(stderr,"error occurred during typing filename");
        return 1;
    }
    buf[strcspn(buf,"\n")]=0;
    size_t buf_len = strlen(buf);
    printf("selected file name : %s\n",buf);

    if(path_len+1+buf_len+1>=path_cap){
        fprintf(stderr,"file path truncated!");
        return 1;
    }
    path[path_len]='/';
    memcpy(path+path_len+1,buf,buf_len+1);
    return 0;
}


unsigned short list_up_file_and_dirs(){
    char path[MAX_FILE_PATH];
    if(getcwd(path,MAX_FILE_PATH)==NULL){
        fprintf(stderr,"Failed to get current directory");
        return 1;
    }
    printf("directory now at : %s",path);
    while(1){
        list_directory(path);
        if(append_file_name(path,MAX_FILE_PATH)!=0){
            return 1;
        }
        const char *file_type=check_file_or_dir(path);
        if(file_type==NULL){
            return 1;
        }
        if(strcmp(file_type,filetype_str[DT_DIR])==0){
            continue;
        }else if(strcmp(file_type,filetype_str[DT_REG])==0){
            break;
        }
    }
    printf("new path : %s\n",path);
    return 0;
}


int main(int argc,char **argv){
    if(argc<3){
        fprintf(stderr,"Usage : %s <ip> <port>",argv[0]);
        return 1;
    }
    ssize_t PORT;
    const char *IP=argv[1];
    if((get_port_num(argv[2],&PORT))<0){
        return 1;
    }
    printf("PORT : %ld\n",PORT);
    if(check_ip_regex(IP)<=0){
        fprintf(stderr,"Not proper IPv4 format!");
        return 1;
    }
    printf("ip : %s\n",IP);
    if(list_up_file_and_dirs()!=0){
        return 1;
    }
    return 0;
}