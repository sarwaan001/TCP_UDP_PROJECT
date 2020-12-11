//
//  main.cpp
//  CNT4007C Project 1
//
//  Created by Sarwaan Ansari on 2/5/19.
//  Copyright © 2019 Sarwaan Ansari. All rights reserved.
//

#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#define SA struct sockaddr

int numquestions = 0;

int MAX = 80;
int PORT = 8080;
int forrand = 0;

void getnumquestions() {
    FILE *fp;
    fp = fopen("questions.txt", "r");
    if (fp == NULL) {
        numquestions = 0;
        return;
    }
    char *line = NULL;
    size_t len = 0;
    ssize_t read;
    int tempcount = 0;
    
    //Variables to return
    int tempnum;
    
    while ((read = getline(&line, &len, fp)) != -1) {
        char readtemp[read];
        readtemp[0] = '\0';
        for (int i = 0; i < read; i++) {
            if ((line[i] < 127) && (line[i] > 1)) {
                char ctostr[2];
                ctostr[1] = '\0';
                ctostr[0] = line[i];
                strcat(readtemp, ctostr);
            }
        }
        if ((strncmp(readtemp, "<st>", 4)) == 0) {
            sscanf(readtemp, "<st> %d", &tempnum);
            if (tempnum > numquestions) {
                tempcount = 1;
                numquestions = tempnum; //ensures that the next question will be the largest number
            }
        }
    }
    if (tempcount == 0) {
        numquestions = 0;
    }
    printf("%d", numquestions);
}

void randques(int sockfd);

void writemessage(int sockfd, char* string) {
    write(sockfd, string, strlen(string));
}

void stripString(char* string) {
    const char* StartString = string;
    while (*StartString != '\0' && isspace(*StartString)) {
        ++StartString;
    }
    size_t len = strlen(StartString)+1;
    memmove(string, StartString, len);
    
    char* endOfString = string + len;
    while (string < endOfString  && isspace(*endOfString)){
        --endOfString ;
    }
    *endOfString = '\0';
}

void StripPrintString(char* string) {
    const char* StartString = string;
    while (isprint(*StartString) == 0) {
        ++StartString;
    }
    size_t len = strlen(StartString)+1;
    memmove(string, StartString, len);
    
    char* endOfString = string + len;
    while (string < endOfString  && isspace(*endOfString)){
        --endOfString ;
    }
    *endOfString = '\0';
}

void printfull(int sockfd) {
    char buff[MAX];
    bzero(buff, sizeof(buff));
    printf("From Client: ");
    ssize_t bytesread;
    bytesread = read(sockfd, buff, sizeof(buff));
    while (bytesread >= MAX ) {
        printf("%s", buff);
        bytesread = read(sockfd, buff, sizeof(buff));
    }
    printf("%s", buff);
}

//void linefull(int sockfd) for sizes greater than 80

int getQuestion(int sockfd, int number, int randyn) {
    FILE *fp;
    fp = fopen("questions.txt", "r");
    if (fp == NULL) {
        writemessage(sockfd, "Question not found:");
        return -2;
    }
    char buff[1000];
    buff[0] = '\0';
    strcpy(buff, "\n");
    char *line = NULL;
    size_t len = 0;
    ssize_t read;
    int tempcount = 0;
    
    //Variables to return
    int tempnum;
    char tempcnum[MAX]; //What were printing out
    bzero(tempcnum, sizeof(tempcnum));
    
    
    if (randyn == 0) {
        writemessage(sockfd, "Retrieving Question: ");
    }
    //fwrite("\n", sizeof("\n"), sizeof(char), stdout);
    while ((read = getline(&line, &len, fp)) != -1) {
        stripString(line);
        if ((strncmp(line, "<st>", 4)) == 0) {
            sscanf(line, "<st> %d", &tempnum);
            if (tempnum == number) {
                forrand = tempnum;
                tempcount = 1;
                //sprintf(tempcnum, "%s", tempnum);
                fwrite(line+4, read, 1, stdout);
                strncat(buff, line+4, read);
                while ((read = getline(&line, &len, fp)) != -1) {
                    char readtemp[read];
                    readtemp[0] = '\0';
                    for (int i = 0; i < read; i++) {
                        if ((line[i] < 127) && (line[i] > 1)) {
                            char ctostr[2];
                            ctostr[1] = '\0';
                            ctostr[0] = line[i];
                            strcat(readtemp, ctostr);
                        }
                    }
                    if ((strncmp(readtemp, "<end>", 5)) == 0) {
                        break;
                    }
                    if ((strncmp(readtemp, ".", 1)) == 0) {
                        continue;
                    }
                    if ((strncmp(readtemp, "<tag>", 5)) == 0) {
                        continue;
                    }
                    if ((strncmp(readtemp, "<Q>", 3)) == 0) {
                        fwrite(readtemp + 3, strlen(readtemp + 3), 1, stdout);
                        strcat(buff, readtemp + 3);
                        continue;
                    }
                    if ((strncmp(readtemp, "<c>", 3)) == 0) {
                        fwrite(readtemp + 3, strlen(readtemp+3), 1, stdout);
                        strcat(buff, readtemp + 3);
                        continue;
                    }
                    if ((strncmp(readtemp, "<ca>", 4)) == 0) {
                        if (randyn == 0) {
                            fwrite(readtemp + 4, strlen(readtemp+4), 1, stdout);
                            strcat(buff, readtemp + 4);
                        }
                        continue;
                    }
                    fwrite(readtemp, 1, strlen(readtemp), stdout);
                    strcat(buff, readtemp);
                }
                break;
            }
        }
        
    }
    //strncat(buff, line, read);
    free(line);
    if (tempcount == 0 && randyn == 0) {
        writemessage(sockfd, "Question not found:");
    }
    else if (tempcount == 0 && randyn == 1) {
        fclose(fp);
        return -1;
        //randques(sockfd);
    }
    else {
        fwrite("Buff", strlen("Buff"), 1, stdout);
        fwrite(buff, strlen(buff), 1, stdout);
        writemessage(sockfd, buff);
    }
    //fwrite(buff, strlen(buff), 1, stdout);
    //fclose(fp);
    return 1;
}

int iterations = 0;

void randques(int sockfd) {
    int ok = 0;
    writemessage(sockfd, "Retrieving Random Question: ");
    getnumquestions;
    if (numquestions == 0) {
        writemessage(sockfd, "There are no questions.\n");
        return;
    }
    for (int i = 0; i < 10; i++) {
        int number = (rand() % numquestions) + 1;
        if (getQuestion(sockfd, number, 1) == 1) {
            ok = 1;
            break;
        }
        else if (getQuestion(sockfd, number, 1) == -2) {
            break;
        }
    }
    if (ok != 1) {
        getQuestion(sockfd, numquestions,1);
    }
}

void deleteques(int sockfd, int number) {
    FILE *fp1, *fp2;
    fp1 = fopen("questions.txt", "r");
    if (fp1 == NULL) {
        writemessage(sockfd, "There are no questions.");
        return;
    }
    char *line = NULL;
    size_t len = 0;
    ssize_t read;
    int tempcount = 0;
    int tempnum;
    fp2 = fopen("temp.txt", "w");
    
    int toskip = 0;
    
    while ((read = getline(&line, &len, fp1)) != -1) {
        
        char readtemp[read];
        readtemp[0] = '\0';
        for (int i = 0; i < read; i++) {
            if ((line[i] < 127) && (line[i] > 1)) {
                char ctostr[2];
                ctostr[1] = '\0';
                ctostr[0] = line[i];
                strcat(readtemp, ctostr);
            }
        }
        
        if ((strncmp(readtemp, "<st>", 4)) == 0) {
            sscanf(line, "<st> %d", &tempnum);
            if (tempnum == number) {
                toskip = 1;
                tempcount++;
            }
        }
        if (toskip != 1) {
            fwrite(readtemp, strlen(readtemp), 1, fp2);
        }
        else if ((strncmp(readtemp, "<end>", 5)) == 0) {
            toskip = 0;
        }
    }
    fclose(fp1);
    fclose(fp2);
    remove("questions.txt");
    rename("temp.txt", "questions.txt");
    getnumquestions();
    if (tempcount > 0) {
        writemessage(sockfd, "Deleted");
    }
    else {
        writemessage(sockfd, "Question not found. It is possible that it has already been deleted.");
    }
}

char* checkanswer(int sockfd, int number) {
    FILE *fp;
    fp = fopen("questions.txt", "r");
    if (fp == NULL) {
        writemessage(sockfd, "There are no questions.");
        return '\0';
    }
    char buff[1000];
    buff[0] = '\0';
    strcpy(buff, "\n");
    char *line = NULL;
    size_t len = 0;
    ssize_t read;
    int tempcount = 0;
    int tempnum;
    char answer[MAX];
    bzero(answer, MAX);
    
    while ((read = getline(&line, &len, fp)) != -1) {
        stripString(line);
        if ((strncmp(line, "<st>", 4)) == 0) {
            sscanf(line, "<st> %d", &tempnum);
            if (tempnum == number) {
                tempcount = 1;
                fwrite(line+4, read, 1, stdout);
                strncat(buff, line+4, read);
                while ((read = getline(&line, &len, fp)) != -1) {
                    char readtemp[read];
                    readtemp[0] = '\0';
                    for (int i = 0; i < read; i++) {
                        if ((line[i] < 127) && (line[i] > 1)) {
                            char ctostr[2];
                            ctostr[1] = '\0';
                            ctostr[0] = line[i];
                            strcat(readtemp, ctostr);
                        }
                    }
                    if ((strncmp(readtemp, "<ca>", 4)) == 0) {
                        sscanf(readtemp, "<ca> %s", answer);
                        printf("%s", answer);
                        return answer;
                    }
                }
                break;
            }
        }
        
    }
    free(line);
    if (tempcount == 0) {
        writemessage(sockfd, "Question not found:");
        return '\0';
    }
}

void createQuestion(int sockfd) {
    getnumquestions();
    numquestions++;
    char* stringnum[3];
    sprintf(stringnum, "%d", numquestions);
    char* questionnum[8];
    strcpy(questionnum, "<st>");
    strcat(questionnum, stringnum);
    strcat(questionnum, "\n");
    FILE *fp;
    fp = fopen("questions.txt", "a");
    fwrite(questionnum, 1, strlen(questionnum) + 1, fp);
    fwrite("<tag>", 1, 5, fp);
    printf("Question tag: ");
    writemessage(sockfd, "Please enter the question tag: ");
    char buff[MAX];
    bzero(buff, sizeof(buff));
    ssize_t bytesread;
    for (;;) {
        bytesread = read(sockfd, buff, sizeof(buff));
        printf("%s", buff);
        if (bytesread < MAX) {
            buff[bytesread] ='\0';
            fwrite(buff, 1, sizeof(buff), fp);
            bzero(buff, sizeof(buff));
            break;
        }
        else {
            buff[MAX] = '\0';
            fwrite(buff, 1, sizeof(buff), fp);
            bzero(buff, sizeof(buff));
        }
    }
    
    writemessage(sockfd, "Please Enter Question: ");
    fwrite("<Q>", 1, 3, fp);
    bzero(buff, sizeof(buff));
    printf("Question: ");
    for (;;) {
        bytesread = read(sockfd, buff, sizeof(buff));
        printf("%s", buff);
        if (bytesread < MAX) {
            buff[bytesread] ='\0';
            fwrite(buff, 1, sizeof(buff), fp);
            bzero(buff, sizeof(buff));
            break;
        }
        else {
            buff[MAX] = '\0';
            fwrite(buff, 1, sizeof(buff), fp);
            bzero(buff, sizeof(buff));
        }
    }
    
    writemessage(sockfd, "Please Enter Answer Choices: ");
    fwrite("<c>", 1, 3, fp);
    bzero(buff, sizeof(buff));
    printf("Choices: ");
    for (;;) {
        bytesread = read(sockfd, buff, sizeof(buff));
        printf("%s", buff);
        if (bytesread < MAX) {
            buff[bytesread] ='\0';
            fwrite(buff, 1, sizeof(buff), fp);
            bzero(buff, sizeof(buff));
            break;
        }
        else {
            buff[MAX] = '\0';
            fwrite(buff, 1, sizeof(buff), fp);
            bzero(buff, sizeof(buff));
        }
    }
    
    
    writemessage(sockfd, "Please Enter Answer: ");
    fwrite("<ca>", 1, 4, fp);
    bzero(buff, sizeof(buff));
    printf("Answer: ");
    for (;;) {
        bytesread = read(sockfd, buff, sizeof(buff));
        printf("%s", buff);
        if (bytesread < MAX) {
            buff[bytesread] ='\0';
            fwrite(buff, 1, sizeof(buff), fp);
            bzero(buff, sizeof(buff));
            break;
        }
        else {
            buff[MAX] = '\0';
            fwrite(buff, 1, sizeof(buff), fp);
            bzero(buff, sizeof(buff));
        }
    }
    char numquestionsmessage[100];
    sprintf(numquestionsmessage,"%d", numquestions);
    strcat(numquestionsmessage, "\n");
    writemessage(sockfd, numquestionsmessage);
    fwrite("<end>\n", 1, 6, fp);
    
    
    
    int close = fclose(fp);
    if (close == 0) {
        printf("Closed file");
    }
    else {
        printf("An error has occured.");
    }
}

void insertques(int sockfd) {
    printf("client requested to insert a question\n");
    createQuestion(sockfd);
}

void findnewclient(int sockfd) {
    int connfd, len;
    struct sockaddr_in servaddr, cli;
    
    if ((listen(sockfd, 5)) != 0) {
        printf("Listen failed...\n");
        exit(0);
    }
    else
        printf("Server listening..\n");
    len = sizeof(cli);
    
    // Accept the data packet from client and verification
    connfd = accept(sockfd, (SA*)&cli, &len);
    if (connfd < 0) {
        printf("server acccept failed...\n");
        exit(0);
    }
    else {
        printf("server acccept the client...\n");
    }
    
    //Initial Server Message
    char welcome[] = "Please enter a command: \nCommands include p - insert question, g - get question, d - delete question, r - random question, c - check answer, k - kill server, q - quit client, h - help. \n\n";
    //for loop until \n\n
    write(connfd, welcome, sizeof(welcome));
}

// Function designed for chat between client and server.
void func(int sockfd)
{
    char buff[MAX];
    getnumquestions();
    // infinite loop for chat
    while (1) {
        bzero(buff, MAX);
        read(sockfd, buff, sizeof(buff));
        if (buff == "\0") {
            printf("Server attempting to find new clients");
            //findnewclient(sockfd);
        }
        
        int commandfound = 0;
        
        printf("From client: %s\t", buff);
        
        if (strncmp("q", buff, 1) == 0) {
            close(sockfd);
            break;
        }
        
        
        if (strncmp("p", buff, 1) == 0) {
            insertques(sockfd);
            commandfound++;
        }
        
        if (strncmp("h", buff, 1) == 0) {
            char help[] = "Commands include p - insert question, g - get question, d - delete question, r - random question, c - check answer, k - kill server, q - quit client, h - help. \n\n";
            write(sockfd, help, sizeof(help));
            commandfound++;
        }
        
        if (strncmp("r", buff, 1) == 0) {
            randques(sockfd);
            char answer[MAX];
            read(sockfd, answer, sizeof(answer));
            char actualans[MAX];
            strcpy(actualans, checkanswer(sockfd, forrand));
            if ((strncmp(actualans, answer, 1))== 0) {
                writemessage(sockfd, "You are Correct!");
            }
            else {
                writemessage(sockfd, "Sorry, that was incorrect.");
            }
            
            commandfound++;
        }
        
        if (strncmp("d", buff, 1) == 0) {
            int number;
            sscanf(buff, "%*c%d", &number);
            deleteques(sockfd, number);
            commandfound++;
        }
        
        if (strncmp("g", buff, 1) == 0) {
            int number;
            sscanf(buff, "%*c%d", &number);
            getQuestion(sockfd, number, 0);
            commandfound++;
        }
        
        if (strncmp("c", buff, 1) == 0) {
            int number;
            char answer[MAX];
            char actualans[MAX];
            sscanf(buff, "%*c%d%s", &number, answer);
            printf("%s", actualans);
            strcpy(actualans, checkanswer(sockfd, number));
            if ((strncmp(actualans, answer, 1))== 0) {
                writemessage(sockfd, "You are Correct!");
            }
            else {
                writemessage(sockfd, "Sorry, that was incorrect.");
            }
            commandfound++;
        }
        
        
        if (strncmp("k", buff, 1) == 0) {
            close(sockfd);
            exit(0);
        }
        
        if (commandfound == 0) {
            writemessage(sockfd, "Command Not found. Please type h to look at the following commands.");
        }
        bzero(buff, MAX);
    }
}

// Driver function -- Heavily influenced by geeksforgeeks
int main(int argc, char *argv[]) {
    int sockfd, connfd, len;
    struct sockaddr_in servaddr, cli;
    
    // socket create and verification
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        printf("socket creation failed...\n");
        exit(0);
    }
    else
        printf("Socket successfully created..\n");
    bzero(&servaddr, sizeof(servaddr));
    
    // assign IP, PORT
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(PORT);
    
    // Binding newly created socket to given IP and verification
    if ((bind(sockfd, (SA*)&servaddr, sizeof(servaddr))) != 0) {
        printf("socket bind failed...\n");
        exit(0);
    }
    else
        printf("Socket successfully binded..\n");
    
    char hostname[1024];
    gethostname(hostname, 1024);
    
    printf("Host name: ");
    puts(hostname);
    printf("Host port: %d\n", PORT);
    
    // Now server is ready to listen and verification
    if ((listen(sockfd, 5)) != 0) {
        printf("Listen failed...\n");
        exit(0);
    }
    else
        printf("Server listening..\n");
    len = sizeof(cli);
    
    // Accept the data packet from client and verification
    connfd = accept(sockfd, (SA*)&cli, &len);
    if (connfd < 0) {
        printf("server acccept failed...\n");
        exit(0);
    }
    else {
        printf("server acccept the client...\n");
    }
    
    //Initial Server Message
    char welcome[] = "Please enter a command: \nCommands include p - insert question, g - get question, d - delete question, r - random question, c - check answer, k - kill server, q - quit client, h - help. \n\n";
    //for loop until \n\n
    write(connfd, welcome, sizeof(welcome));
    // Function for chatting between client and server
    
    
    for (;;) {
        func(connfd);
        findnewclient(sockfd);
    }
    
    // After chatting close the socket
    close(sockfd);
} 
