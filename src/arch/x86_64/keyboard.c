#include "keyboard.h"
#include "ps2.h"
#include "printk.h"
#include <stdint.h>

char ascii_lookup_tbl[128][5] = {
    "\0", "F9", "\0", "F5", "F3", "F1", "F2", "F12", "\0", "F10",  // 0x09
    "F8", "F6", "F4", "\t", "`", "\0", "\0", "LALT", "LSHFT", "\0", // 0X13
    "LCTRL", "q", "1", "\0", "\0", "\0", "z", "s", "a", "w", "2",  // 0x1E
    "\0", "\0", "c", "x", "d", "e", "4", "3", "\0", "\0", " ", "v", // 0x2A
    "f", "t", "r", "5", "\0", "\0", "n", "b", "h", "g", "y", "6", "\0", //0x37
    "\0", "\0", "m", "j", "u", "7", "8", "\0", "\0", ",", "k", "i", //0x43
    "o", "0", "9", "\0", "\0", ".", "/", "l", ";", "p", "-", "\0", //0x4F
    "\0", "\0", "'", "\0", "[", "=", "\0", "\0", "CAPS", "RSHFT", "\n", //0x5A
    "]", "\0", "\\", "\0", "\0", "\0", "\0", "\0","\0", "\0","\0", "BKSPC", //0x66
    "\0", "\0" };

char * poll_keystroke()
{
    uint8_t scancode = 0;
    while(!scancode)
    {
        get_ps2_response(&scancode);
    }
    return ascii_lookup_tbl[scancode];
}

char * get_key()
{
    uint8_t scancode = 0;
    get_ps2_response(&scancode);
    if(scancode == 0xF0) // will be used for detecting key release
    {
        get_ps2_response(&scancode);
        return "\0";
        //printk("got 0xF0\n");
    }    
    return ascii_lookup_tbl[scancode];
}
void keyboard_init()
{
    uint8_t response = 0;
    char * test_str = "null"; 
    ps2_init();
    send_ps2_controller_cmd(ENABLE_PS2_PORT1);

    // reset device
    send_ps2_dev_cmd(KEY_RESET, 0, NO_DATA);
    get_ps2_response(&response); // Reset cmd sends two responses??
    //printk("response: %hx\n", response);
    if(response != DEV_SELF_TEST_PASS) printk("ERROR: Device at Port 1 Failed reset\n");

    // set scan codes
    send_ps2_dev_cmd(KEY_SCAN_CODES, SCAN_CODE_SET2, SEND_DATA);

    // enable keyboard
    send_ps2_dev_cmd(KEY_EN_SCANNING, 0, NO_DATA);

    // get extra bytes sent prolly an error code or something lol, usually 0x06
    get_ps2_response(&response);


}
