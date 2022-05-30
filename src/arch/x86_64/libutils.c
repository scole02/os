// taken from osdev: https://wiki.osdev.org/Printing_To_Screen
#include <stdint.h>

#define ASCII_CHARS

void itoa( int value, char * str, int base)
{
    char * rc;
    char * ptr;
    char * low;
    ptr = str;
    // Set '-' for negative decimals.
    if ( value < 0 && base == 10 )
    {
        *ptr++ = '-';
    }
    // Remember where the numbers start.
    low = ptr;
    // The actual conversion.
    do
    {
        // Modulo is negative for negative value. This trick makes abs() unnecessary.
        *ptr++ = "zyxwvutsrqponmlkjihgfedcba9876543210123456789abcdefghijklmnopqrstuvwxyz"[35 + value % base];
        value /= base;
    } while ( value );
    // Terminating the string.
    *ptr-- = '\0';
    // Invert the numbers.
    while ( low < ptr )
    {
        char tmp = *low;
        *low++ = *ptr;
        *ptr-- = tmp;
    }
}
void htoa( short value, char * str, int base)
{
    char * rc;
    char * ptr;
    char * low;
    ptr = str;
    // Set '-' for negative decimals.
    if ( value < 0 && base == 10 )
    {
        *ptr++ = '-';
    }
    // Remember where the numbers start.
    low = ptr;
    // The actual conversion.
    do
    {
        // Modulo is negative for negative value. This trick makes abs() unnecessary.
        *ptr++ = "zyxwvutsrqponmlkjihgfedcba9876543210123456789abcdefghijklmnopqrstuvwxyz"[35 + value % base];
        value /= base;
    } while ( value );
    // Terminating the string.
    *ptr-- = '\0';
    // Invert the numbers.
    while ( low < ptr )
    {
        char tmp = *low;
        *low++ = *ptr;
        *ptr-- = tmp;
    }
}
void ultoa( uint64_t value, char * str, int base)
{
    char * rc;
    char * ptr;
    char * low;
    ptr = str;
    // Set '-' for negative decimals.
    if ( value < 0 && base == 10 )
    {
        *ptr++ = '-';
    }
    // Remember where the numbers start.
    low = ptr;
    // The actual conversion.
    do
    {
        // Modulo is negative for negative value. This trick makes abs() unnecessary.
        *ptr++ = "zyxwvutsrqponmlkjihgfedcba9876543210123456789abcdefghijklmnopqrstuvwxyz"[35 + value % base];
        value /= base;
    } while ( value );
    // Terminating the string.
    *ptr-- = '\0';
    // Invert the numbers.
    while ( low < ptr )
    {
        char tmp = *low;
        *low++ = *ptr;
        *ptr-- = tmp;
    }
}

void ltoa( long value, char * str, int base)
{
    char * rc;
    char * ptr;
    char * low;
    ptr = str;
    // Set '-' for negative decimals.
    if ( value < 0 && base == 10 )
    {
        *ptr++ = '-';
    }
    // Remember where the numbers start.
    low = ptr;
    // The actual conversion.
    do
    {
        // Modulo is negative for negative value. This trick makes abs() unnecessary.
        *ptr++ = "zyxwvutsrqponmlkjihgfedcba9876543210123456789abcdefghijklmnopqrstuvwxyz"[35 + value % base];
        value /= base;
    } while ( value );
    // Terminating the string.
    *ptr-- = '\0';
    // Invert the numbers.
    while ( low < ptr )
    {
        char tmp = *low;
        *low++ = *ptr;
        *ptr-- = tmp;
    }
}

// for reading/wirting ports
inline void _outb(uint16_t port, uint8_t val)
{
    asm volatile ( "outb %0, %1" : : "a"(val), "Nd"(port) );
}

inline uint8_t _inb(uint16_t port)
{
    uint8_t ret;
    asm volatile ( "inb %1, %0"
    : "=a"(ret)
    : "Nd"(port) );
    return ret;
}

// inline void iowait()
// {
//     for(int i=0; i<1000000;i++);
// }

inline void iowait()
{
    _outb(0x80, 0);
}

void serialise_64bit(uint8_t dest[8], uint64_t n)
{
    dest[7] = (n >> 56) & 0xff;
    dest[6] = (n >> 48) & 0xff;
    dest[5] = (n >> 40) & 0xff;
    dest[4] = (n >> 32) & 0xff;
    dest[3] = (n >> 24) & 0xff;
    dest[2] = (n >> 16) & 0xff;
    dest[1] = (n >>  8) & 0xff;
    //dest[0] = (n >>  0) & 0xff;  
}
// uint64_t serialise_64bit(uint8_t src[8])
// {
//     uint64_t n = 0;
//     n |= src[0] << 56;
//     n |= src[1] << 48;
//     n |= src[2] << 40;
//     n |= src[3] << 32;
//     n |= src[4] << 24;
//     n |= src[5] << 16;
//     n |= src[6] << 8;
//     n |= src[7];
//     return n;
//}