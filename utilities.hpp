#ifndef __UTILITIES_H
#define __UTILITIES_H

#include <cstdio>
#include <cstdlib>
#include <errno.h>
#include <cstring>

void errore(char* s, int n);
char* substr(char* string, char* start, char* end);
long getFileSize(FILE* file);

void errore(char* s, int n) {
    printf("ERROR in %s - %d - %s\n", s, errno, strerror(errno));
    printf("return code: %d\n", n);
    exit(n);
}

char* substr(char* string, char* start, char* end) {
    char* startPointer;
    char* endPointer;

    if (start != NULL) {
        startPointer = strstr(string, start);
        if (startPointer == NULL) return NULL;
        startPointer += strlen(start);
    }
    else
        startPointer = string;
    
    if (end != NULL) {
        endPointer = strstr(startPointer, end);
        if (endPointer == NULL) endPointer = string+strlen(string);
    }
    else 
        endPointer = string+strlen(string);
    int size = 0;
    for (char* p = startPointer; p != endPointer; p++) 
        size++;

    if (size == 0) return NULL;
    char* ret = (char*) malloc(sizeof(char)*size + 1);
    char* retP = ret;
    for (char* p = startPointer; p != endPointer; p++) {
        *retP = *p;
        retP++;
    }
    *retP = '\0'; 
    
    return ret;

}

long getFileSize(FILE* file) {
	fseek(file, 0, SEEK_END);
	long size = ftell(file);
	fseek(file, 0, SEEK_SET);
	return size;
}

#endif