#include "my_string.h"
#include "libutils.h"
#include "hw/vga.h"
#include "printk.h"
#include "hw/keyboard.h"
#include "interrupts/pic.h"
#include "interrupts/interrupts.h"
#include "hw/serial.h"
#include "memory/multiboot.h"
#include "interrupts/gdt.h"
#include "memory/phys_memory.h"
#include "memory/virt_memory.h"

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

    //printk("offset: %d, pt: %d, pd: %d, pdpt: %d, pml4: %d", v->offset, v->pt_index, v->pd_index, v->pdpt_index, v->pml4_index);

    //asm("int $10\n"); // bad way of testing, should trigger a page fault and step through with gdb to check stack

    parse_multiboot_tags(multiboot_addr);
    init_pf_allocater();
    void * pml4 = MMU_pf_alloc(); 
    memset(pml4, 0, PAGE_SIZE);
    init_virt_memory(pml4);





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


    // while(1)
    // {
    //     asm volatile("hlt\n\t"); // such is life
    // }   
}


