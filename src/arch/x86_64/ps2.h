#include <stdint.h>
#ifndef PS2_H
#define PS2_H

void ps2_init();
void get_ps2_response(uint8_t * resp); // used to get data from PS2 devs
void send_ps2_controller_cmd(uint8_t cmd);
void send_ps2_controller_data(uint8_t data);
void send_ps2_dev_cmd(uint8_t cmd, uint8_t data, uint8_t send_data); // if !send_data only send one byte


#define CMD_PORT 0x64 // read for status register, write to send cmd
#define DATA_PORT 0x60

// controller cmds
#define DISABLE_PS2_PORT1 0xAD
#define DISABLE_PS2_PORT2 0xA7
#define ENABLE_PS2_PORT1 0xAE
#define READ_CONFIG 0x20
#define WRITE_CONFIG 0x60
#define TEST_PS2_CONTROLLER 0xAA 
#define TEST_PS2_PORT1 0xAB

// keyboard dev cmds
#define KEY_RESET 0xFF
#define KEY_SCAN_CODES 0xF0
#define KEY_EN_SCANNING 0xF4

// responses
#define DATA_ACK 0xFA
#define DATA_RESEND 0xFE
#define PS2_CONT_TEST_PASS 0x55
#define PS2_CONT_TEST_FAIL 0xFC
#define PS2_PORT_TEST_PASS 0x00
#define DEV_SELF_TEST_PASS 0xAA

// misc
#define SCAN_CODE_SET2 2 
#define SEND_DATA 1
#define NO_DATA 0

// bitfields for big endian systems

struct PS2StatusReg { // accquired from reading port 0x64
    uint8_t out_buf:1; // must be set before attempting to read data from IO port 0x60
    uint8_t in_buf:1; // must be clear before attempting to write data to IO port 0x60 or IO port 0x64
    uint8_t sys_flag:1; // Meant to be cleared on reset and set by firmware
    // 0 = data written to input buffer is data for PS/2 device 1 = data written to input buffer is data for PS/2 controller command
    uint8_t cmd_data:1; 
    uint8_t :1;
    uint8_t :1;
    uint8_t timeout_err:1; // 1 = error
    uint8_t parity_err:1; // 1 = error
} __attribute__((packed)) typedef PS2StatusReg;

struct PS2ControllerConfig { // read with cmd 0x20 and written with 0x60
    uint8_t intr1:1; // 1 = interrupts enabled
    uint8_t intr2:1; 
    uint8_t sys_flag:1; // 1 = system passed POST
    uint8_t :1; // should be 0
    uint8_t clk1:1; // 0 = enabled
    uint8_t clk2:1; //
    uint8_t port_translation:1; // 1 = enabled
    uint8_t zero:1; // must be 0
} __attribute__((packed)) typedef PS2ControllerConfig;

#endif