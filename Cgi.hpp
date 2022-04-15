#ifndef __CGI_HPP
#define __CGI_HPP

#include <stdlib.h>
#include <string.h>
#include "utilities.hpp"
#include "Sqlite.hpp"

#define TAG_START "<sql"
#define TAG_CLOSE ">"
#define TAG_END "</sql>"

class Cgi {
public:
	static char* parse(char*);
private:
	static char* splitSql(char* string, char** db);
};


char* Cgi::parse(char* content) {

	char* db;
	char* query;
	char finalContent[10000];
	*finalContent = '\0';
	while (content) {
		char* nextContent = substr(content, NULL, "<sql");
		strcat(finalContent, nextContent);
		free(nextContent);
		query = splitSql(content, &db);
		if (query) {
			Table table = Sqlite::query(db, query);
			char* htmlTable = Sqlite::toHTML(table);
			strcat(finalContent, htmlTable);
			free(htmlTable);
			Sqlite::deleteTable(table);
			free(query);
			free(db);
		
		}
		content = strstr(content, TAG_END);
		if (content) content += strlen(TAG_END);
	}
	
	return strdup(finalContent);
}

char* Cgi::splitSql(char* string, char** db) {
	char* attributes = substr(string, TAG_START, TAG_CLOSE);
	if (!attributes) return NULL;
	char* dbname = substr(attributes, "\"", "\"");
	*db = dbname;
	free(attributes);

	char* next = strstr(string, TAG_START);
	char* ret = substr(next, TAG_CLOSE, TAG_END);
	return ret;
}
#endif