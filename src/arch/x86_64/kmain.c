#include "my_string.h"
#include "vga.h"
void kmain()
{
    volatile int text = 0x57695748;
    VGA_clear();
    VGA_display_char('H');
    VGA_display_char('E');
    VGA_display_char('L');
    VGA_display_char('L');
    VGA_display_char('O');
    for(int i=0; i <100000000; i++)
       asm volatile(""); 
    scroll_text();
    VGA_display_char(' ');
    VGA_display_char('W');
    VGA_display_char('O');
    VGA_display_char('R');
    VGA_display_char('L');
    VGA_display_char('D');
    //memcpy((int*)0xb8000, (void*)&text, 4); // write text
    //*((int*)0xb8000)=0x57695748; // write hi in purple
    while(1)
    {
        asm volatile(//"mov qword %[0xb8000], $0x2f4b2f4b\n\t"
                    // "mov %eax, $0xb8000 \n\t"
                     "hlt\n\t");
    }    
}
