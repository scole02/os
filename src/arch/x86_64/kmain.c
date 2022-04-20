#include "my_string.h"
#include "vga.h"
#include "printk.h"
#include "keyboard.h"
#include "pic.h"
#include "interrupts.h"
#include "serial.h"

extern const uint16_t GDT64_CODE_OFFSET asm("gdt64.code");
extern const uint64_t GDT64 asm("gdt64.pointer");
    // printk("gdt: %hd\n", GDT64);
    // printk("gddt code offset: %ld\n", GDT64_CODE_OFFSET);
void kmain()
{
    char * key = "\0";
    int pressed = 0;
    //const char * str2 = "testing\n";
    
    int i = 0;
    //while(!i);
    asm("cli\n");
    VGA_clear();
    keyboard_init();
    PIC_init(PIC1, PIC2);
    idt_init();
    keybrd_int_init();
    serial_init();
    init_state(&serial_state);

    SERIAL_write(&serial_state, 11, "lol lol !");
    // producer_add_char('f', &serial_state);
    // producer_add_char('g', &serial_state);
    // producer_add_char('h', &serial_state);
    // printk("%c\n", consumer_next(&serial_state));
    // printk("%c\n", consumer_next(&serial_state));
    // printk("%c\n", consumer_next(&serial_state));
    //start_tx(&serial_state);




    // while(1)
    // {
    //     key = poll_keystroke();
    //     //printk("\nYou pressed: %hx --> %s\n", scancode, ascii_lookup_tbl[scancode]);
    //     if(key[0] == '\0') 
    //     {
    //         pressed = 1;
    //         continue;
    //     }
    //     else if(pressed)
    //     {
    //         pressed = 0;
    //         continue;
    //     }
    //     printk("%s", key);
    // }



    // for (i=0;i<20;i++)
    // {
    //     if(i%2) printk(" %hx %d %p %%\n", 0x57f4, 100, &text); 
    //     else VGA_display_str(str2);
    //     // for(int i=0; i <3000000; i++)
    //     //     asm volatile("");
    // } 

    while(1)
    {
        asm volatile("hlt\n\t"); // such is life
    }   
}

