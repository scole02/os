#include "my_string.h"
#include "ps2.h"
#include "printk.h"
#include "libutils.h"
#include <stdint.h>


void send_ps2_controller_cmd(uint8_t cmd)
{
    uint8_t status = _inb(CMD_PORT);

    while( ((PS2StatusReg*)&status)->in_buf ) // wait until in_buf is empty
        status = _inb(CMD_PORT);
    
    _outb(CMD_PORT, cmd);
}

// used to get data from PS2 devs
void get_ps2_response(uint8_t * resp)
{
    uint8_t status = _inb(CMD_PORT);

    while( !((PS2StatusReg*)&status)->out_buf ) // wait until out_buf is full
        status = _inb(CMD_PORT);
    
    *resp = _inb(DATA_PORT); // get response byte
}

void send_ps2_controller_data(uint8_t data)
{
    uint8_t status = _inb(CMD_PORT);

    while( ((PS2StatusReg*)&status)->in_buf ) // wait until in_buf is empty
        status = _inb(CMD_PORT);
    
    _outb(DATA_PORT, data);
}

void send_ps2_dev_cmd(uint8_t cmd, uint8_t data, uint8_t send_data) // if !send_data only send one byte
{
    uint8_t response = 0, status = _inb(CMD_PORT);

    while( ((PS2StatusReg*)&status)->in_buf ) // wait until in_buf is empty
        status = _inb(CMD_PORT);
    
    _outb(DATA_PORT, cmd);

    // check response here
    get_ps2_response(&response);
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
    send_ps2_controller_cmd(DISABLE_PS2_PORT1);
    send_ps2_controller_cmd(DISABLE_PS2_PORT2);

    // get current config
    send_ps2_controller_cmd(READ_CONFIG);
    get_ps2_response(&response);
    //printk("reading config: %hd\n", response);
    cfg = (PS2ControllerConfig*)&response;

    // modfiy config
    cfg->intr1 = 1;
    cfg->intr2 = 0;
    cfg->clk1 = 1;
    cfg->clk2 = 0;
    cfg->port_translation = 0;

    send_ps2_controller_cmd(WRITE_CONFIG);
    send_ps2_controller_data(*((uint8_t *)cfg));

    // test controller
    send_ps2_controller_cmd(TEST_PS2_CONTROLLER);
    get_ps2_response(&response);

    if (response == PS2_CONT_TEST_PASS) test_str = "PASSED";
    else if (response == PS2_CONT_TEST_FAIL) test_str = "FAILED";
    printk("PS2 CONTROLLER TEST: %s\n", test_str);

    // test port
    send_ps2_controller_cmd(TEST_PS2_PORT1);
    get_ps2_response(&response);

    if (response == PS2_PORT_TEST_PASS) test_str = "PASSED";
    else test_str = "FAILED";
    printk("PS2 PORT TEST: %s\n", test_str);

}



