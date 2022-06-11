#include <arpa/inet.h>
#include <stdint.h>
#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <ncurses.h>

#include "controller.h"

void create_socket(in_addr_t enemy_ip_addr, in_addr_t my_ip_addr, int port, struct network *net) {
    net->enemy_addr.sin_family = AF_INET;
    net->enemy_addr.sin_port = htons(port);
    net->enemy_addr.sin_addr.s_addr = enemy_ip_addr;

    net->my_addr.sin_family = AF_INET;
    net->my_addr.sin_port = htons(port);
    net->my_addr.sin_addr.s_addr = my_ip_addr;
    fprintf(stderr, "%u\n", my_ip_addr);

    net->nd = socket(AF_INET, SOCK_DGRAM, 0);
    if (net->nd == -1) {
        close(net->nd);
    }
}