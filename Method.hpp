#ifndef __METHOD_HPP
#define __METHOD_HPP

#include <stdlib.h>
#include <string.h>

typedef enum {
    GET,
    NOT_IMPLEMENTED
} Method;

inline char const* methodToString(Method m) {
    char const* methods[] = {
        "GET",
        "NOT IMPLEMENTED",
    };
    return strdup(methods[m]);
}

#endif