#ifndef SERIAL_H
#define SERIAL_H

#define BUFF_SIZE 512
#define COM1_PORT 0x3f8          // COM1 --> IRQ: PIC1+COM1_IRQ_LINE
#define COM1_IRQ_LINE 4
#define TRANS_EMPTY_MASK 0x20 // bit 6 is set if transmitter(TX) is idle
#define IIR_TRANS_MASK 0x02
#define IIR_LINE_MASK 0x06

#include <stdint.h>
volatile uint8_t SERIAL_buf_busy; // mimics HW buffer state 

struct SerialState {
    char buff[BUFF_SIZE];
    char *consumer, *producer;
} typedef SerialState;

SerialState serial_state;

void SERIAL_write(SerialState *state, int len, const char* buf);
int start_tx();
void serial_init();
void init_serial_state(struct SerialState *state);
short consumer_next(struct SerialState *state); // returns next element in char buf or -1
int producer_add_char(char toAdd, struct SerialState *state);


#endif