/*
 * a simple udp echo server program
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>

#define MAXLINE 2048

static void usage()
{
    printf("usage:./srv 8080\n");
}


int main(int argc, char *argv[])
{
    struct sockaddr_in serv_addr, cli_addr;
    socklen_t cliaddr_len;
    char buf[MAXLINE];
    int sockfd, n;

    if (argc < 2) {
        fprintf(stderr, "You mast enter 1 parameters.\n");
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
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(atoi(argv[1]));
    
    if (bind(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1) {
        perror("bind() error");
        exit(-1);
    }

    printf("Accepting connections ...\n");
    while (1) {
        cliaddr_len = sizeof(cli_addr);
        n = recvfrom(sockfd, buf, sizeof(buf), 0, (struct sockaddr*)&cli_addr, 
                     &cliaddr_len);
        if (n == -1) {
            perror("recvfrom error");
            exit(-1);
        } else if (n == MAXLINE) {
            fprintf(stderr, "Out of buffer size[%d].\n", MAXLINE);
            continue;
        }
        printf("Recieve from client[%s]: %s \n", inet_ntoa(cli_addr.sin_addr), buf);

        n = sendto(sockfd, buf, sizeof(buf), 0, (struct sockaddr*)&cli_addr, 
                   sizeof(cli_addr));
        if (n == -1) {
            perror("sendto error");
            exit(-1);
        }
    }

    return 0;
}