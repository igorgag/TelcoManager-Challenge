#include <stdio.h>
#include <stdlib.h>
#include <mysql.h>
#include <string.h>

typedef struct file{
	char name[100];
	struct file *next;
} File;

typedef struct listFile{
	File *first;
	File *last;
} ListFile;

typedef struct{
	char id[10];
	char name[100];
	ListFile list_files;
} User;

int readUser(char id[]);
int addFile(char name[], char adress[]);
int printFiles();

