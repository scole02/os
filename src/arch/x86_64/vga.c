#include "my_string.h"

#define VGA_BASE_ADDR 0xb8000
#define VGA_COLS 80
#define VGA_ROWS 25
#define COLOR 0x5700 // OR'ed with char
#define VGA_CHAR_SIZE 2

int cur_byte_offset = 0; // location in bytes of next char to be written

void VGA_clear(void)
{
    // set entire buffer to 0, each vga location is 2 bytes
    memset((void*)VGA_BASE_ADDR, 0,  VGA_ROWS * VGA_COLS * 2); 
}

void scroll_text(void)
{
    char * second_row = (char*)VGA_BASE_ADDR + VGA_COLS*VGA_CHAR_SIZE;
    memcpy((char*)VGA_BASE_ADDR, second_row,  VGA_CHAR_SIZE * (VGA_COLS-1) * (VGA_ROWS -1));
    //memset((char*)VGA_BASE_ADDR + (VGA_ROWS-1)*(VGA_COLS-1)*VGA_CHAR_SIZE, 0, VGA_CHAR_SIZE * (VGA_COLS-1));
    cur_byte_offset -= cur_byte_offset % (VGA_CHAR_SIZE * VGA_COLS); 
}
void VGA_display_char(char c)
{
    short int vga_char = COLOR | c;
    
    if (cur_byte_offset >= (VGA_COLS-1) * (VGA_ROWS-1) * sizeof(vga_char))
    {
        memcpy((char*)VGA_BASE_ADDR, (char*)VGA_BASE_ADDR + VGA_COLS*sizeof(vga_char), sizeof(vga_char) * (VGA_COLS-1) * (VGA_ROWS -1));
    }
    memcpy((char*)VGA_BASE_ADDR + cur_byte_offset, &vga_char, sizeof(vga_char)); 
    cur_byte_offset += sizeof(vga_char);
}

// TODO:
// move cursor after scrolling correctly
// display_str
// detect newlines
// zero out bottom row
