#include "my_string.h"
#include "vga.h"
#include "printk.h"
#include "keyboard.h"

void kmain()
{
    char * key = "\0";
    int pressed = 0;
    //const char * str2 = "testing\n";
    
    int i = 0;
    //while(!i);
    VGA_clear();
    keyboard_init();
    while(1)
    {
        key = poll_keystroke();
        //printk("\nYou pressed: %hx --> %s\n", scancode, ascii_lookup_tbl[scancode]);
        if(key[0] == '\0') 
        {
            pressed = 1;
            continue;
        }
        else if(pressed)
        {
            pressed = 0;
            continue;
        }
        printk("%s", key);
    }



    // for (i=0;i<20;i++)
    // {
    //     if(i%2) printk(" %hx %d %p %%\n", 0x57f4, 100, &text); 
    //     else VGA_display_str(str2);
    //     // for(int i=0; i <3000000; i++)
    //     //     asm volatile("");
    // } 

    while(1)
    {
        asm volatile(//"mov qword %[0xb8000], $0x2f4b2f4b\n\t"
                    // "mov %eax, $0xb8000 \n\t"
                     "hlt\n\t");
    }    
}
