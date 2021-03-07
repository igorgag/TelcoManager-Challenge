#include <stdio.h>
#include <stdlib.h>
//#include <mysql.h>

typedef struct{
	int id;
	char name[100];
	char adress[100];
} File;

typedef struct node{
	File file;
	struct node *next;
} Node;

typedef struct listFile{
	Node *first;
	Node *last;
} ListFile;

typedef struct{
	char id[10];
	char name[100];
	ListFile list_files;
} User;

int readUser(char id);
int updateUser(char id);
int addFile(ListFile list_files, File file);
void printFiles(ListFile list_files);
