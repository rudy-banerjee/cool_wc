#include "cool_wc.h"
#include "strbuf.h"

#define STRBUF_ERROR_ALLOCFAIL -1

result_t Strbuf_from_constchar(Strbuf * s, const char * str)
{
    s->length = strlen(str);
    s->capacity = 1;
    while (s->capacity < s->length)
    {
        s->capacity *= 2;
    }

    s->buffer = (char *) malloc((s->capacity + 1) * sizeof(char));
    if (s->buffer == NULL)
        { return STRBUF_ERROR_ALLOCFAIL; }

    sprintf(s->buffer, "%s", str);

    return 0;
}

void Strbuf_free(Strbuf * s)
{
    if (s->buffer != NULL)
    {
        free(s->buffer);
        s->buffer = NULL;
    }
    s->length = 0;
    s->capacity = 0;
}

result_t Strbuf_append(Strbuf * s, char * str)
{
    size_t str_len = strlen(str);
    size_t new_len = s->length + str_len;

    char buffer_copy[s->capacity];
    strcpy(buffer_copy, s->buffer);
    
    if (new_len >= s->capacity)
    {
        size_t new_capacity = s->capacity * 2;
        while (new_capacity < new_len)
        {
            new_capacity *= 2;
        }

        s->buffer = (char *) realloc(s->buffer, (new_capacity + 1) * sizeof(char *));
        if (s->buffer == NULL)
            { return STRBUF_ERROR_ALLOCFAIL; }
        
        
        s->capacity = new_capacity;
    }

    sprintf(s->buffer, "%s%s", buffer_copy, str);
    s->length = new_len;

    return 0;
}

result_t Strbuf_append_char(Strbuf * s, char c)
{
    size_t new_len = s->length + 1;

    char buffer_copy[s->capacity];
    strcpy(buffer_copy, s->buffer);
    
    if (new_len >= s->capacity)
    {
        size_t new_capacity = s->capacity * 2;

        s->buffer = (char *) realloc(s->buffer, (new_capacity + 1) * sizeof(char *));
        if (s->buffer == NULL)
            { return STRBUF_ERROR_ALLOCFAIL; }
        
        
        s->capacity = new_capacity;
    }

    sprintf(s->buffer, "%s%c", buffer_copy, c);
    s->length = new_len;

    return 0;
}