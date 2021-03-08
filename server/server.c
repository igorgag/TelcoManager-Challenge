#include <stdio.h> 
#include <netdb.h> 
#include <netinet/in.h> 
#include <stdlib.h> 
#include <string.h> 
#include <sys/socket.h> 
#include <sys/types.h> 
#define PORT 50080
#define MAX 100

#include <sys/types.h> 

#define SA struct sockaddr

#include "user/user.h"

void func(int sockfd)
{ 
    char buff[MAX]; 
    int n; 

    for (;;) { 
        bzero(buff, MAX); 
  
        read(sockfd, buff, sizeof(buff)); 

        printf("C: %s\n", buff); 
        
	/*
	bzero(buff, MAX); 
	n = 0;
	while ((buff[n++] = getchar()) != '\n') 
            ;
        */
        if(strncmp(buff, "hello", 5) == 0){
        	strcpy(buff, "hello\n");
        }else if(strncmp("list", buff, 4) == 0){
        
        }else if(strncmp("get", buff, 3) == 0){
        
        } else if(strncmp("send", buff, 4) == 0){
        	 
        } else{ //se não é um comando é o Id do cliente
        	if(!readUser(strtok(buff, "\n"))){        		
			strcpy(buff,"Cliente não encontrado.\n");
		} else {			
			strcpy(buff,"Cliente encontrado.\n");
		}
		
        }        
        
        write(sockfd, buff, sizeof(buff)); 
	/*
        if (strncmp("exit", buff, 4) == 0) { 
            printf("Server Exit...\n"); 
            break; 
        }*/
    } 
} 

int main() 
{ 
    int sockfd, connfd, len; 
    struct sockaddr_in servaddr, cli; 
  
    sockfd = socket(AF_INET, SOCK_STREAM, 0); 
    if (sockfd == -1) { 
        printf("socket creation failed...\n"); 
        exit(0); 
    } 
    else
        printf("Socket successfully created..\n"); 
    bzero(&servaddr, sizeof(servaddr)); 
  
    servaddr.sin_family = AF_INET; 
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY); 
    servaddr.sin_port = htons(PORT); 
  
    if ((bind(sockfd, (SA*)&servaddr, sizeof(servaddr))) != 0) { 
        printf("socket bind failed...\n"); 
        exit(0); 
    } 
    else
        printf("Socket successfully binded..\n"); 
  
    if ((listen(sockfd, 5)) != 0) { 
        printf("Listen failed...\n"); 
        exit(0); 
    } 
    else
        printf("Server listening..\n"); 
    len = sizeof(cli); 
  
    connfd = accept(sockfd, (SA*)&cli, &len); 
    if (connfd < 0) { 
        printf("server acccept failed...\n"); 
        exit(0); 
    } 
    else
        printf("server acccept the client...\n"); 
  
    func(connfd); 
  
    close(sockfd); 
} 
