#include <stdio.h> 
#include <netdb.h> 
#include <netinet/in.h> 
#include <stdlib.h> 
#include <string.h> 
#include <sys/socket.h> 
#include <sys/types.h> 
#define PORT 50080
#define MAX 100
#define IP "127.0.0.1"
#define SIZE 1024

#include <sys/types.h> 

#define SA struct sockaddr

#include "user/user.h"

void getfile(char buff[], int sockfd, char id[]){
	
	FILE *fp;
	char data[SIZE] = {0};
	
	char *pch;
	char name_file[50];
	char msg[100];

	strcpy(msg, buff);
	strtok(msg, "\n");
	pch = strstr(msg, "get");
	if (pch != NULL)
	  strncpy (pch,"***", 3);
	pch  = strtok(msg, "*** \n"); 
	strcpy(name_file, pch);
	
	char filename[100];
	
	sprintf(filename,"./server/files/%s/%s", id, name_file);
	
	fp = fopen(filename, "r");
	if (fp == NULL) {
		perror("[-]Error in reading file.");
		exit(1);
	}
	
	while(fgets(data, sizeof(data), fp) != NULL) {
	
	    if(send(sockfd, data, sizeof(data),  0) == -1) {
	      perror("[-]Error in sending file.");
	      exit(1);
	    }
            bzero(data, SIZE);	    
	  }
        fclose(fp);              
}

void sendfile(char buff[], int sockfd, char id[], char *name_file, char *size){	
	
  int n;
  FILE *fp;
  
  char *pch;
  char msg[100];
  
  strcpy(msg, buff);
  strtok(msg, "\n");
  pch = strstr(msg, "send");
  if (pch != NULL) strncpy (pch,"****", 4);
  pch  = strtok(msg, "**** \n"); 
  strcpy(name_file, pch);

  char filename[100];
  
  sprintf(filename,"./server/files/%s/%s", id, name_file);
  
  char buffer[SIZE];  

  fp = fopen(filename, "w");
  if (fp == NULL) {
	perror("[-]Error in creating file.");
	exit(1);
  } 
  
  while (1) {  
    n = recv(sockfd, buffer, sizeof(buffer), 0);
    printf("buffer: %s", buffer);
    if (n <= 0){
      break;
      return;
    }    
    fprintf(fp, "%s", buffer);    
    bzero(buffer, SIZE);
  }
  
  fseek(fp, 0, SEEK_END);
  sprintf(size, "%lld", ftell(fp));
  
  fclose(fp);                    
}
void func(int sockfd)
{ 
    char buff[MAX]; 
    int n;  
    bool cliente = false;       

    for (;;) { 
         
        recv(sockfd, buff, sizeof(buff),0); 

        printf("C: %s\n", buff); 
        
	/*
	bzero(buff, MAX); 
	n = 0;
	while ((buff[n++] = getchar()) != '\n') 
            ;
        */
        if (strncmp("exit", buff, 4) == 0) { 
            printf("Good Bye Client...\n"); 
            break; 
        }
        
        if(strncmp(buff, "hello", 5) == 0){
        	strcpy(buff, "hello\n");                        	
        }else{ //é o Id do cliente
        	if(!readUser(strtok(buff, "\n"))){        		
			strcpy(buff,"NOK - Cliente não encontrado.\n");
		} else {			
			strcpy(buff,"OOK - Cliente encontrado.\n");
			while(1){
 				send(sockfd, buff, sizeof(buff),0);
 				recv(sockfd, buff, sizeof(buff),0);
				if(strncmp("get", buff, 3) == 0){
					getfile(buff, sockfd, getUserID());               
				} else if(strncmp("send", buff, 4) == 0){
					char name_file[100];
					char size[100];
					sendfile(buff, sockfd, getUserID(), name_file, size);                
					addFile(name_file, size);
				} else if(strncmp("list", buff, 4) == 0){
					if(!printFiles(buff)){
						strcpy(buff,"NOK - Cliente não encontrado.\n");
					}
				}else if(strncmp("exit", buff, 4) == 0){
					break;				
				} else {
					strcpy(buff,"NOK - Comando Invalido.\n");
				}								 
			}
		}
		
        }        
                	        
        send(sockfd, buff, sizeof(buff),0);         
    } 
} 

int main() 
{ 
    int sockfd, sockarq, connfd, len;
    
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
    servaddr.sin_addr.s_addr = inet_addr(IP); 
    servaddr.sin_port = PORT; 
  
    if ((bind(sockfd, (SA*)&servaddr, sizeof(servaddr))) != 0) { 
        printf("socket bind failed...\n"); 
        exit(0); 
    } 
    else
        printf("Socket successfully binded..\n"); 
  
    if ((listen(sockfd, 10)) != 0) { 
        printf("Listen failed...\n"); 
        exit(0); 
    } 
    else
        printf("Server listening..\n"); 
    len = sizeof(cli); 
    while(1){
    connfd = accept(sockfd, (SA*)&cli, &len); 
    if (connfd < 0) { 
        printf("server acccept failed...\n"); 
        exit(0); 
    } 
    else
        printf("server acccept the client...\n"); 
  
    func(connfd); 
    }
  
    close(sockfd); 
} 
