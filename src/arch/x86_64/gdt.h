#ifndef GDT_H
#define GDT_H
#include <stdint.h>

typedef struct  {
    uint16_t Size;
    uint64_t Offset;
} __attribute__((packed)) GDTDescriptor;

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
} __attribute__((packed)) GDTTSSEntry; // gets loaded into GDT

typedef struct  {
    uint16_t Limit0;
    uint16_t Base0;
    uint8_t Base1;
    uint8_t AccessByte;
    uint8_t Limit1_Flags;
    uint8_t Base2;
}__attribute__((packed)) GDTEntry;

typedef struct {
    GDTEntry null; //0x00
    GDTEntry kernel_code; //0x08
    GDTEntry kernel_data; //0x10
	GDTTSSEntry tss;
} __attribute__((packed)) 
__attribute((aligned(0x1000))) GDT;

typedef struct {
	uint32_t reserved1;
	uint64_t rsp[3];
	uint64_t reserved2;
	uint64_t *ist[7];
	uint64_t reserved3;
	uint16_t reserved4_ign;
	uint16_t iomap_baseaddr;
} __attribute__((packed)) TSS;

//extern GDT gdt_c;
extern TSS tss_c;
void init_gdt(void);
extern void ldGDT(GDTDescriptor* gdtDescriptor); // assembly
extern void ldTSS();

#endif