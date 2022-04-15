#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utilities.hpp"

char* splitSql(char* string, char** db) {
	char* attributes = substr(string, "<sql", ">");
	if (!attributes) return NULL;
	char* dbname = substr(attributes, "\"", "\"");
	*db = dbname;
	free(attributes);
	char* next = strstr(string, "<sql");
	char* ret = substr(next, ">", "</sql>");

	return ret;
}

int main(int argc, char const *argv[])
{
	char* db = NULL;
	char* ret = NULL;
	ret = splitSql("sjnjn<sql db=\"cane\">select * from tab</sql>", &db);
	printf("%s %s\n", ret, db);
	free(ret);
	free(db);
	return 0;
}