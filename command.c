#define DB_FILE "forask.db"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "command.h"
#include "sqlite3.h"
#include "sql.h"
#include "log.h"

sqlite3 *conn;

void openDb(){
	LOG("Opening local DB...\n");
        int ret=sqlite3_open(DB_FILE,&conn);
        if (ret!=SQLITE_OK){
                LOG("Open local DB failed...\n");
                exit(-1);
        }else{
		LOG("Open local DB successfully...\n");
		char *errMsg;
		ret=sqlite3_exec(
			conn,
			CREATE_CONFIG_TABLE_SQL,
			NULL,
			NULL,
			&errMsg
		);
		if (ret!=SQLITE_OK)
			LOG("%s\n",errMsg);
		if (ret==SQLITE_OK){
			ret=sqlite3_exec(
				conn,
				INSERT_CURRENT_AREA_CONFIG_SQL,
				NULL,
				NULL,
				&errMsg
			);
			if (ret!=SQLITE_OK)
				LOG("%s\n",errMsg);
		}		
		ret=sqlite3_exec(
			conn,
			CREATE_AREA_TABLE_SQL,
			NULL,
			NULL,
			&errMsg
		);
		if (ret!=SQLITE_OK)
			LOG("%s\n",errMsg);
	}
}

void closeDb(){
	LOG("Closing local DB...\n");
        int ret=sqlite3_close(conn);
        if (ret!=SQLITE_OK){
                LOG("Close loacl DB failed...\n");
                exit(-1);
        }else{
		LOG("Closed local DB successfully...\n");
	}
}

int sync(){
	return 0;
}

int areaShow(){
	openDb();
	char *errMsg;
	char **dbResult;
	int nRow,nColumn;
	
	int ret=sqlite3_get_table(
			conn,
			QUERY_ALL_AREA_SQL,
			&dbResult,
			&nRow,
			&nColumn,
			&errMsg
		);
	if (ret==SQLITE_OK){
		printf("%d areas in total:\n",nRow);
		for (int i=0;i<nRow;i++){
			printf("%s\n",dbResult[(i+1)*nColumn]);
		}
	}else{
		printf("Query all areas failed...\n");
	}

	ret=sqlite3_get_table(
			conn,
			QUERY_CURRENT_AREA_SQL,
			&dbResult,
			&nRow,
			&nColumn,
			&errMsg
		);
	if (ret==SQLITE_OK){
		if (dbResult[1]==NULL){
			printf("No selected area.\n");
		}else{
			printf("Selected area:%s\n",dbResult[1]);
		}
	}


	closeDb();
	return 0;
}

int areaSwitch(char *area){
	openDb();
	char **dbResult;
	int nRow,nColumn;
	char *errMsg;
	char *selectSql=(char*)malloc(1024);
	sprintf(selectSql,QUERY_CERTAIN_AREA_SQL,area);
	int ret=sqlite3_get_table(
			conn,
			selectSql,
			&dbResult,
			&nRow,
			&nColumn,
			&errMsg
		);
	if (ret!=SQLITE_OK){
		LOG("%s\n",errMsg);
	}else{
		if (nRow<1){
			printf("No such area.\n");
		}else{
			char *updateSql=(char*)malloc(1024);
			sprintf(updateSql,UPDATE_SWITHCED_AREA_SQL,dbResult[1]);
			ret=sqlite3_exec(
					conn,
					updateSql,
					NULL,
					NULL,
					&errMsg
				);
			if (ret!=SQLITE_OK){
				LOG("%s\n",errMsg);
			}else{
				printf("Current arear swithced to %s.\n",area);
			}
			free(updateSql);
		}
	}
	free(selectSql);
	closeDb();
	return 0;
}

int areaAdd(char *area){
	openDb();
	char *errMsg;
	char *insertSql=(char*)malloc(1024);
	sprintf(insertSql,INSERT_AREA_SQL,area);	
	int	ret=sqlite3_exec(
			conn,
			insertSql,
			NULL,
			NULL,
			&errMsg
		);
	if (ret!=SQLITE_OK){
		LOG("%s\n",errMsg);
	}else{
		printf("Area %s added.\n",area);
	}
	free(insertSql);
	closeDb();
	return 0;
}

int taskShow(){
	return 0;
}

int taskShowId(long taskId){
	return 0;
}

int taskAdd(char *title,char *desc){
	return 0;
}

int taskDone(long taskId){
	return 0;
}

int help(){
	return 0;
}

