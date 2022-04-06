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

void print_int(int i)
{   
    char * num_str;
    itoa(i, num_str, BASE10);
    VGA_display_str(num_str); 
}

void print_long_hex(long l)
{   
    char * num_str;
    ltoa(l, num_str, BASE16);
    VGA_display_str(num_str); 
}

int printk(const char *fmt, ...)
{

    va_list args;
    char fmt_specifier;
    //char * str = fmt;
    va_start(args, fmt);
    

    while(*fmt) // check for null terminator
    {
        if (*fmt == '%') // format specifier reached
        {
            fmt++;
            fmt_specifier = *fmt;
            if (fmt_specifier == '%') // special case where we dont fetch an arg
            {
                print_char('%');
                fmt++;
                continue;
            }
            switch(fmt_specifier)
            {
                case 'd': // int
                    print_int(va_arg(args, int));
                    break;
                
                case 's':
                    print_str(va_arg(args, char *));
                    break;
                
                case 'c':
                    print_char(va_arg(args, int));
                    break;
                    
                case 'u':
                    print_int(va_arg(args, unsigned int));
                    break;
            }
        } 
        else print_char(*fmt);
        fmt++;
    }
    return 0;
}   
