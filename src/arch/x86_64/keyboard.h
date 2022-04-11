#ifndef KEYBOARD_H
#define KEYBOARD_H
#include <stdint.h>

void keyboard_init();
char * poll_keystroke();

char ascii_lookup_tbl[128][5]; // scancode --> chararray, 0x16 --> "1" 


#endif