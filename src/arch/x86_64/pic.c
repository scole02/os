#include "pic.h"
#include "libutils.h"
#include <stdint.h>
/*
arguments:
	offset1 - vector offset for master PIC
		vectors on the master become offset1..offset1+7
	offset2 - same for slave PIC: offset2..offset2+7
*/



void PIC_remap(int offset1, int offset2)
{
	unsigned char a1, a2;
 
	a1 = _inb(PIC1_DATA);                        // save masks
	a2 = _inb(PIC2_DATA);
 
	_outb(PIC1_COMMAND, ICW1_INIT | ICW1_ICW4);  // starts the initialization sequence (in cascade mode)
	iowait();
	_outb(PIC2_COMMAND, ICW1_INIT | ICW1_ICW4);
	iowait();
	_outb(PIC1_DATA, offset1);                 // ICW2: Master PIC vector offset
	iowait();
	_outb(PIC2_DATA, offset2);                 // ICW2: Slave PIC vector offset
	iowait();
	_outb(PIC1_DATA, 4);                       // ICW3: tell Master PIC that there is a slave PIC at IRQ2 (0000 0100)
	iowait();
	_outb(PIC2_DATA, 2);                       // ICW3: tell Slave PIC its cascade identity (0000 0010)
	iowait();
 
	_outb(PIC1_DATA, ICW4_8086);
	iowait();
	_outb(PIC2_DATA, ICW4_8086);
	iowait();
 
	_outb(PIC1_DATA, a1);   // restore saved masks.
	_outb(PIC2_DATA, a2);
}

void PIC_sendEOI(unsigned char irq)
{
	if(irq >= 8)
		_outb(PIC2_COMMAND,PIC_EOI);
 
	_outb(PIC1_COMMAND,PIC_EOI);
}

//each pic has a bitmap (1byte) for 8 irqlines.
//When a bit is set, the PIC ignores the request 

void IRQ_set_mask(unsigned char irqline) {
    // ignore irqline
    uint16_t port;
    uint8_t value;
 
    if(irqline < 8) {
        port = PIC1_DATA;
    } else {
        port = PIC2_DATA;
        irqline -= 8;
    }
    value = _inb(port) | (1 << irqline);
    _outb(port, value);        
}

int IRQ_get_mask(unsigned char irqline)
{
    uint16_t port;
    uint8_t value;
    if(irqline < 8) {
        port = PIC1_DATA;
    } else {
        port = PIC2_DATA;
        irqline -= 8;
    }
    return _inb(port);
}

void IRQ_clear_mask(unsigned char irqline) 
{
    // enable irqline
    uint16_t port;
    uint8_t value;
 
    if(irqline < 8) {
        port = PIC1_DATA;
    } else {
        port = PIC2_DATA;
        irqline -= 8;
    }
    value = _inb(port) & ~(1 << irqline);
    _outb(port, value);        
}

void IRQ_set_all_masks()
{
    // disables all hardware interrupts
    _outb(PIC1_DATA, 0xFF);
    _outb(PIC2_DATA, 0xFF);
}

void PIC_init(int offset1, int offset2)
{
    PIC_remap(offset1, offset2);
    IRQ_set_all_masks();
}