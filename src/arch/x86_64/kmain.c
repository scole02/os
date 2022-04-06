#include "my_string.h"
#include "vga.h"
void kmain()
{
    volatile int text = 0x57695748;
    const char * str1 = "string\n";
    const char * str2 = "lol\n";
    
    int i = 0;
   // while(!i);
    //VGA_clear();
    for (i=0;i<27;i++)
    {
        if(i%2) VGA_display_str(str1); 
        else VGA_display_str(str2);
        for(int i=0; i <30000000; i++)
            asm volatile("");
    } 
    VGA_display_char('\n');
    VGA_display_str(str2); 
// VGA_display_char('H');
   // VGA_display_char('E');
   // VGA_display_char('L');
   // VGA_display_char('L');
   // VGA_display_char('O');
   // scroll_text();
   // VGA_display_char(' ');
   // VGA_display_char('W');
   // VGA_display_char('O');
   // VGA_display_char('R');
   // VGA_display_char('L');
   // VGA_display_char('D');
    //memcpy((int*)0xb8000, (void*)&text, 4); // write text
    //*((int*)0xb8000)=0x57695748; // write hi in purple
    while(1)
    {
        asm volatile(//"mov qword %[0xb8000], $0x2f4b2f4b\n\t"
                    // "mov %eax, $0xb8000 \n\t"
                     "hlt\n\t");
    }    
}
