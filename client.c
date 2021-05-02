/*
TCP socket (client)
Author: Shangda Li
*/
#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <sys/socket.h>
#include <string.h>

#define MAX 200
//#define PORT 4096

void func(int sockfd){
    char buff[MAX];
    char num[MAX];

    while (1) {
        bzero(buff, sizeof(buff));   //clear buff
        bzero(num, sizeof(num));   //clear num

        printf("input: ");
        int n = 0;
        while ((buff[n++] = getchar()) != '\n');   //get input from user
        write(sockfd, buff, sizeof(buff));

        // if the input is "quit", end client
        if (memcmp(buff, "quit", 4)==0) {
            printf("Cya!\n");
            exit(0);
        }

        bzero(buff, sizeof(buff));
        read(sockfd, buff, sizeof(buff));
        printf("output: %s", buff);


        read(sockfd, num, sizeof(num));
        printf("words: %s\n", num);
        bzero(num, sizeof(num));

        read(sockfd, num, sizeof(num));
        printf("alphabets: %s\n", num);
        bzero(num, sizeof(num));

        read(sockfd, num, sizeof(num));
        printf("vowels: %s\n", num);
        bzero(num, sizeof(num));
    }
}

int main(int argc, char *argv[]) {
    int sockfd, connfd;
    struct sockaddr_in servaddr;
    int PORT;

    if(argc == 2) {
      PORT = atoi(argv[1]);
    }
    else {
      printf("Please run client with a valid port number\n");
      exit(0);
    }

    // socket create and varification
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    bzero(&servaddr, sizeof(servaddr));

    // assign IP, PORT
    servaddr.sin_family = AF_INET;
    //servaddr.sin_addr.s_addr = INADDR_ANY;
    /* Convert IPv4 and IPv6 addresses from text to binary form */
    inet_pton(AF_INET,"129.120.151.94",&(servaddr.sin_addr));
    servaddr.sin_port = htons(PORT);

    // connect the client socket to server socket
    connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr));

    func(sockfd);

    // close the socket
    close(sockfd);
  }
