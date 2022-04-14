#ifndef LIBUTILS_H
#define LIBUTILS_H
#include <stdint.h>

void itoa( int value, char * str, int base);
void ltoa( long value, char * str, int base);
void htoa( short value, char * str, int base);
inline void _outb(uint16_t port, uint8_t val);
inline uint8_t _inb(uint16_t port);
inline void iowait();


#endif