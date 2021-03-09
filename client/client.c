#include <netdb.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <sys/socket.h> 
#define MAX 100 
#define PORT 50080 
#define SA struct sockaddr 
#define SERVER_IP "127.0.0.1"
#define SIZE 1024

void getfile(char buff[], int sockfd){

  int n;
  FILE *fp;
  
  char *pch;
  char *name_file;
  char msg[100];
  
  strcpy(msg, buff);
  strtok(msg, "\n");
  pch = strstr(msg, "get");
  if (pch != NULL) strncpy (pch,"***", 3);
  pch  = strtok(msg, "*** \n"); 
  strcpy(name_file, pch);

  char filename[100];

  sprintf(filename,"%s", name_file);
  
  char buffer[SIZE];  

  fp = fopen(filename, "w");
  if (fp == NULL) {
	perror("[-]Error in creating file.");
	exit(1);
  } 
  
  long int length;
  recv(sockfd, buffer, sizeof(buffer), 0);
  length = atol(buffer);
  
  while (1) {  
    memset(buffer, 0, sizeof(buffer));
    n = recv(sockfd, buffer, sizeof(buffer), 0);  
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
  fclose(fp);
}
void sendfile(char buff[], int sockfd){

	FILE *fp;
	char data[SIZE] = {0};
	
	char *pch;
	char name_file[50];
	char msg[100];

	strcpy(msg, buff);
	strtok(msg, "\n");
	pch = strstr(msg, "send");
	if (pch != NULL)
	  strncpy (pch,"****", 4);
	pch  = strtok(msg, "**** \n"); 
	strcpy(name_file, pch);
	
	char filename[100];
	
	sprintf(filename,"%s", name_file);
	
	fp = fopen(filename, "r");
	if (fp == NULL) {
		perror("[-]Error in reading file.");
		exit(1);
	}
	
	fseek(fp, 0, SEEK_END);
	snprintf(data, sizeof(data), "%ld", ftell(fp));
	send(sockfd, data, sizeof(data),  0);
	//bzero(data, SIZE);
	fseek(fp, 0, SEEK_SET);
	
	while(fgets(data, sizeof(data), fp) != NULL) {	
	    if(send(sockfd, data, sizeof(data),  0) == -1) {
	      perror("[-]Error in sending file.");
	      exit(1);
	    }
            bzero(data, SIZE);
	  }
        fclose(fp); 
}

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
        if(strncmp(buff, "help", 4) == 0){
        	printf("Lista de comandos disponíveis:\n-list <opção de ordenação> <asc/desc>: as opções de ordenação são 'name' e 'size'\n-send : enviar um arquivo no path 'file'\n-get : recuperar o arquivo com o nome 'file' \n-exit\n");
	
	} else if ((strncmp(buff, "exit", 4)) == 0) { 
	    send(sockfd, buff, sizeof(buff),0); 
            printf("Client Exit...\n"); 
            break; 
        } else{
        	send(sockfd, buff, sizeof(buff),0);        	
        	if((strncmp(buff, "get", 3)) == 0){
        		getfile(buff, sockfd);
        	} else if((strncmp(buff, "send", 4)) == 0){
        		sendfile(buff, sockfd);
        	}
        	
        	bzero(buff, sizeof(buff));         	
  	        recv(sockfd, buff, sizeof(buff),0); 
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
    servaddr.sin_addr.s_addr = inet_addr(SERVER_IP); 
    servaddr.sin_port = PORT; 
  
    if (connect(sockfd, (SA*)&servaddr, sizeof(servaddr)) != 0) { 
        printf("connection with the server failed...\n"); 
        exit(0); 
    } 
    else
        printf("connected to the server..\n"); 
  
    func(sockfd); 
  
    close(sockfd); 
} 

