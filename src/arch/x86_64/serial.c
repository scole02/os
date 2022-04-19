#include "serial.h"
#include "interrupts.h"
#include "pic.h"
#include "libutils.h"
#include "printk.h"
#include <stdint.h>

//#define SER_STATE_BUF_EMPTY(state) {state->consumer == state->producer}

void serial_init() {
    _outb(COM1_PORT + 1, 0x01);    // set transmitter empty interrupt
    _outb(COM1_PORT + 3, 0x80);    // Enable DLAB (set baud rate divisor)
    _outb(COM1_PORT + 0, 0x03);    // Set divisor to 3 (lo byte) 38400 baud
    _outb(COM1_PORT + 1, 0x00);    //                  (hi byte)
    _outb(COM1_PORT + 3, 0x03);    // 8 bits, no parity, one stop bit
    _outb(COM1_PORT + 2, 0xC7);    // Enable FIFO, clear them, with 14-byte threshold
    _outb(COM1_PORT + 4, 0x0B);    // IRQs enabled, RTS/DSR set
    _outb(COM1_PORT + 4, 0x1E);    // Set in loopback mode, test the serial chip
    _outb(COM1_PORT + 0, 0xAE);    // Test serial chip (send byte 0xAE and check if serial returns same byte)
 
   // Check if serial is faulty (i.e: not same byte as sent)
    if(_inb(COM1_PORT + 0) != 0xAE) 
      printk("SERIAL TEST: ERROR\n");
    
    else // If serial is not faulty set it in normal operation mode
    {
    // (not-loopback with IRQs enabled and OUT#1 and OUT#2 bits enabled)
        printk("SERIAL TEST: PASSED\n");
        _outb(COM1_PORT + 4, 0x0F);  
    }

    IRQ_clear_mask(COM1_IRQ_LINE);
}

int start_tx(SerialState *state)
{
    short c = 0;
    if((state->consumer == state->producer)) // check if buffer is empty, kind of redundant
        return 1;
    if((c = consumer_next(state)) < 0) // consumer fails to return value
        return 2;

    _outb(COM1_PORT, (uint8_t)c);
    
}
void SERIAL_write(SerialState *state, uint8_t c)
{
    uint8_t line_status = 0;
    if(producer_add_char(c, state))
        printk("failed to add char to buf\n");
    line_status = _inb(COM1_PORT+5);
    if ((state->consumer != state->producer) && (line_status & TRANS_EMPTY_MASK)) // buffer is not empty and hw is idle
        start_tx(state);
}       

void init_state(struct SerialState *state)
{
    state->consumer = &state->buff[0];
    state->producer = &state->buff[0];
}

short consumer_next(struct SerialState *state)
{
    uint8_t enable_ints = 0;
    if(are_interrupts_enabled())
    {
        enable_ints = 1;
        CLI;
    }
    int res = -1;
    if (state->consumer == state->producer) // buffer is empty
    {
        return res; // might have problems returning neg
        if(enable_ints) STI;
    }    

    //consume_byte(*state->consumer++);
    res = *(state->consumer);
    (state->consumer)++;
    if (state->consumer >= &state->buff[BUFF_SIZE])
        state->consumer = &state->buff[0];
    if(enable_ints) STI;
    return res;
}
int producer_add_char(char toAdd, struct SerialState *state)
{
    // buffer is full
    if (state->producer == state->consumer - 1 ||
        (state->consumer == &(state->buff[0]) && state->producer == &(state->buff[BUFF_SIZE-1])))
        return 1; // not added
    
    *state->producer++ = toAdd;
    if (state->producer >= &state->buff[BUFF_SIZE])
        state->producer = &state->buff[0];
    return 0;
}