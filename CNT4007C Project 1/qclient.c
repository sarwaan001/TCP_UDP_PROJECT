//
//  qclient.cpp
//  CNT4007C Project 1
//
//  Created by Sarwaan Ansari on 2/5/19.
//  Copyright © 2019 Sarwaan Ansari. All rights reserved.
//

#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#define SA struct sockaddr

int MAX = 80;
long PORT = 8080;

void getquesinterface(int sockfd) {
    char buff[MAX];
    ssize_t bytesread;
    bytesread = read(sockfd, buff, sizeof(buff));
    
    
    while (bytesread >= MAX ) {
        if(bytesread < sizeof(buff)) {
            buff[bytesread-1] = '\0';
        }
        else {
            buff[sizeof(buff)-1] = '\0';
        }
        printf("%s", buff);
        bytesread = read(sockfd, buff, sizeof(buff));
    }
    buff[bytesread] = '\0';
    printf("%s", buff);
    
}

void questioninterface(int sockfd) {
    char buff[MAX];
    int n;
    for (;;) {
        bzero(buff, sizeof(buff));
        printf("> ");
        n = 0;
        for (;;) {
            if ((buff[n++] = getchar()) == '\n') {
                if (n == 80) {
                    write(sockfd, buff, n);
                    bzero(buff, sizeof(buff));
                    n = 0;
                }
                if ((buff[n++] = getchar()) == '.') {
                    if (n == 80) {
                        write(sockfd, buff, n);
                        bzero(buff, sizeof(buff));
                        n = 0;
                    }
                    if ((buff[n++] = getchar()) == '\n') {
                        if (n == 80) {
                            write(sockfd, buff, n);
                            bzero(buff, sizeof(buff));
                            n = 0;
                        }
                        break;
                    }
                }
            }
            if (n == 80) {
                write(sockfd, buff, n);
                bzero(buff, sizeof(buff));
                n = 0;
            }
        }
        buff[n++] = '\0';
        write(sockfd, buff, n);
        bzero(buff, sizeof(buff));
        break;
    }
}

void choicesinterface(int sockfd) {
    char buff[MAX];
    int n;
    for (;;) {
        bzero(buff, sizeof(buff));
        printf("> ");
        n = 0;
        for (;;) {
            if ((buff[n++] = getchar()) == '\n') {
                if (n == 80) {
                    write(sockfd, buff, n);
                    bzero(buff, sizeof(buff));
                    n = 0;
                }
                if ((buff[n++] = getchar()) == '.') {
                    if (n == 80) {
                        write(sockfd, buff, n);
                        bzero(buff, sizeof(buff));
                        n = 0;
                    }
                    if ((buff[n++] = getchar()) == '\n') {
                        if (n == 80) {
                            write(sockfd, buff, n);
                            bzero(buff, sizeof(buff));
                            n = 0;
                        }
                        if ((buff[n++] = getchar()) == '.') {
                            if (n == 80) {
                                write(sockfd, buff, n);
                                bzero(buff, sizeof(buff));
                                n = 0;
                            }
                            if ((buff[n++] = getchar()) == '\n') {
                                if (n == 80) {
                                    write(sockfd, buff, n);
                                    bzero(buff, sizeof(buff));
                                    n = 0;
                                }
                                break;
                                
                            }
                        }
                    }
                }
            }
            if (n == 80) {
                write(sockfd, buff, n);
                bzero(buff, sizeof(buff));
                n = 0;
            }
        }
        buff[n++] = '\0';
        write(sockfd, buff, n);
        bzero(buff, sizeof(buff));
        break;
    }
}

void printfull(int sockfd) {
    char buff[MAX];
    bzero(buff, sizeof(buff));
    printf("From Server: ");
    ssize_t bytesread;
    bytesread = read(sockfd, buff, sizeof(buff));
    
    if ((strncmp(buff, "Please Enter Question: ", 23)) == 0) {
        printf("%s", buff);
        bzero(buff, sizeof(buff));
        bytesread = 0;
        questioninterface(sockfd);
        bytesread = read(sockfd, buff, sizeof(buff));
    }
    
    if ((strncmp(buff, "Please Enter Answer Choices: ", 29)) == 0) {
        printf("%s", buff);
        bzero(buff, sizeof(buff));
        bytesread = 0;
        choicesinterface(sockfd);
        bytesread = read(sockfd, buff, sizeof(buff));
    }
    
    if ((strncmp(buff, "Retrieving Question: ", 21)) == 0) {
        printf("%s", buff);
        bzero(buff, sizeof(buff));
        bytesread = 0;
        getquesinterface(sockfd);
        //bytesread = read(sockfd, buff, sizeof(buff));
        return;
    }
    
    if ((strncmp(buff, "Retrieving Random Question: ", 28)) == 0) {
        printf("%s", buff);
        bzero(buff, sizeof(buff));
        bytesread = 0;
        getquesinterface(sockfd);
        //bytesread = read(sockfd, buff, sizeof(buff));
        return;
    }
    
    while (bytesread >= MAX ) {
        if(bytesread < sizeof(buff)) {
            buff[bytesread] = '\0';
        }
        else {
            buff[sizeof(buff)] = '\0';
        }
        printf("%s", buff);
        bytesread = read(sockfd, buff, sizeof(buff));
    }
    printf("%s", buff);
}


void func(int sockfd) {
    char buff[MAX];
    int n;
    for (;;) {
        bzero(buff, sizeof(buff));
        printf("\n> ");
        n = 0;
        while ((buff[n++] = getchar()) != '\n') {
            if (n == 80) {
                write(sockfd, buff, n);
                bzero(buff, sizeof(buff));
                n = 0;
            }
        }
        buff[n++] = '\0';
        
        char scanholder[MAX];
        
        sscanf(buff, "%s", scanholder);
        if (strlen(scanholder) <= 1) {
            if (strncmp(buff, "q", sizeof("q")-1) == 0) {
                printf("Client Exit...\n");
                write(sockfd, buff, n);
                break;
            }
        
            if (strncmp(buff, "k", sizeof("k")-1) == 0) {
                write(sockfd, buff, n);
                printf("Server Exit...\n");
                break;
            }
        }
        write(sockfd, buff, n);
        
        
        bzero(buff, sizeof(buff));
        //read(sockfd, buff, sizeof(buff));
        printfull(sockfd);
        //printf("From Server : %s", buff);
    }
}

void checkentry(struct hostent * entry) {
    if (entry == NULL) {
        perror("checkentry");
        exit(1);
    }
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        perror("Insert Hostname and Port");
        exit(0);
    }
    char *p;
    PORT = strtol(argv[2], &p, 10);
    printf("%d", PORT);
    
    int sockfd, connfd;
    
    char* hostname = argv[1];
    
    struct hostent *host_info;
    host_info = gethostbyname(hostname);
    checkentry(host_info);
    
    struct in_addr **addr_list;
    addr_list = (struct in_addr **)host_info->h_addr_list;
    
    char ip[256];
    strcpy(ip, inet_ntoa(*addr_list[0]));
    
    struct sockaddr_in servaddr, cli;
    //Create socket using TCP (SOCK_SREAM)
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) { //sockfd is -1 if it failed
        printf("socket creation failed...\n");
        exit(0);
    }
    else {
        printf("Socket successfully created..\n");
    }
    
    //Initializing a zero struct
    bzero(&servaddr, sizeof(servaddr));
    
    //Create an ip address and port.
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr(ip);
    servaddr.sin_port = htons(PORT);
    
    // connect the client socket to server socket
    if (connect(sockfd, (SA*)&servaddr, sizeof(servaddr)) != 0) {
        printf("connection with the server failed...\n");
        exit(0);
    }
    else {
        printf("connected to the server..\n");
        //Initial Message
        printf("Reading initial message..\n");
        
        printfull(sockfd);
    }
    
    // function for chat
    func(sockfd);
    
    // close the socket
    close(sockfd);
}
