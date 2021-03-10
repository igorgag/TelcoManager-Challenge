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

int getfile(char buff[], int sockfd, char id[]){
	
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
		return 0;
	}
	
	fseek(fp, 0, SEEK_END);
	snprintf(data, sizeof(data), "%ld", ftell(fp));
	send(sockfd, data, sizeof(data),  0);
	bzero(data, SIZE);
	fseek(fp, 0, SEEK_SET);
	
	while(fgets(data, sizeof(data), fp) != NULL) {
	
	    if(send(sockfd, data, sizeof(data),  0) == -1) {
	      perror("[-]Error in sending file.");
	      return 0;
	    }
            bzero(data, SIZE);	    
	  }
        fclose(fp);   
        return 1;           
}

int sendfile(char buff[], int sockfd, char id[], char *name_file, char *size){	
	
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
	return 0;
  } 
  
  long int length;
  recv(sockfd, buffer, sizeof(buffer), 0);
  length = atol(buffer);
  
  printf("len: %ld", length);  
  
  while (1) {  
    memset(buffer, 0, sizeof(buffer));
    n = recv(sockfd, buffer, sizeof(buffer), 0);  
    printf("n: %d", n);
    if ((length = length - n) <= 0){
      fprintf(fp, "%s", buffer);
      fflush(fp); 	
      break;
      return;
    }    
    fprintf(fp, "%s", buffer);
    fflush(fp);    
    //bzero(buffer, SIZE);
  }
  
  fseek(fp, 0, SEEK_END);
  snprintf(size, sizeof(size), "%ld", ftell(fp));
  
  fclose(fp); 
  return 1;                   
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
					if(!getfile(buff, sockfd, getUserID()))
						strcpy(buff,"NOK - Arquivo não recebido.\n");					
					strcpy(buff,"OOK - Arquivo recebido.\n");               
				} else if(strncmp("send", buff, 4) == 0){
					char name_file[100];
					char size[100];
					if(!sendfile(buff, sockfd, getUserID(), name_file, size))
						strcpy(buff,"OOK - Arquivo não enviado.\n");      
					addFile(name_file, size);
					strcpy(buff,"OOK - Arquivo enviado.\n");
				} else if(strncmp("list", buff, 4) == 0){
					if(!printFiles(buff))
						strcpy(buff,"NOK - Comando inválido.\n");

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
