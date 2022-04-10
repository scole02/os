#include <stdint.h>
#ifndef PS2_H
#define PS2_H

void ps2_init();
void keyboard_test();

// bitfields for big endian systems

struct PS2StatusReg { // accquired from reading port 0x64
    uint8_t out_buf:1; // must be set before attempting to read data from IO port 0x60
    uint8_t in_buf:1; // must be clear before attempting to write data to IO port 0x60 or IO port 0x64
    uint8_t sys_flag:1; // Meant to be cleared on reset and set by firmware
    // 0 = data written to input buffer is data for PS/2 device 1 = data written to input buffer is data for PS/2 controller command
    uint8_t cmd_data:1; 
    uint8_t unused1:1;
    uint8_t unused2:1;
    uint8_t timeout_err:1; // 1 = error
    uint8_t parity_err:1; // 1 = error
} __attribute__((packed)) typedef PS2StatusReg;

struct PS2ControllerConfig { // read with cmd 0x20 and written with 0x60
    uint8_t intr1:1; // 1 = interrupts enabled
    uint8_t intr2:1; 
    uint8_t sys_flag:1; // 1 = system passed POST
    uint8_t unused1:1; // should be 0
    uint8_t clk1:1; // 0 = enabled
    uint8_t clk2:1; //
    uint8_t port_translation:1; // 1 = enabled
    uint8_t zero:1; // must be 0
} __attribute__((packed)) typedef PS2ControllerConfig;

#endif