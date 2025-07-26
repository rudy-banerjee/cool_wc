#ifndef STRBUF_H
#define STRBUF_H

#include <stdlib.h>
#include <stdio.h>
#include "cool_wc.h"

typedef struct {
    char * buffer;
    size_t length;
    size_t capacity;
} Strbuf;

result_t Strbuf_from_constchar(Strbuf * s, const char * str);

void Strbuf_free(Strbuf * s);

result_t Strbuf_append(Strbuf * s, char * str);

result_t Strbuf_append_char(Strbuf * s, char c);

#endif