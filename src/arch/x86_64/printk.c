#include "vga.h"
#include "libutils.h"
#include <stdarg.h>
#include <stdint.h>

#define BASE16 16
#define BASE10 10



void print_char(char c) { VGA_display_char(c); }
void print_str(const char *str) { VGA_display_str(str); }
void print_uchar(unsigned char c)
{
    char * num_str = "\0";
    itoa(c, num_str, BASE10);
    VGA_display_str(num_str); 
}

void print_int(int i)
{   
    char * num_str = "\0";
    itoa(i, num_str, BASE10);
    VGA_display_str(num_str); 
}

void print_short(short h, int base)
{   
    char * num_str = "\0";
    htoa(h, num_str, base);
    VGA_display_str(num_str); 
}

void print_long_hex(long l, int base)
{   
    char * num_str = "\0";
    ltoa(l, num_str, base);
    VGA_display_str(num_str); 
}
void print_ptr(void * p)
{
    char * num_str = "\0";
    ltoa((uintptr_t)p, num_str, BASE16);
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
                case 'u':
                    print_int(va_arg(args, unsigned int));
                    break;
                case 'x':
                    print_long_hex(va_arg(args, long), BASE16);
                    break;
                case 'c':
                    print_char(va_arg(args, int));
                    break;
                case 'p':
                    print_ptr(va_arg(args, void *));
                    break;
                case 'l':
                    fmt++;
                    if (*fmt == 'd') print_long_hex(va_arg(args, long), BASE10);
                    else if (*fmt == 'u') print_long_hex(va_arg(args, unsigned long), BASE10);
                    else if (*fmt == 'x') print_long_hex(va_arg(args, long), BASE16);
                    break;
                case 'h':
                    fmt++;
                    if (*fmt == 'd') print_short(va_arg(args, int), BASE10);
                    else if (*fmt == 'u') print_short(va_arg(args, unsigned int), BASE10);
                    else if (*fmt == 'x') print_short(va_arg(args, int), BASE16);
                    break;    
                case 'q':
                    fmt++;
                    if (*fmt == 'd') print_long_hex(va_arg(args, long), BASE10);
                    else if (*fmt == 'u') print_long_hex(va_arg(args, unsigned long), BASE10);
                    else if (*fmt == 'x') print_long_hex(va_arg(args, long), BASE16);
                    break;                 


                case 's':
                    print_str(va_arg(args, char *));
                    break;
                

                    

                
                
            }
        } 
        else print_char(*fmt);
        fmt++;
    }
    return 0;
}   
