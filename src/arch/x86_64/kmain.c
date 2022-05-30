#include "my_string.h"
#include "libutils.h"
#include "vga.h"
#include "printk.h"
#include "keyboard.h"
#include "pic.h"
#include "interrupts.h"
#include "serial.h"
#include "multiboot.h"
#include "gdt.h"
#include "memory.h"

//extern const uint16_t GDT64_CODE_OFFSET asm("gdt64.code");
extern const uint64_t GDT64 asm("gdt64");
    // printk("gdt: %hd\n", GDT64);
    // printk("gddt code offset: %ld\n", GDT64_CODE_OFFSET);
void kmain (void * multiboot_addr)
{
    int pressed = 0;    
    int i = 0;

    asm("cli\n");
    init_gdt();
    VGA_clear();
    keyboard_init();
    
    PIC_init(PIC1, PIC2);
    idt_init();
    keybrd_int_init();
    serial_init();
    init_serial_state(&serial_state);

    printk("printk test cases:\n"); 
    printk("int: %d\n", 100);
    printk("unisigned: %x\n", &i);
    printk("char: %c\n", 'c');
    printk("ulong: %qd %qu\n", 0xffffffffffffffff, 0xffffffffffffffff);
    printk("long: %qd %qx\n", 0xfffffffffffffff,  0xfffffffffffffff);    
    printk("ushort: %hd %hu\n", 0xffff,  0xffff); 
    printk("short: %hd %hx\n", 0x7fff, 0x7fff);
    printk("uquad: %qd %qu\n", 0xffffffffffffffff, 0xffffffffffffffff);
    printk("quad: %qd %qx\n", 0xfffffffffffffff,  0xfffffffffffffff);

    //asm("int $10\n"); // bad way of testing, should trigger a page fault and step through with gdb to check stack

    // parse_multiboot_tags(multiboot_addr);
    // init_pf_allocater();
    //printk("Gimme some pages!\n");
    
    // void * first_pf, * pf;
    // first_pf= MMU_pf_alloc();
    // for(int j=0; j<512; j++) 
    //     ((uint64_t*)first_pf)[j] = (uint64_t)first_pf;  

    // #define NUM_PAGES 32452
    // for(i=1; i<NUM_PAGES; i++)
    // {
    //     pf= MMU_pf_alloc();
    //     //serialise_64bit((char*)pf, (uint64_t)pf);
    //     for(int j=0; j<512; j++) 
    //         ((uint64_t*)pf)[j] = (uint64_t)pf;
    // }
    // printk("done allocating\n");
    // for(pf = first_pf; pf < first_pf + (PAGE_SIZE*NUM_PAGES); pf += PAGE_SIZE)
    // {
    //     //if ((uint64_t)pf != *((uint64_t*)pf))
    //         //printk("pg: %x contents: %lx\n", pf, *((uint64_t*)pf));
    //     for(int j=0; j<512; j++)
    //         if(((uint64_t*)pf)[j] != (uint64_t)pf) printk("wrong: %lx ", ((uint64_t*)pf)[j]);
    // }
    // printk("done checking\n");



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
    //     //printk("%d", strlen(key));s
    //     //SERIAL_write(&serial_state, strlen(key), key);
    //     printk("%s", key);
    // }


    while(1)
    {
        asm volatile("hlt\n\t"); // such is life
    }   
}


