#include "my_string.h"
#include "vga.h"
#include "printk.h"
#include "libutils.h"

void kmain()
{
    volatile int text = 0x57695748;
    const char * str1;
    const char * str2 = "testing\n";
    
    int i = 0;
    //while(!i);
    //VGA_clear();
    for (i=0;i<25;i++)
    {
        if(i%2) printk(" %hx %d %p %%\n", 0x57f4, 100, &text); 
        else VGA_display_str(str2);
        // for(int i=0; i <3000000; i++)
        //     asm volatile("");
    } 
    while(1)
    {
        asm volatile(//"mov qword %[0xb8000], $0x2f4b2f4b\n\t"
                    // "mov %eax, $0xb8000 \n\t"
                     "hlt\n\t");
    }    
}
