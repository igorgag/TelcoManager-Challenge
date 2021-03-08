#include "user.h"

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
	/*
	if(mysql_fetch_row(result) == NULL){
		finish_with_error(con);
		return 0;
	}*/

	if((row = mysql_fetch_row(result)))
	{				
		for(int i = 0; i < num_fields; i++)
		{
		  printf("%s ", row[i] ? row[i] : "NULL");
		}

		printf("\n");
	} else {
		return 0;
	}
	
	mysql_free_result(result);
	mysql_close(con);
  	return 1; 	
}
int updateUser(char id)
{
}
int addFile(ListFile list_files, File file)
{
}
void printFiles(ListFile list_files)
{
}
void connectDB(){
}

