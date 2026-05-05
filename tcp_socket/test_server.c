// server.c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define PORT 9001
#define BUF_SIZE 4096

int main(int argc,char **argv){
    int server_fd = 0;
    if((server_fd=socket(AF_INET, SOCK_STREAM, 0))<0){
        perror("socket failed");
        return 1;
    }

    struct sockaddr_in addr;
    memset(&addr,0,sizeof(addr));
    addr.sin_family=AF_INET;
    addr.sin_port = htons(PORT);
    addr.sin_addr.s_addr = INADDR_ANY;
    socklen_t len = sizeof(addr);

    if(bind(server_fd,(struct sockaddr*)&addr,sizeof(addr))<0){
        perror("bind failed");
        return 1;
    }
    if(getsockname(server_fd, (struct sockaddr *)&addr, &len)<0){
        perror("failed to get socket info");
    }else{
        char ip[INET_ADDRSTRLEN];
        inet_ntop(AF_INET,&addr.sin_addr,ip,sizeof(ip));
        printf("Server listening on %s:%d\n",
           ip,
           ntohs(addr.sin_port));
    }

    if(listen(server_fd,5)<0){
        perror("listen failed");
        return 1;
    }

    printf("Server listening on port %d...\n",PORT);

    int client_fd = accept(server_fd,NULL,NULL);
    if(client_fd<0){
        perror("accept failed");
        return 1;
    }

    u_int32_t net_len = 0;
    recv(client_fd,&net_len,sizeof(net_len),0);
    uint32_t name_len = ntohl(net_len);
    printf("received : name len : %d\n",name_len);
    char filename[256]={0};
    recv(client_fd, filename, name_len, 0);
    printf("received : filename : %s\n",filename);

    printf("Receiving file: %s\n", filename);

    close(client_fd);
    close(server_fd);
    return 0;
}