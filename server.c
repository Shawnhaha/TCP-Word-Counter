/*
TCP socket (server)
Author: Shangda Li
*/
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <netinet/in.h>
#include <string.h>

#define MAX 200
//#define PORT 4096

// Function for communication between client and server.
void func(int sockfd){
    char buff[MAX];
    char num[MAX];

    while (1) {
        bzero(buff, MAX);
        int wordNum = 0;
        int alphaNum = 0;
        int vowelNum = 0;
        read(sockfd, buff, sizeof(buff));     // read from client
        printf("input: %s\n", buff);    // print out

        // if the input is "quit", end server
        if (memcmp(buff, "quit", 4)==0) {
            exit(0);
        }

        for(int i=0; buff[i]!='\0'; i++) {
         //check first character is lowercase alphabet
         if(i == 0 && buff[i] != ' ') {
           wordNum++;
           if((buff[i] >= 'a' && buff[i] <= 'z')) {
             buff[i] = buff[i] - 32;  // lowercase to uppercase
           }
           continue;  //To the next iter
         }
         if(buff[i] == ' ') {   //check space
           i++;   //To the next iter
           wordNum++;
           //check if the next character is lowercase
           if(buff[i] >= 'a' && buff[i] <= 'z') {
             buff[i] = buff[i] - 32;  //lowercase to uppercase
           }
         }
         else {      //all other uppercase to lowercase
           if(buff[i] >= 'A' && buff[i] <= 'Z')
           buff[i] = buff[i] + 32;
         }
        }
        // send that buffer to client
        write(sockfd, buff, sizeof(buff));

        for (int i=0; buff[i]!='\0'; i++) {
          if ((buff[i] >= 'A' && buff[i] <= 'Z') || (buff[i] >= 'a' && buff[i] <= 'z')) {  //check alphabet
            alphaNum++;
          }
          if (buff[i]=='A' || buff[i]=='E' || buff[i]=='I' || buff[i]=='O' || buff[i]=='U' || buff[i]=='a' || buff[i]=='e' || buff[i]=='i' || buff[i]=='o' || buff[i]=='u'){
            vowelNum++;
          }
        }

        //send Number of words
        snprintf(num, sizeof(num), "%d", wordNum);
        write(sockfd, num, sizeof(num));
        bzero(num,MAX);

        //send Number of alphabets
        snprintf(num, sizeof(num), "%d", alphaNum);
        write(sockfd, num, sizeof(num));
        bzero(num,MAX);

        //send Number of vowels
        snprintf(num, sizeof(num), "%d", vowelNum);
        write(sockfd, num, sizeof(num));
        bzero(num,MAX);

        bzero(buff, MAX);
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
      printf("Please run server with a valid port number\n");
      exit(0);
    }

    // creat socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    bzero(&servaddr, sizeof(servaddr));

    // assign IP, PORT
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(PORT);

    // Bind socket
    bind(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr));

    //listen to incoming connections
    listen(sockfd, 8);

    // Accept connection
    connfd = accept(sockfd, (struct sockaddr*)NULL, NULL);

    func(connfd);

    //close the socket
    close(sockfd);
}
