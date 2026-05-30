#ifndef TCP_FILE_COMMON_H
#define TCP_FILE_COMMON_H

#include <stdint.h>
#include <unistd.h>
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
#include <limits.h>
#define TCP_FILE_BUF_SIZE 4096
#define TCP_FILE_NAME_MAX 255
#define BUF_SIZE 4096
#define MAX_PORT_NUM 65535

ssize_t send_data_byte_to_other_side(int sock_fd,void *chunk,ssize_t len);
ssize_t revc_data_byte_from_other_side(int sock_fd,void *chunk,ssize_t len);
int send_file_name(int sock_fd,char *filename);
int receive_file_name(int client_sock_fd,char *buf);
int get_port_num(const char *port_str,uint16_t *ptr);
int check_ip_regex(const char *ip);
int append_file_name(char*path,size_t path_cap);
int list_up_file_and_dirs(char *path);
int set_connection_with_server(
    int *sock_fd,const char*server_ip,
    uint16_t port
);
int send_file_size(const int sock_fd,const char *file_path);
uint64_t receive_file_size(const int sock_fd);

static inline uint64_t tcp_htonll(uint64_t value)
{
    uint64_t result = 0;
    unsigned char *dst = (unsigned char*)&result;

    dst[0] = (unsigned char)((value>>56)&0xff);
    dst[1] = (unsigned char)((value>>48)&0xff);
    dst[2] = (unsigned char)((value>>40)&0xff);
    dst[3] = (unsigned char)((value>>32)&0xff);
    dst[4] = (unsigned char)((value>>24)&0xff);
    dst[5] = (unsigned char)((value>>16)&0xff);
    dst[6] = (unsigned char)((value>>8)&0xff);
    dst[7] = (unsigned char)((value>>0)&0xff);
    return result;
}

static inline uint64_t tcp_ntohll(uint64_t value){
    const unsigned char *src = (const unsigned char*)&value;
    return ((uint64_t)src[0]<<56) | 
        ((uint64_t)src[1]<<48) |
        ((uint64_t)src[2]<<40) |
        ((uint64_t)src[3]<<32) |
        ((uint64_t)src[4]<<24) |
        ((uint64_t)src[5]<<16) |
        ((uint64_t)src[6]<<8) |
        (uint64_t)src[7];
}

#endif
