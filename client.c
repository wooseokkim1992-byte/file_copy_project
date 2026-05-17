#include <stdio.h>
#include <stdlib.h>
#include "socket_common.h"

int main(int argc,char **argv){
    if(argc<3){
        fprintf(stderr,"Usage : %s <ip> <port>",argv[0]);
        return 1;
    }
    uint16_t PORT;
    const char *IP=argv[1];
    if(get_port_num(argv[2],&PORT)<0){
        return 1;
    }
    printf("PORT : %d\n",PORT);
    if(check_ip_regex(IP)<=0){
        fprintf(stderr,"Not proper IPv4 format!");
        return 1;
    }
    printf("ip : %s\n",IP);
    int sock_fd = -1;
    if(set_connection_with_server(&sock_fd, IP, PORT)<0){
        return 1;
    }
    char path[PATH_MAX];
    if(list_up_file_and_dirs(path)!=0){
        close(sock_fd);
        return 1;
    }
    if(send_file_name(sock_fd,path)!=0){
        close(sock_fd);
        return 1;
    }
    close(sock_fd);
    return 0;
}