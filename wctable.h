#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cool_wc.h"

#define WCTABLE_SIZE 2048
#define WCTABLE_WORD_SIZE 256
#define WCTABLE_ERROR_ENTRYNOTFOUND -1
#define WCTABLE_ERROR_NOSPACE -2
#define WCTABLE_ERROR_ENTRYTOOLONG -3

typedef struct {
    int count;
    char word[WCTABLE_WORD_SIZE];
} WordAndCount;

typedef struct {
    size_t size;
    WordAndCount buffer[WCTABLE_SIZE];
} WCTable;

result_t WCTable_get_count(WCTable * table, char * str);

result_t WCTable_put_str(WCTable * table, char * str);

void WCTable_print(WCTable * table);