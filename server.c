#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <unistd.h>
#include "socket_common.h"


int main(int argc, char **argv){
    if(argc<2){
        fprintf(stderr,"Usage %s : <port#>",argv[0]);
        return 1;
    }
    uint16_t PORT = atoi(argv[1]);
    if(PORT<=0||PORT>MAX_PORT_NUM){
        fprintf(stderr,"port number shoud be between 0 and %d",MAX_PORT_NUM);
        return 1;
    }
    
    ssize_t server_fd;
    ssize_t client_fd;
    if((server_fd=socket(AF_INET,SOCK_STREAM,0))<0){
        perror("failed to initialize socket \n");
        return 1;
    }
    struct sockaddr_in addr;
    memset(&addr,0,sizeof(addr));
    addr.sin_family=AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(PORT);
    printf("PORT : %d\n",PORT);
;    socklen_t len = sizeof(addr);
    if(bind(server_fd,(struct sockaddr*)&addr,len)<0){
        perror("failed to bind socket with addr struct");
        return 1;
    }
    if(getsockname(server_fd, (struct sockaddr *)&addr, &len)<0){
        perror("failed to get socket info");
    }else{
        char ip[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, (struct sockaddr_in*)&addr.sin_addr,ip, sizeof(ip));
        printf("Server listening on %s:%d\n",
           ip,
           ntohs(addr.sin_port));
    }
    if(listen(server_fd, 5)<0){
        perror("failed to listen");
        close(server_fd);
        return 1;
    }
    if((client_fd = accept(server_fd,NULL,NULL))<0){
        perror("failed to accetp client");
        close(server_fd);
        return 1;
    }
    close(client_fd);
    close(server_fd);
    return 0;
}