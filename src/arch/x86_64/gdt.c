#include "gdt.h"
#include "my_string.h"
#include "printk.h"

#define IST_SIZE 256

TSS tss_c;
GDTDescriptor gdt_desc;
uint64_t ist1[IST_SIZE];
uint64_t ist2[IST_SIZE];

__attribute__((aligned(0x1000)))
static GDT gdt_c = {
    {0, 0, 0, 0x00, 0x00, 0}, // null
    {0, 0, 0, 0x9a, 0xa0, 0}, // code segment
    {0, 0, 0, 0x92, 0xa0, 0}, // data segment
	{0, 0, 0, 0x89, 0, 0, 0} // tss
};


void init_gdt(void)
{
	
	printk("stack 1: %x\n", ist1+IST_SIZE);
	tss_c.ist[0] = ist1 + (IST_SIZE-1);
	tss_c.ist[1] = ist1 + (IST_SIZE-1);

	gdt_c.tss.addr_low1 = ((uint64_t)&tss_c & 0xFFFF); 
	gdt_c.tss.addr_mid2 = (((uint64_t)&tss_c >> 16) & 0xFF); 
	gdt_c.tss.addr_mid3 = (((uint64_t)&tss_c >> 24) & 0xFF); 
	gdt_c.tss.addr_high4 = (((uint64_t)&tss_c >> 32) & 0xFFFFFFFF); 
	//gdt_c.tss.G = 1;
	gdt_c.tss.seg_limit_low = 200;
	gdt_desc.Size = sizeof(gdt_c);
	gdt_desc.Offset = (uint64_t)&gdt_c;
	ldGDT(&gdt_desc);
	//ldTSS();
}

