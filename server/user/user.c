#include "user.h"

User *user;
ListFile *list_files;

MYSQL_RES *result;
MYSQL_ROW row;

void finish_with_error(MYSQL *con)
{
  fprintf(stderr, "%s\n", mysql_error(con));
  mysql_close(con);
}
int readUser(char id[])
{
	MYSQL *con = mysql_init(NULL);
	char query[200];
	
	if (con == NULL){
	      fprintf(stderr, "%s\n", mysql_error(con));
	      return 0;
  	}
  	
  	if (mysql_real_connect(con, "localhost", "root", "root",
	"TELCOMANAGER", 0, NULL, 0) == NULL){
		finish_with_error(con);
		return 0;
	}
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

		//printf("%s %s\n", user->id, user->name);


	} else {
		return 0;
	}
	
	mysql_free_result(result);	
	
	//busca arquivos do cliente
	sprintf(query, "SELECT file.NAME_FILE FROM FILE file WHERE file.USER_ID = '%s'", id);		
	
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
 	
	num_fields = mysql_num_fields(result);	
	
	list_files = malloc(sizeof(ListFile));	
	list_files->first = NULL;
	list_files->last = NULL;
	
	while((row = mysql_fetch_row(result)))
	{	

		File *file = malloc(sizeof(File));
		strcpy(file->name,row[0]);
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
	
	if(!printFiles()){
		printf("Lista de Arquivos Vazia.\n");
	}
	
  	return 1; 	
}
int addFile(char name[], char adress[])
{
}
int printFiles()
{
	File *iterator;
	if(list_files->first == NULL){
		return 0;
	}else{			
		iterator = list_files->first;
		while(iterator != NULL){
			printf("Nome: %s - Tamanho: %d\n", iterator->name, 10);
			iterator = iterator->next;
		}		
	}
	return 1;
}


