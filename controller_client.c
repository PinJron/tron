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

u_int get_local_ip() {
    FILE *f;
    char line[100], *p, *c;

    f = fopen("/proc/net/route", "r");

    while (fgets(line, 100, f)) {
        p = strtok(line, " \t");
        c = strtok(NULL, " \t");

        if (p != NULL && c != NULL) {
            if (strcmp(c, "00000000") == 0) {
                // printf("Default interface is : %s \n", p);
                break;
            }
        }
    }

  // which family do we require , AF_INET or AF_INET6
  int fm = AF_INET;
  struct ifaddrs *ifaddr, *ifa;
  int family, s;
  char host[NI_MAXHOST];

  if (getifaddrs(&ifaddr) == -1) {
    perror("getifaddrs");
    exit(EXIT_FAILURE);
  }

  // Walk through linked list, maintaining head pointer so we can free list
  // later
  for (ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next) {
    if (ifa->ifa_addr == NULL) {
      continue;
    }

    family = ifa->ifa_addr->sa_family;

    if (strcmp(ifa->ifa_name, p) == 0) {
      if (family == fm) {
        s = getnameinfo(ifa->ifa_addr,
                        (family == AF_INET) ? sizeof(struct sockaddr_in)
                                            : sizeof(struct sockaddr_in6),
                        host, NI_MAXHOST, NULL, 0, NI_NUMERICHOST);

        if (s != 0) {
        //   printf("getnameinfo() failed: %s\n", gai_strerror(s));
          exit(EXIT_FAILURE);
        }

        // printf("address: %s", host);
      }
    //   printf("\n");
    }
  }

  freeifaddrs(ifaddr);

  return inet_addr( host );
}
