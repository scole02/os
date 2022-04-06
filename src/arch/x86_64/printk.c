#include "vga.h"
#include "libutils.h"
#include <stdarg.h>

#define BASE16 16
#define BASE10 10



void print_char(char c) { VGA_display_char(c); }
void print_str(const char *str) { VGA_display_str(str); }
void print_uchar(unsigned char c)
{
    char * num_str;
    itoa(c, num_str, BASE10);
    VGA_display_str(num_str); 
}

void print_short(short s)
{   
    char * num_str;
    itoa(s, num_str, BASE10);
    VGA_display_str(num_str); 
}

void print_long_hex(long l)
{   
    char * num_str;
    ltoa(l, num_str, BASE16);
    VGA_display_str(num_str); 
}

int printk(const char *fmt, ...) __attribute__ ((format (printf, 1, 2)))
{
    va_list args;
    char * str = fmt;
    va_start(args, fmt);
    

    while(*str) // check for null terminator
    {
        if (*str == '%') // format specifier reached
        {
            return;
        } 
        else print_char(*str);
        str++;
    }
}   
