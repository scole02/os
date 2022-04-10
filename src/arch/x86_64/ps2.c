#include "my_string.h"
#include "ps2.h"
#include "printk.h"
#include <stdint.h>

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

static inline void _outb(uint16_t port, uint8_t val)
{
    asm volatile ( "outb %0, %1" : : "a"(val), "Nd"(port) );
}

static inline uint8_t _inb(uint16_t port)
{
    uint8_t ret;
    asm volatile ( "inb %1, %0"
    : "=a"(ret)
    : "Nd"(port) );
    return ret;
}

static void _send_ps2_controller_cmd(uint8_t cmd)
{
    uint8_t status = _inb(CMD_PORT);

    while( ((PS2StatusReg*)&status)->in_buf ) // wait until in_buf is empty
        status = _inb(CMD_PORT);
    
    _outb(CMD_PORT, cmd);
}

static void _get_ps2_response(uint8_t * resp)
{
    uint8_t status = _inb(CMD_PORT);

    while( !((PS2StatusReg*)&status)->out_buf ) // wait until out_buf is full
        status = _inb(CMD_PORT);
    
    *resp = _inb(DATA_PORT); // get response byte
}

static void _send_ps2_controller_data(uint8_t data)
{
    uint8_t status = _inb(CMD_PORT);

    while( ((PS2StatusReg*)&status)->in_buf ) // wait until in_buf is empty
        status = _inb(CMD_PORT);
    
    _outb(DATA_PORT, data);
}

static void _send_ps2_dev_cmd(uint8_t cmd, uint8_t data, uint8_t send_data) // if !send_data only send one byte
{
    uint8_t response = 0, status = _inb(CMD_PORT);

    while( ((PS2StatusReg*)&status)->in_buf ) // wait until in_buf is empty
        status = _inb(CMD_PORT);
    
    _outb(DATA_PORT, cmd);

    // check response here
    _get_ps2_response(&response);
    printk("dev_cmd %hx, response: %hx\n", (short)cmd, (short)response);

    if(send_data == NO_DATA) return;
    
    status = _inb(CMD_PORT);

    while( ((PS2StatusReg*)&status)->in_buf ) // wait until in_buf is empty
        status = _inb(CMD_PORT);
    _outb(DATA_PORT, data);
}

void ps2_init()
{
    // 1. Disable port 1 
    // 2. Disable port 2
    // 3. Read byte 0 from RAM (current configuration)
    // 4. Modify the bits in the configuration so the first clock and first 
    //    interrupt are enabled, but the second clk + interrupt are not.
    // 5. Write the configuration byte back to the controller. Note that you will need 
    //    to poll the status bit to determine when it is safe to write the configuration byte. 

    uint8_t response = 0;
    char* test_str = "null";
    PS2ControllerConfig *cfg;
    // disable ports
    _send_ps2_controller_cmd(DISABLE_PS2_PORT1);
    _send_ps2_controller_cmd(DISABLE_PS2_PORT2);

    // get current config
    _send_ps2_controller_cmd(READ_CONFIG);
    _get_ps2_response(&response);
    //printk("reading config: %hd\n", response);
    cfg = (PS2ControllerConfig*)&response;

    // modfiy config
    cfg->intr1 = 1;
    cfg->intr2 = 0;
    cfg->clk1 = 1;
    cfg->clk2 = 0;
    cfg->port_translation = 0;

    _send_ps2_controller_cmd(WRITE_CONFIG);
    _send_ps2_controller_data(*((uint8_t *)cfg));

    // test controller
    _send_ps2_controller_cmd(TEST_PS2_CONTROLLER);
    _get_ps2_response(&response);

    if (response == PS2_CONT_TEST_PASS) test_str = "PASSED";
    else if (response == PS2_CONT_TEST_FAIL) test_str = "FAILED";
    printk("PS2 CONTROLLER TEST: %s\n", test_str);

    // test port
    _send_ps2_controller_cmd(TEST_PS2_PORT1);
    _get_ps2_response(&response);

    if (response == PS2_PORT_TEST_PASS) test_str = "PASSED";
    else test_str = "FAILED";
    printk("PS2 PORT TEST: %s\n", test_str);

}

void keyboard_test()
{
    uint8_t response = 0;
    char * test_str = "null"; 
    _send_ps2_controller_cmd(ENABLE_PS2_PORT1);

    // reset device
    _send_ps2_dev_cmd(KEY_RESET, 0, NO_DATA);
    // _get_ps2_response(&response);
    // if(response != DATA_ACK) printk("ERROR: Device at Port 1 Failed reset\n");
    _get_ps2_response(&response);
    printk("response: %hx\n", response);
    if(response != DEV_SELF_TEST_PASS) printk("ERROR: Device at Port 1 Failed reset\n");

    // set scan codes
    _send_ps2_dev_cmd(KEY_SCAN_CODES, SCAN_CODE_SET2, SEND_DATA);

    // enable keyboard
    _send_ps2_dev_cmd(KEY_EN_SCANNING, 0, NO_DATA);


}