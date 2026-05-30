#include <sys/socket.h>
#include <sys/stat.h>
#include <unistd.h>
#include "socket_common.h"
#include "file_search.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <regex.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <stddef.h>
#include <stdint.h>

ssize_t revc_data_byte_from_other_side(int sock_fd,void *chunk,ssize_t len){
    ssize_t total=0;
    char *buf = chunk;
    while(total<len){
        ssize_t recv_size = recv(sock_fd,buf+total,len-total,0);
        if(recv_size<=0){
            return  -1;
        }
        total+=recv_size;
    }

    return total;
}

ssize_t send_data_byte_to_other_side(int sock_fd,void *chunk,ssize_t len){
    const char *buf=chunk;
    ssize_t total = 0;
    while(total<len){
        ssize_t sent_size = send(sock_fd, buf+total, len-total, 0);
        if(sent_size<=-1){
            return -1;
        }
        total+=sent_size;
    }
    return total;    
}

int receive_file_name(int client_sock_fd,char *buf){
    uint32_t filename_len;
    uint32_t converted_filename_len;
    if(revc_data_byte_from_other_side(client_sock_fd, &filename_len,sizeof(filename_len))<0){
        perror("failed to reveive filename length");
        return 1;
    }
    converted_filename_len = ntohl(filename_len);
    printf("received file name length : %d\n",converted_filename_len);
    if(revc_data_byte_from_other_side(client_sock_fd, buf, converted_filename_len)<0){
        perror("failed to receive file name");
        return 1;
    }
    if(strlen(buf)+1>=PATH_MAX||converted_filename_len+1>=PATH_MAX){
        perror("file length truncated!");
        return 1;
    }
    buf[converted_filename_len]='\0';
    return 0;
}

int send_file_name(int sock_fd,char *filename){
    uint32_t filename_len = (uint32_t)strlen(filename); 
    uint32_t net_filename_len = htonl(filename_len);
    ssize_t total=0;
    if((total=send_data_byte_to_other_side(sock_fd, & net_filename_len, sizeof(net_filename_len)))<0){
        perror("Failed to send file name length");
        return 1;
    }
    if((total=send_data_byte_to_other_side(sock_fd, filename, filename_len))<0){
        perror("failed to send filename");
        return 1;
    }
    return 0;
}

int send_file_size(const int sock_fd,const char *file_path){
    int file_size=0;
    if((file_size=get_file_size(file_path))<0){
        return 1;
    }
    printf("file size : %d  ",file_size);
    uint64_t net_file_size = tcp_htonll(file_size);
    printf("net file size : %lld\n",net_file_size);
    if(send_data_byte_to_other_side(sock_fd,&net_file_size, sizeof(net_file_size))<0){
        return -1;
    }
    return 0;
}

uint64_t receive_file_size(const int sock_fd){
    uint64_t file_size;
    if(revc_data_byte_from_other_side(sock_fd, &file_size, sizeof(file_size))<0){
        return -1;
    }
    return tcp_ntohll(file_size);
}



int get_port_num(const char *port_str,uint16_t *ptr){
    int port = atoi(port_str);
    printf("port : %d\n",port);
    if(port<=0||port>MAX_PORT_NUM){
        fprintf(stderr,"port number shoud be between 0 and %d",MAX_PORT_NUM);
        return -1;
    }
    *ptr=htons((uint16_t)port);
    return 0;
}

int check_ip_regex(const char *ip){
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

int append_file_name(char*path,size_t path_cap){
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


int list_up_file_and_dirs(char *path){
    if(getcwd(path,PATH_MAX)==NULL){
        fprintf(stderr,"Failed to get current directory");
        return 1;
    }
    printf("directory now at : %s",path);
    while(1){
        list_directory(path);
        if(append_file_name(path,PATH_MAX)!=0){
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

int set_connection_with_server(
    int *sock_fd,const char*server_ip,
    uint16_t port
){

    *sock_fd=socket(AF_INET,SOCK_STREAM,0);
    if(*sock_fd<0){
        perror("Failed to initialize socket!");
        return 1;
    }
    struct sockaddr_in server_addr;
    memset(&server_addr,0,sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port=port;
    if(inet_pton(AF_INET, server_ip, &server_addr.sin_addr)<=0){
        perror("Failed to transfer ip into bytes.");
        close(*sock_fd);
        return 1;
    }
    if(connect(*sock_fd, (struct sockaddr *)&server_addr, sizeof(server_addr))<0){
        perror("Failed to connect to server!");
        close(*sock_fd);
        return 1;
    }
    return 0;
}
