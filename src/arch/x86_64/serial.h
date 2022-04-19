#ifndef SERIAL_H
#define SERIAL_H

#define BUFF_SIZE 8
#define COM1_PORT 0x3f8          // COM1 --> IRQ: PIC1+COM1_IRQ_LINE
#define COM1_IRQ_LINE 4
#define TRANS_EMPTY_MASK 0x40 // bit 6 is set if transmitter(TX) is idle

struct SerialState {
    char buff[BUFF_SIZE];
    char *consumer, *producer;
} typedef SerialState;

SerialState serial_state;

void serial_init();
void init_state(struct SerialState *state);
short consumer_next(struct SerialState *state); // returns next element in char buf or -1
int producer_add_char(char toAdd, struct SerialState *state);

#endif