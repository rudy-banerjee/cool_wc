#include <stdio.h>
#include <string.h>

/* C89-compatible itoa implementation */
void itoa(int value, char *str, int base)
{
    char buffer[33]; /* enough for 32-bit binary plus null terminator */
    int i = 0;
    int is_negative = 0;
    unsigned int n;

    /* Handle only valid bases */
    if (base < 2 || base > 36) {
        str[0] = '\0';
        return;
    }

    /* Handle negative numbers (only for base 10) */
    if (value < 0 && base == 10) {
        is_negative = 1;
        n = (unsigned int)(-value);
    } else {
        n = (unsigned int)value;
    }

    /* Convert to string (in reverse) */
    do {
        int digit = n % base;
        buffer[i++] = (digit < 10) ? (char)(digit + '0') : (char)(digit - 10 + 'A');
        n /= base;
    } while (n > 0);

    /* Add negative sign if needed */
    if (is_negative) {
        buffer[i++] = '-';
    }

    /* Null terminate */
    buffer[i] = '\0';

    /* Reverse the string into the output buffer */
    {
        int j;
        for (j = 0; j < i; j++) {
            str[j] = buffer[i - j - 1];
        }
        str[i] = '\0';
    }
}

/* Example usage */
int main(void)
{
    char s[33];

    itoa(12345, s, 10);
    printf("Decimal: %s\n", s);

    itoa(-12345, s, 10);
    printf("Negative Decimal: %s\n", s);

    itoa(255, s, 16);
    printf("Hex: %s\n", s);

    itoa(255, s, 2);
    printf("Binary: %s\n", s);

    return 0;
}
