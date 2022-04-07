#include "my_string.h"
#include "vga.h"
#include "printk.h"
#include "libutils.h"

void kmain()
{
    volatile int text = 0x57695748;
    const char * str1;
    const char * str2 = "lol\n";
    
    int i = 0;
    //while(!i);
    //VGA_clear();
    itoa(1, str1, 10);
    for (i=0;i<25;i++)
    {
        if(i%2) printk(" %x %d %p %%\n", 0x1ffffffff, 100, str1); 
        else VGA_display_str(str2);
        // for(int i=0; i <3000000; i++)
        //     asm volatile("");
    } 
    // for(int i=0; i <30000000; i++)
    //     asm volatile("");
    // VGA_display_str("verrryyyyyyyyyyyyyy loooooonnnnnnggggggggggggg sssssstttttttrrrrrrrriiiinnnnnggg");
    // for(int i=0; i <80000000; i++)
    //     asm volatile("");
    // VGA_display_str(str2);
    // short * VGA_b = 0xb8000;
 
    while(1)
    {
        asm volatile(//"mov qword %[0xb8000], $0x2f4b2f4b\n\t"
                    // "mov %eax, $0xb8000 \n\t"
                     "hlt\n\t");
    }    
}
