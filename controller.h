#ifndef __CONTROLLER_H__
#define __CONTROLLER_H__

#include <pthread.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <curses.h>

#include <arpa/inet.h>
#include <bits/pthreadtypes.h>
#include <pthread.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h> //errno
#include <ifaddrs.h>
#include <netdb.h>
#include <string.h> //memset
#include <sys/socket.h>

#include <linux/fb.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <fcntl.h>

#include <stdint.h>

struct network
{
    struct sockaddr_in enemy_addr;
    struct sockaddr_in my_addr;
    int nd;
};

struct length
{
    unsigned int x[40];
    unsigned int y[40];
};


struct tron
{
    struct length* tron;
    int* key;
    int* key_prev;
    unsigned int* tail_x;
    unsigned int* tail_y;
    pthread_mutex_t mutex;
};

struct server_data
{
    struct network *net;
    struct tron* my_tron;
    void* (*manage_my_tron)(void *);
    struct tron* enemy_tron;
    void* (*manage_enemy_tron)(void *);
    uint8_t* work_flag;
};

void create_socket(in_addr_t enemy_ip_addr, in_addr_t my_ip_addr, int port, struct network *net);

u_int get_local_ip();

#endif
