#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cool_wc.h"
#include "wctable.h"

#define WCTABLE_SIZE 2048
#define WCTABLE_WORD_SIZE 256
#define WCTABLE_ERROR_ENTRYNOTFOUND -1
#define WCTABLE_ERROR_NOSPACE -2
#define WCTABLE_ERROR_ENTRYTOOLONG -3

result_t WCTable_get_count(WCTable * table, char * str)
{
    int result = WCTABLE_ERROR_ENTRYNOTFOUND;
    int i;
    for (i = 0; i < WCTABLE_SIZE; i++)
    {
        int str_equals = strcmp(table->buffer[i].word, str);
        if (str_equals == 0)
        {
            result = table->buffer[i].count;
            break;
        }
    }

    return result;
}

result_t WCTable_put_str(WCTable * table, char * str)
{
    if (table->size == WCTABLE_SIZE)
        { return WCTABLE_ERROR_NOSPACE; }
    if (strlen(str) >= WCTABLE_WORD_SIZE)
        { return WCTABLE_ERROR_ENTRYTOOLONG; }

    int result = 0;
    bool found_entry = FALSE;
    bool found_available_index = FALSE;
    int first_available_index = 0;
    int i;
    for (i = 0; i < WCTABLE_SIZE; i++)
    {
        if (!found_available_index) {
            if (table->buffer[i].count == 0) {
                found_available_index = TRUE;
                first_available_index = i;
            }
        }

        int str_equals = strcmp(table->buffer[i].word, str);
        if (str_equals == 0)
        {
            found_entry = TRUE;
            table->buffer[i].count += 1;
            strcpy(table->buffer[i].word, str);
            break;
        }
    }

    if (!found_entry)
    {
        strcpy(table->buffer[first_available_index].word, str);
        table->buffer[first_available_index].count = 1;
        table->size += 1;
    }

    return result;
}

void WCTable_print(WCTable * table)
{
    /* Get length of longest word in table */
    int max_word_length = 0;
    int i;
    for (i = 0; i < WCTABLE_SIZE; i++)
        max_word_length = ((int) strlen(table->buffer[i].word) > max_word_length)
            ? (int) strlen(table->buffer[i].word)
            : max_word_length;
    
    if (max_word_length == 0)
    {
        printf("WCTable is empty");
        return;
    }

    /* Print all words with their counts */
    printf(">>> WCTable, size: %ld\n", table->size);
    printf("+");
    int k;
    for (k = 0; k < max_word_length + 1; k++) putchar('-');
        printf("+--------+\n");
    for (i = 0; i < WCTABLE_SIZE; i++)
    {
        if (table->buffer[i].count != 0) {
            if (table->buffer[i].count == 1) {
                printf(
                    "| %-*s|%-8s|\n",
                    max_word_length,
                    table->buffer[i].word,
                    ".");
            }
            else
            {
                printf(
                    "| %-*s|%-8d|\n",
                    max_word_length,
                    table->buffer[i].word,
                    table->buffer[i].count);
            }
        }
    }
    printf("+");
    for (k = 0; k < max_word_length + 1; k++) putchar('-');
        printf("+--------+\n");
}