#ifndef INTERRUPTS_H
#define INTERRUPTS_H
#include <stdint.h>

#define CLI {asm volatile ("cli\n");}
#define STI {asm volatile ("sti\n");}

typedef struct {
	uint16_t    isr_low;      // The lower 16 bits of the ISR's address
	uint16_t    kernel_cs;    // The GDT segment selector that the CPU will load into CS before calling the ISR
	uint8_t	    ist;          // The IST in the TSS that the CPU will load into RSP; set to zero for now
	uint8_t     attributes;   // Type and attributes; see the IDT page
	uint16_t    isr_mid;      // The higher 16 bits of the lower 32 bits of the ISR's address
	uint32_t    isr_high;     // The higher 32 bits of the ISR's address
	uint32_t    reserved;     // Set to zero
} __attribute__((packed)) idt_entry_t;

__attribute__((aligned(0x10))) 
static idt_entry_t idt[256]; // Create an array of IDT entries; aligned for performance

typedef struct {
	uint16_t	limit;
	uint64_t	base;
} __attribute__((packed)) idtr_t;

typedef struct {
	uint16_t    seg_limit_low;   
	uint16_t    addr_low1;    
	uint8_t	    addr_mid2;          
	uint8_t     attributes;  
	uint8_t     seg_limit_high: 4;
	uint8_t     AVL: 1;
	uint8_t     : 2;
	uint8_t     G: 1;
	uint8_t     addr_mid3;
	uint32_t    addr_high4;      
	uint32_t    reserved;    
} __attribute__((packed)) tss_desc_t; // gets loaded into GDT

typedef struct {
	uint16_t limit_low;
	uint16_t base_low;
	uint8_t base_middle;
	uint8_t access;
	uint8_t limit_and_flags;
	uint8_t base_high;
} __attribute__ ((packed)) gdt_entry_t;


//uint32_t tss[26];

typedef void (*irq_handler_t)(int, int, void*);
extern void IRQ_set_handler(int irq, irq_handler_t handler, void *arg);
void idt_set_descriptor(uint8_t vector, void* isr, uint8_t flags, uint8_t ist);
void exception_handler(uint8_t isr_num);
void err_exception_handler(uint8_t isr_num, uint64_t error_code);
void keybrd_int_init();
void idt_init(void);
uint8_t are_interrupts_enabled();
//void init_tss(void * gdt_addr);



#endif
