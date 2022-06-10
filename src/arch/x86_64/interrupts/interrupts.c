
#include "interrupts.h"
#include "../hw/keyboard.h"
#include "../printk.h"
#include "pic.h"
#include "../libutils.h"
#include "../hw/serial.h"
#include "../my_string.h"

#include <stdint.h>


#define GDT_SIZE 64
#define IDT_MAX_DESCRIPTORS 256




static idtr_t idtr;
static uint32_t tss[26];
uint8_t c_gdt[GDT_SIZE];

//extern const uint16_t GDT64_CODE_OFFSET asm("gdt64.code");
const uint16_t GDT64_CODE_OFFSET = 0x08;
extern void* isr_stub_table[];


void kb_exception_handler(uint8_t isr_num) 
{
    char* key = "\0";
    key = get_key();
    if(key[0] != '\0') // valid keypress
        printk("%s", key);
        
    PIC_sendEOI(isr_num - PIC1); // a lil jank, its just 1 for kb
    //__asm__ volatile ("cli"); // Completely hangs the computer
}

void serial_exception_handler(uint8_t isr_num)
{

    uint8_t err = 0, line_status = 0;
    uint8_t iir_status = _inb(COM1_PORT + 2);
    // determine what kind of interrupt we got

    // TRANSMIT BUFFER EMPTY INTERRUPT
    if ((iir_status & IIR_TRANS_MASK) == IIR_TRANS_MASK) 
    {
        //printk("In Trasnmit buf\n");
        SERIAL_buf_busy = 0; // clear busy flag
        if((err = start_tx(&serial_state))) printk("ERROR %d\n", err);
    }
    // LINE INTERRUPT, shouldnt happen
    else if((iir_status & IIR_LINE_MASK) == IIR_LINE_MASK)// kind of gross but line mask is b0110
    {
        printk("ERROR: SERIAL LINE INTERRUPT \n"); 
        line_status = _inb(COM1_PORT+5); // clear line int by reading LSR

    }
    // Shouldnt get anything else unless interrupts not enabled correctly   
    else printk("ERROR with Serial Interrupt\n");
    
    PIC_sendEOI(isr_num - PIC1);
}

void exception_handler(uint8_t isr_num)
{
    register int *sp asm("rsp"); // for checking stack address
    if (isr_num == PIC1 + 1) // keyboard interrupt number
    {
        //printk("%x\n", sp);   
        kb_exception_handler(isr_num);
    }
    else if(isr_num == PIC1 + 4) // COM1 uart line
        serial_exception_handler(isr_num);
    else printk("Interrupt: %d not handled\n", isr_num);
}

void err_exception_handler(uint8_t isr_num, uint64_t error_code)
{
    printk("ERROR: %lx\nInterrupt: %d\n", isr_num, isr_num);
}

void idt_set_descriptor(uint8_t vector, void* isr, uint8_t flags, uint8_t ist) 
{
    idt_entry_t* descriptor = &idt[vector];
 
    descriptor->isr_low       = (uint64_t)isr & 0xFFFF;
    descriptor->kernel_cs     = GDT64_CODE_OFFSET;
    descriptor->ist           = ist;
    descriptor->attributes    = flags;
    descriptor->isr_mid       = ((uint64_t)isr >> 16) & 0xFFFF;
    descriptor->isr_high      = ((uint64_t)isr >> 32) & 0xFFFFFFFF;
    descriptor->reserved      = 0;
}
 
void idt_init() 
{
    //uint8_t ist = 0;
    idtr.base = (uintptr_t)&idt[0];
    idtr.limit = (uint16_t)sizeof(idt_entry_t) * IDT_MAX_DESCRIPTORS - 1;
 
    for (uint8_t vector = 0; vector < 255; vector++) 
    {
        if(vector == 13 || vector == 14) idt_set_descriptor(vector, isr_stub_table[vector], 0x8E, 1);
        else idt_set_descriptor(vector, isr_stub_table[vector], 0x8E, 0);
    }
 
    __asm__ volatile ("lidt %0" : : "m"(idtr)); // load the new IDT
    __asm__ volatile ("sti"); // set the interrupt flag
}

void keybrd_int_init(void) 
{
    IRQ_clear_mask(KEYBOARD_INT_LINE);
}

uint8_t are_interrupts_enabled()
{
    unsigned long flags;
    asm volatile("pushf; pop %0"
                : "=rm" (flags)
                : /* no input */
                : "memory");
    return (flags & 0x200 ) != 0;
}


// void init_tss(void * gdt_addr)
// {
//     static tss_desc_t tss_desc;
//     uint64_t tss_offset = 16;
//     memset(&tss_desc, 0, sizeof(tss_desc_t));
//     // set address to tss global table 
//     tss_desc.addr_low1  = (uint64_t)tss & 0xFFFF;
//     tss_desc.addr_mid2  = ((uint64_t)tss & 0xFF0000) >> 16;
//     tss_desc.addr_mid3  = ((uint64_t)tss & 0xFF000000) >> 24;
//     tss_desc.addr_high4 = ((uint64_t)tss & 0xFFFFFFFF00000000) >> 32;
//     tss_desc.attributes = 0x89;
//     tss_desc.G = 1;
//     printk("Og addr: %x\n", tss);
//     printk("addr: %hx %hx %hx %lx\n", tss_desc.addr_low1, tss_desc.addr_mid2, tss_desc.addr_mid3, tss_desc.addr_high4);
//     memset(c_gdt, 0, GDT_SIZE);
//     memcpy(c_gdt, gdt_addr, GDT_SIZE); // copy contents of old gdt: 0 entry and code segment
//     memcpy(c_gdt + 16, &tss_desc, sizeof(tss_desc));
//     //memcpy(gdt_addr + 16, &tss_desc, sizeof(tss_desc));
    

//     //memcpy(c_gdt+16, &tss_desc, sizeof(tss_desc)); // put tss in gdt
//     lgdt(c_gdt, GDT_SIZE);
//     asm volatile("mov $0x10, %%ax\n"
//                  "ltr %%ax\n"
//                  : : "AN" (tss_offset));
    
// }

