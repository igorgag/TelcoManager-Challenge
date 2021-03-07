
#include <netdb.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <sys/socket.h> 
#define MAX 100 
#define PORT 50080 
#define SA struct sockaddr 

void func(int sockfd) 
{ 
    char buff[MAX]; 
    int n; 
    for (;;) { 
        bzero(buff, sizeof(buff)); 
        printf("C : "); 
        n = 0; 
        while ((buff[n++] = getchar()) != '\n') 
            ; 
        if(strcmp(buff, "help\n") == 0){
        	printf("Lista de comandos disponíveis:\n-list <opção de ordenação> <asc/desc>: as opções de ordenação são 'name' e 'size'\n-send : enviar um arquivo no path 'file'\n-get : recuperar o arquivo com o nome 'file' \n-exit\n");
	
	} else if ((strncmp(buff, "exit", 4)) == 0) { 
            printf("Client Exit...\n"); 
            break; 
        } else{
        	write(sockfd, buff, sizeof(buff));
        	bzero(buff, sizeof(buff)); 
        	read(sockfd, buff, sizeof(buff)); 
        	printf("S: %s", buff);  
        }
         
    } 
} 
  
int main() 
{ 
    int sockfd, connfd; 
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
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1"); 
    servaddr.sin_port = htons(PORT); 
  
    if (connect(sockfd, (SA*)&servaddr, sizeof(servaddr)) != 0) { 
        printf("connection with the server failed...\n"); 
        exit(0); 
    } 
    else
        printf("connected to the server..\n"); 
  
    func(sockfd); 
  
    close(sockfd); 
} 

