#include "my_string.h"
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
    char * key = "this is a test\n";
    int pressed = 0;    
    int i = 0;
    
    // //while(!i);
    // typedef struct {
    //     uint16_t length;
    //     void* base;
    // } __attribute__((packed)) GDTR;
    // GDTR *gdt; 
    // //struct GDTR * gdt;
    // //uint64_t * gdt_ptr;
    // asm("mov %0, gdtr"
    //     :
    //     :"r"(gdt));

    asm("cli\n");
    init_gdt();
    VGA_clear();
    keyboard_init();
    
    PIC_init(PIC1, PIC2);
    idt_init();
    keybrd_int_init();
    serial_init();
    init_state(&serial_state);
    
    //asm("int $0x0");
    //init_tss((void*)GDT64);
    //printk("break %d\n", 1/0);
    // printk("gdt_ptr: %x", GDT64);
    // printk("gdt_ptr + 1: %x\n", *(&GDT64 +1));

    parse_multiboot_tags(multiboot_addr);

    init_pf_allocater();
    printk("Gimme some pages!\n");
    void * pf;
    for(i = 0; i<10; i++)
    {
        printk("%x\n", pf=MMU_pf_alloc());
        MMU_pf_free(pf);
        printk("freed %x\n", pf);
    }




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

// TODO from "Handle Interrupts" Milestone

// add assembly code for special error ints
// print error mesage for unhandled errors
// setup and configure tss

