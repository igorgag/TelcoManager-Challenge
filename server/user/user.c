#include "user.h"
#include "../connectDB.h"

User *user;
ListFile *list_files;

MYSQL *con;

MYSQL_RES *result;
MYSQL_ROW row;

void finish_with_error(MYSQL *con);

int connectDB(){
	con = mysql_init(NULL);			
	
	if (con == NULL){
	      fprintf(stderr, "%s\n", mysql_error(con));
	      return 0;
  	}
  	
  	if (mysql_real_connect(con, "localhost", USER, PASSWORD,
	"TELCOMANAGER", 0, NULL, 0) == NULL){
		finish_with_error(con);
		return 0;
	}
}

int readUser(char id[])
{
	char query[200];
		
	if(!connectDB())
		return 0;
		
	//Busca dados do usuario(cliente)		
	sprintf(query, "SELECT * FROM USER user WHERE user.USER_ID = '%s'", id);		
	
	if (mysql_query(con, query))
	{
		finish_with_error(con);
		return 0;	   
	}
	
	result = mysql_store_result(con);

	if (result == NULL)
	{	
		finish_with_error(con);		
		return 0;
	}
 
	int num_fields = mysql_num_fields(result);	

	if((row = mysql_fetch_row(result)))
	{	
		user = malloc(sizeof(User));			
		strcpy(user->id,row[0]);
		strcpy(user->name,row[1]);		


	} else {
		return 0;
	}
	
	mysql_free_result(result);		
	mysql_close(con);
  	return 1; 	
}
char * getUserID(){
	return user->id;
}
int addFile(char name[], char size[])
{
	char query[200];
	
	File *file = malloc(sizeof(File)); 
	strcpy(file->name,name);
	strcpy(file->size,size);
	file->next = NULL;
	
	if(!connectDB())
		return 0;
		
	//insere arquivo no banco		
	sprintf(query, "INSERT INTO FILE(NAME_FILE,SIZE,USER_ID) VALUES('%s','%s','%s')  ", name, size, user->id);		
	
	if (mysql_query(con, query))
	{
		finish_with_error(con);
		return 0;	   
	}
		
	list_files->last->next = file;			
	list_files->last = file;
	
	mysql_close(con);
	return 1;
	
}
int printFiles(char msg[])
{	
	char query[200];
	
	if(!connectDB())
		return 0;
		
	strtok(msg,"\n");
		
	char ord[10];
	char asdc[10];
	
	char ord_format[50];
	char asdc_format[10];
	
	for(int i=0;i<4;i++){
		ord[i] = msg[i+5];		
	}
	
	for(int i=0;i<4;i++){
		asdc[i] = msg[i+10];
	}
	
	if(strncmp(ord, "name",4)){
		strcpy(ord_format,"file.NAME_FILE");
	} else if(strncmp(ord, "size",4)){
		strcpy(ord_format,"file.SIZE");
	} else {
		return 0;
	}
	
	if(strncmp(asdc,"asc",3)){
		strcpy(asdc_format,"ASC");
	} else if(strncmp(asdc, "desc", 4)){
		strcpy(asdc_format,"DESC");
	}else{
		return 0;
	}
	
	//busca arquivos do cliente
	sprintf(query, "SELECT file.NAME_FILE,file.SIZE FROM FILE file WHERE file.USER_ID = '%s' ORDER BY %s %s", user->id, ord_format, asdc_format);		
	
	if (mysql_query(con, query))
	{
		finish_with_error(con);
		return 0;	   
	}
	
	result = mysql_store_result(con);

	if (result == NULL)
	{	
		finish_with_error(con);		
		return 0;
	}
 	
	int num_fields = mysql_num_fields(result);	
	
	list_files = malloc(sizeof(ListFile));	
	list_files->first = NULL;
	list_files->last = NULL;
	
	while((row = mysql_fetch_row(result)))
	{	

		File *file = malloc(sizeof(File));
		strcpy(file->name,row[0]);
		strcpy(file->size,row[1]);
		file->next = NULL;	

		if(list_files->first == NULL){
			list_files->first = file;
			list_files->last = file;
		} else {
			list_files->last->next = file;			
			list_files->last = file;
		}	
	} 	
	
	mysql_close(con);
	
	File *iterator;
	char row[1000];
	if(list_files->first == NULL){
		return 0;
	}else{			
		iterator = list_files->first;
		while(iterator != NULL){
			sprintf(row, "Nome: %s - Tamanho: %s\n", iterator->name, iterator->size);
			strcat(msg, row);
			iterator = iterator->next;
		}		
	}
	return 1;
}

void finish_with_error(MYSQL *con)
{
  fprintf(stderr, "%s\n", mysql_error(con));
  mysql_close(con);
}



