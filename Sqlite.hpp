#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h> 
#include <string.h>

typedef struct table {
	int rows;
	int columns;
	char*** content;
	char** headers;
} Table;

class Sqlite
{
private:
	static sqlite3* connectDB(char*);
	static void closeDB(sqlite3*);
	static int read_select(void*, int, char**, char**);
public: 
	static Table query(char*, char*);
	static void exec(char*, char*);
	static void deleteTable(Table);
	static char* toHTML(Table);
};


sqlite3* Sqlite::connectDB(char* dbname) {
	sqlite3* db;
	int rc;

   	rc = sqlite3_open(dbname, &db);
   	if (rc) return NULL;
   	return db;
}

void Sqlite::closeDB(sqlite3* db) {
	sqlite3_close(db);
}

Table Sqlite::query(char* dbname, char* sql) {

	sqlite3* db = Sqlite::connectDB(dbname);
	char* errMsg = 0;
	Table table = {
		.rows = 0,
		.columns = 0,
		.content = NULL,
		.headers = NULL,
	};
	if (sql)
		sqlite3_exec(db, sql, read_select, (void*) &table, &errMsg);
	sqlite3_free(errMsg);
	Sqlite::closeDB(db);
	return table;
}

void Sqlite::exec(char* dbname, char* sql) {
	sqlite3* db = Sqlite::connectDB(dbname);
	char* errMsg = 0;
	sqlite3_exec(db, sql, NULL, NULL, &errMsg);
	sqlite3_free(errMsg);
	Sqlite::closeDB(db);
}

int Sqlite::read_select(void* data, int argc, char** argv, char** colName){
	Table* table = (Table*) data;
	
	if (!table->content) { 
		table->content = (char***) malloc(sizeof(char**)); //alloco una singola riga
		table->columns = argc;

	}
	else 
		table->content = (char***) realloc(table->content, (table->rows+1)*sizeof(char**));

	int i;
	if (!table->headers) {
		table->headers = (char**) malloc(sizeof(char*)*argc);
		for (i = 0; i < argc; i++) {
			table->headers[i] = strdup(colName[i]);
		}
	}

	char** row = (char**) malloc(sizeof(char*)*argc);
	for(i = 0; i < argc; i++) {
		row[i] = strdup(argv[i]);
	}
	table->content[table->rows] = row;

	table->rows++;	

	return 0;
}

void Sqlite::deleteTable(Table table) {
	for (int i = 0; i < table.columns; i++) {
		free(table.headers[i]);
	}
	free(table.headers);
	for (int i = 0; i < table.rows; i++) {
		for (int j = 0; j < table.columns; j++) {
			free(table.content[i][j]);
		}
		free(table.content[i]);
	}
	free(table.content);
}

char* Sqlite::toHTML(Table table) {
	char buffer[10000] = "";
	strcat(buffer, "<table><thead><tr>");
	for (int i = 0; i < table.columns; i++) {
		sprintf(buffer, "%s<th>%s</th>", buffer, table.headers[i]);
	}
	strcat(buffer, "</tr></thead>\n<tbody>");
	for (int i = 0; i < table.rows; i++) {
		strcat(buffer, "\n<tr>");
		for (int j = 0; j < table.columns; j++) {
			sprintf(buffer, "%s<td>%s</td>", buffer, table.content[i][j]);
		}
		strcat(buffer, "</tr>");
	}
	strcat(buffer, "\n</tbody></table>");
	char* ret = (char*) malloc(strlen(buffer));
	strcpy(ret, buffer);
	
	return ret;
}






