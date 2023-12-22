
/* TMK includes */
#include "report.h"
#include "host.h"
#include "host_driver.h"
#include "keyboard.h"
#include "action.h"
#include "action_util.h"
#include "usb_device_state.h"
#include "mousekey.h"
#include "led.h"
#include "sendchar.h"
#include "debug.h"
#include "print.h"

/* declarations */
uint8_t keyboard_leds(void);
void    send_keyboard(report_keyboard_t *report);
void    send_mouse(report_mouse_t *report);
void    send_extra(report_extra_t *report);

/* host struct */
host_driver_t pico_driver = {keyboard_leds, send_keyboard, send_mouse, send_extra};

static host_driver_t *driver = NULL;

void protocol_pre_init(void) {
    driver = &pico_driver;

    // /* Do need to wait here!
    //  * Otherwise the next print might start a transfer on console EP
    //  * before the USB is completely ready, which sometimes causes
    //  * HardFaults.
    //  */
    // wait_ms(50);

    // print("USB configured.\n");
}

void protocol_post_init(void) {
    host_set_driver(driver);
}

