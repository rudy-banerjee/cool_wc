#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cool_wc.h"
#include "strbuf.h"
#include "wctable.h"

void panic_constchar(const char * msg)
{
    fprintf(stderr, "PANIC: %s\n", msg);
    fflush(stderr);
    abort();  /* Immediately terminates the program */
}

int main(int argc, char *argv[])
{
    Strbuf s;
    Strbuf_from_constchar(&s, "Test!");
    printf("Strbuf s buffer: |%s|\n", s.buffer);

    Strbuf_append(&s, " - Test!");
    printf("Strbuf s buffer: |%s|\n", s.buffer);

    Strbuf_free(&s);
    printf("\n");
    printf("\n");

    FILE *file;
    int ch;

    /* Check if a file name is provided */
    if (argc != 2) {
        printf("Usage: %s <filename>\n", argv[0]);
        return 1;
    }

    /* Open the file for readingarray_strbuf.items[i] */
    file = fopen(argv[1], "r");
    if (file == NULL) {
        printf("Error: Could not open file %s\n", argv[1]);
        return 1;
    }

    WCTable t;
    memset(t.buffer, 0, WCTABLE_SIZE * sizeof(WordAndCount));
    /* ^ Remove this line to see a cool bug */
    /* If the memory is not memset to 0, it will collect random garbage as words sometimes*/

    Strbuf current_strbuf;
    Strbuf_from_constchar(&current_strbuf, "");
    printf("Strbuf buffer: |%s|\n", current_strbuf.buffer);

    /* Read and print each character until EOF */
    while ((ch = fgetc(file)) != EOF)
    {
/*      putchar(ch);        */
        /* 32 = Space, 127 = DEL */
        /* Everything in between is a readable char */
        if (ch >= 33 && ch <= 126)
        {
            Strbuf_append_char(&current_strbuf, (char) ch);
        }
        else if (ch == ' ' || ch == '\n')
        {
            if (current_strbuf.length == 0)
                continue; /* Strbuf is empty */
            
            /* Save strbuf contents in wctable*/
            result_t err = WCTable_put_str(&t, current_strbuf.buffer);
            switch (err) {
                case WCTABLE_ERROR_ENTRYTOOLONG:
                    panic_constchar("WCTable insertion failed: string is too long");
                break;
                case WCTABLE_ERROR_NOSPACE:
                    panic_constchar("WCTable insertion failed: no space in table");
                break;
            }
            if (t.size < 10)
            {
                printf("> %s saved in table\n", current_strbuf.buffer);
                WCTable_print(&t);
            }

            /* Clear current_strbuf */
            current_strbuf.length = 0;
            memset(current_strbuf.buffer, 0, (current_strbuf.capacity + 1) * sizeof(char));
        }
        else
        {
            /* Found some random bullshit,
               Clear current_strbuf */
            current_strbuf.length = 0;
            memset(current_strbuf.buffer, 0, (current_strbuf.capacity + 1) * sizeof(char));
        }
    }
    putchar('\n');

    if (t.size == 0)
        { panic_constchar("No words inserted into table??"); }
    
    WCTable_print(&t);

    fclose(file);

    return 0;
};