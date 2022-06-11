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


u_int get_local_ip()
{
    FILE *f;
    char line[100], *p, *c;

    f = fopen("/proc/net/route", "r");

    while (fgets(line, 100, f))
    {
        p = strtok(line, " \t");
        c = strtok(NULL, " \t");

        if (p != NULL && c != NULL)
        {
            if (strcmp(c, "00000000") == 0)
            {
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

    if (getifaddrs(&ifaddr) == -1)
    {
        perror("getifaddrs");
        exit(EXIT_FAILURE);
    }

    // Walk through linked list, maintaining head pointer so we can free list
    // later
    for (ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next)
    {
        if (ifa->ifa_addr == NULL)
        {
            continue;
        }

        family = ifa->ifa_addr->sa_family;

        if (strcmp(ifa->ifa_name, p) == 0)
        {
            if (family == fm)
            {
                s = getnameinfo(ifa->ifa_addr,
                                (family == AF_INET) ? sizeof(struct sockaddr_in)
                                                    : sizeof(struct sockaddr_in6),
                                host, NI_MAXHOST, NULL, 0, NI_NUMERICHOST);

                if (s != 0)
                {
                    //   printf("getnameinfo() failed: %s\n", gai_strerror(s));
                    exit(EXIT_FAILURE);
                }

                printf("address: %s", host);
            }
            printf("\n");
        }
    }

    freeifaddrs(ifaddr);

    printf("ntohl(get_local_ip()) = %lu\n", ntohl(inet_addr(host)));
    return inet_addr(host);
}

int main()
{
    get_local_ip();
}
