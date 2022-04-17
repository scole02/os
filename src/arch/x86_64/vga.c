#include "my_string.h"
#include "interrupts.h"

#define VGA_BASE_ADDR 0xb8000
#define VGA_COLS 80
#define VGA_ROWS 25
#define COLOR 0x5700 // OR'ed with char
#define VGA_CHAR_SIZE 2

int cur_byte_offset = 0; // location in bytes of next char to be written
unsigned short * VGA_buf = (short*)VGA_BASE_ADDR;

void VGA_clear(void)
{
    // set entire buffer to 0, each vga location is 2 bytes
    memset(VGA_buf, 0,  VGA_ROWS * VGA_COLS * VGA_CHAR_SIZE); 
}

static void _scroll_text(void)
{
    unsigned short * second_row = VGA_buf + VGA_COLS;
    memcpy(VGA_buf, second_row,  VGA_CHAR_SIZE * VGA_COLS * (VGA_ROWS-1) );
   
    // set cur to beginning of current line
    cur_byte_offset = cur_byte_offset > VGA_COLS ? cur_byte_offset - (cur_byte_offset % VGA_COLS) : 0; 
     // clear current line
     memset(VGA_buf + cur_byte_offset, 0, VGA_CHAR_SIZE * VGA_COLS);
}
void VGA_display_char(char c)
{
    short int vga_char = COLOR | c;
    uint8_t enable_ints = 0;
    if(are_interrupts_enabled())
    {
        enable_ints = 1;
        CLI;
    }

    if (c == '\n') // newline
    {
        // set offset to beginning of next row
        cur_byte_offset = ((cur_byte_offset/VGA_COLS) * VGA_COLS) + VGA_COLS;
    }
    
    // reached end of display
    if (cur_byte_offset  >= VGA_COLS * VGA_ROWS)
    {
       // set offset to beginning of last row
       cur_byte_offset = VGA_COLS * (VGA_ROWS-1);
       _scroll_text();
        //memcpy(VGA_buf, VGA_buf + VGA_COLS*sizeof(vga_char), sizeof(vga_char) * (VGA_COLS-1) * (VGA_ROWS -1));
    }
    if (c == '\n')
        return;
  
    //memcpy((char*)VGA_BASE_ADDR + cur_byte_offset, &vga_char, sizeof(vga_char)); 
    VGA_buf[cur_byte_offset] = vga_char;
    cur_byte_offset++;

    //if (enable_ints) STI;
}

void VGA_display_str(const char *str)
{
  while(*str)
  {
    VGA_display_char(*str);
    str++;
  }
}

