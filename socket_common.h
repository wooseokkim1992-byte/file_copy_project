#ifndef TCP_FILE_COMMON_H
#define TCP_FILE_COMMON_H

#include <stdint.h>
#include <unistd.h>

#define TCP_FILE_BUF_SIZE 4096
#define TCP_FILE_NAME_MAX 255
#define BUF_SIZE 4096
#define MAX_PORT_NUM 65535

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
