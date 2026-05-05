#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/stat.h>

#define PORT 9001
#define BUF_SIZE 4096

int main(int argc, char **argv){
    if (argc < 3) {
        fprintf(stderr, "Usage: %s <server_ip> <file>\n", argv[0]);
        return 1;
    }

    const char *server_ip = argv[1];
    const char *filename = argv[2];

    int sock = socket(AF_INET,SOCK_STREAM,0);
    if(sock<0){
        perror("socket failed");
        return 1;
    }

    struct sockaddr_in addr;
    memset(&addr,0,sizeof(addr));
    
    addr.sin_family = AF_INET;
    // 주소체계 ipv4 : AF_INET
    // ipv6 : AF_INET6
    addr.sin_port = htons(PORT); 
    //server_ip 주소를 ipv4 형태로 addr.sin_addr 에 복사
    if(inet_pton(AF_INET, server_ip, &addr.sin_addr)<=0){
        perror("failed to convert proper ip address");
        return 1;
    }
    if(connect(sock,(struct sockaddr*)&addr,sizeof(addr))<0){
        perror("failed to connect\n");
        return 1;
    }

    uint32_t name_len = strlen(filename);
    // 네트워크 바이트 오더 변환 (중요)
    uint32_t net_len = htonl(name_len);

    send(sock, &net_len, sizeof(net_len), 0);
    send(sock, filename, name_len, 0);

    close(sock);
    return 0;
}
