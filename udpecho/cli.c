/*
 * a simple udp echo client program
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>

static void usage()
{
    printf("usage:./cli 192.168.0.33 8080 \"hello\"\n");
}

int main(int argc, char *argv[])
{
    struct sockaddr_in serv_addr;
    socklen_t servaddr_len;
    char buf[BUFSIZ];
    int sockfd, n, ret;

    if (argc < 4) {
        fprintf(stderr, "You mast enter 3 parameters.\n");
        usage();
        exit(-1);
    }

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd == -1) {
        perror("socket() error");
        exit(-1);
    }

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    ret = inet_pton(AF_INET, argv[1], &serv_addr.sin_addr.s_addr);
    if (ret == -1) {
        perror("inet_pton() error");
        exit(-1);
    }
    serv_addr.sin_port = htons(atoi(argv[2]));

    n = sendto(sockfd, argv[3], strlen(argv[3])+1, 0, 
               (struct sockaddr*)&serv_addr, sizeof(serv_addr));
    if (n == -1) {
        perror("sendto() error");
        exit(-1);
    }

    servaddr_len = sizeof(serv_addr);
    n = recvfrom(sockfd, buf, sizeof(buf), 0, (struct sockaddr*)&serv_addr, 
                 &servaddr_len);
    if (n == -1) {
        perror("recvfrom() error");
        exit(-1);
    }
    printf("Recieve from server[%s]: %s \n", inet_ntoa(serv_addr.sin_addr), buf);

    return 0;
}