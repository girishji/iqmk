
#include "wait.h"
#include "usb_device_state.h"
#include "usb_descriptor.h"
#include "usb_driver.h"



#ifdef NKRO_ENABLE
#    include "keycode_config.h"

extern keymap_config_t keymap_config;
#endif

/* LED status */
uint8_t keyboard_leds(void) {
    return keyboard_led_state;
}

/* prepare and start sending a report IN
 * not callable from ISR or locked state */
void send_keyboard(report_keyboard_t *report) {
    osalSysLock();
    if (usbGetDriverStateI(&USB_DRIVER) != USB_ACTIVE) {
        goto unlock;
    }

#ifdef NKRO_ENABLE
    if (keymap_config.nkro && keyboard_protocol) { /* NKRO protocol */
        /* need to wait until the previous packet has made it through */
        /* can rewrite this using the synchronous API, then would wait
         * until *after* the packet has been transmitted. I think
         * this is more efficient */
        /* busy wait, should be short and not very common */
        if (usbGetTransmitStatusI(&USB_DRIVER, SHARED_IN_EPNUM)) {
            /* Need to either suspend, or loop and call unlock/lock during
             * every iteration - otherwise the system will remain locked,
             * no interrupts served, so USB not going through as well.
             * Note: for suspend, need USB_USE_WAIT == TRUE in halconf.h */
            osalThreadSuspendS(&(&USB_DRIVER)->epc[SHARED_IN_EPNUM]->in_state->thread);

            /* after osalThreadSuspendS returns USB status might have changed */
            if (usbGetDriverStateI(&USB_DRIVER) != USB_ACTIVE) {
                goto unlock;
            }
        }
        usbStartTransmitI(&USB_DRIVER, SHARED_IN_EPNUM, (uint8_t *)report, sizeof(struct nkro_report));
    } else
#endif /* NKRO_ENABLE */
    {  /* regular protocol */
        /* need to wait until the previous packet has made it through */
        /* busy wait, should be short and not very common */
        if (usbGetTransmitStatusI(&USB_DRIVER, KEYBOARD_IN_EPNUM)) {
            /* Need to either suspend, or loop and call unlock/lock during
             * every iteration - otherwise the system will remain locked,
             * no interrupts served, so USB not going through as well.
             * Note: for suspend, need USB_USE_WAIT == TRUE in halconf.h */
            osalThreadSuspendS(&(&USB_DRIVER)->epc[KEYBOARD_IN_EPNUM]->in_state->thread);

            /* after osalThreadSuspendS returns USB status might have changed */
            if (usbGetDriverStateI(&USB_DRIVER) != USB_ACTIVE) {
                goto unlock;
            }
        }
        uint8_t *data, size;
        if (keyboard_protocol) {
            data = (uint8_t *)report;
            size = KEYBOARD_REPORT_SIZE;
        } else { /* boot protocol */
            data = &report->mods;
            size = 8;
        }
        usbStartTransmitI(&USB_DRIVER, KEYBOARD_IN_EPNUM, data, size);
    }
    keyboard_report_sent = *report;

unlock:
    osalSysUnlock();
}

/* ---------------------------------------------------------
 *                     Mouse functions
 * ---------------------------------------------------------
 */

#ifdef MOUSE_ENABLE
void send_mouse(report_mouse_t *report) {
    osalSysLock();
    if (usbGetDriverStateI(&USB_DRIVER) != USB_ACTIVE) {
        osalSysUnlock();
        return;
    }

    if (usbGetTransmitStatusI(&USB_DRIVER, MOUSE_IN_EPNUM)) {
        /* Need to either suspend, or loop and call unlock/lock during
         * every iteration - otherwise the system will remain locked,
         * no interrupts served, so USB not going through as well.
         * Note: for suspend, need USB_USE_WAIT == TRUE in halconf.h */
        if (osalThreadSuspendTimeoutS(&(&USB_DRIVER)->epc[MOUSE_IN_EPNUM]->in_state->thread, TIME_MS2I(10)) == MSG_TIMEOUT) {
            osalSysUnlock();
            return;
        }
    }
    usbStartTransmitI(&USB_DRIVER, MOUSE_IN_EPNUM, (uint8_t *)report, sizeof(report_mouse_t));
    osalSysUnlock();
}

#else  /* MOUSE_ENABLE */
void send_mouse(report_mouse_t *report) {
    (void)report;
}
#endif /* MOUSE_ENABLE */

/* ---------------------------------------------------------
 *                   Extrakey functions
 * ---------------------------------------------------------
 */

void send_extra(report_extra_t *report) {
#ifdef EXTRAKEY_ENABLE
    osalSysLock();
    if (usbGetDriverStateI(&USB_DRIVER) != USB_ACTIVE) {
        osalSysUnlock();
        return;
    }

    if (usbGetTransmitStatusI(&USB_DRIVER, SHARED_IN_EPNUM)) {
        /* Need to either suspend, or loop and call unlock/lock during
         * every iteration - otherwise the system will remain locked,
         * no interrupts served, so USB not going through as well.
         * Note: for suspend, need USB_USE_WAIT == TRUE in halconf.h */
        if (osalThreadSuspendTimeoutS(&(&USB_DRIVER)->epc[SHARED_IN_EPNUM]->in_state->thread, TIME_MS2I(10)) == MSG_TIMEOUT) {
            osalSysUnlock();
            return;
        }
    }

    usbStartTransmitI(&USB_DRIVER, SHARED_IN_EPNUM, (uint8_t *)report, sizeof(report_extra_t));
    osalSysUnlock();
#endif
}

void send_programmable_button(report_programmable_button_t *report) {
#ifdef PROGRAMMABLE_BUTTON_ENABLE
    osalSysLock();
    if (usbGetDriverStateI(&USB_DRIVER) != USB_ACTIVE) {
        osalSysUnlock();
        return;
    }

    if (usbGetTransmitStatusI(&USB_DRIVER, SHARED_IN_EPNUM)) {
        /* Need to either suspend, or loop and call unlock/lock during
         * every iteration - otherwise the system will remain locked,
         * no interrupts served, so USB not going through as well.
         * Note: for suspend, need USB_USE_WAIT == TRUE in halconf.h */
        if (osalThreadSuspendTimeoutS(&(&USB_DRIVER)->epc[SHARED_IN_EPNUM]->in_state->thread, TIME_MS2I(10)) == MSG_TIMEOUT) {
            osalSysUnlock();
            return;
        }
    }

    usbStartTransmitI(&USB_DRIVER, SHARED_IN_EPNUM, (uint8_t *)report, sizeof(report_programmable_button_t));
    osalSysUnlock();
#endif
}

void send_joystick(report_joystick_t *report) {
#ifdef JOYSTICK_ENABLE
    osalSysLock();
    if (usbGetDriverStateI(&USB_DRIVER) != USB_ACTIVE) {
        osalSysUnlock();
        return;
    }

    if (usbGetTransmitStatusI(&USB_DRIVER, JOYSTICK_IN_EPNUM)) {
        /* Need to either suspend, or loop and call unlock/lock during
         * every iteration - otherwise the system will remain locked,
         * no interrupts served, so USB not going through as well.
         * Note: for suspend, need USB_USE_WAIT == TRUE in halconf.h */
        if (osalThreadSuspendTimeoutS(&(&USB_DRIVER)->epc[JOYSTICK_IN_EPNUM]->in_state->thread, TIME_MS2I(10)) == MSG_TIMEOUT) {
            osalSysUnlock();
            return;
        }
    }

    usbStartTransmitI(&USB_DRIVER, JOYSTICK_IN_EPNUM, (uint8_t *)report, sizeof(report_joystick_t));
    osalSysUnlock();
#endif
}

void send_digitizer(report_digitizer_t *report) {
#ifdef DIGITIZER_ENABLE
    osalSysLock();
    if (usbGetDriverStateI(&USB_DRIVER) != USB_ACTIVE) {
        osalSysUnlock();
        return;
    }

    if (usbGetTransmitStatusI(&USB_DRIVER, DIGITIZER_IN_EPNUM)) {
        /* Need to either suspend, or loop and call unlock/lock during
         * every iteration - otherwise the system will remain locked,
         * no interrupts served, so USB not going through as well.
         * Note: for suspend, need USB_USE_WAIT == TRUE in halconf.h */
        if (osalThreadSuspendTimeoutS(&(&USB_DRIVER)->epc[DIGITIZER_IN_EPNUM]->in_state->thread, TIME_MS2I(10)) == MSG_TIMEOUT) {
            osalSysUnlock();
            return;
        }
    }

    usbStartTransmitI(&USB_DRIVER, DIGITIZER_IN_EPNUM, (uint8_t *)report, sizeof(report_digitizer_t));
    osalSysUnlock();
#endif
}

/* ---------------------------------------------------------
 *                   Console functions
 * ---------------------------------------------------------
 */

#ifdef CONSOLE_ENABLE

int8_t sendchar(uint8_t c) {
    static bool timed_out = false;
    /* The `timed_out` state is an approximation of the ideal `is_listener_disconnected?` state.
     *
     * When a 5ms timeout write has timed out, hid_listen is most likely not running, or not
     * listening to this keyboard, so we go into the timed_out state. In this state we assume
     * that hid_listen is most likely not gonna be connected to us any time soon, so it would
     * be wasteful to write follow-up characters with a 5ms timeout, it would all add up and
     * unncecessarily slow down the firmware. However instead of just dropping the characters,
     * we write them with a TIME_IMMEDIATE timeout, which is a zero timeout,
     * and this will succeed only if hid_listen gets connected again. When a write with
     * TIME_IMMEDIATE timeout succeeds, we know that hid_listen is listening to us again, and
     * we can go back to the timed_out = false state, and following writes will be executed
     * with a 5ms timeout. The reason we don't just send all characters with the TIME_IMMEDIATE
     * timeout is that this could cause bytes to be lost even if hid_listen is running, if there
     * is a lot of data being sent over the console.
     *
     * This logic will work correctly as long as hid_listen is able to receive at least 200
     * bytes per second. On a heavily overloaded machine that's so overloaded that it's
     * unusable, and constantly swapping, hid_listen might have trouble receiving 200 bytes per
     * second, so some bytes might be lost on the console.
     */

    const sysinterval_t timeout = timed_out ? TIME_IMMEDIATE : TIME_MS2I(5);
    const size_t        result  = chnWriteTimeout(&drivers.console_driver.driver, &c, 1, timeout);
    timed_out                   = (result == 0);
    return result;
}

// Just a dummy function for now, this could be exposed as a weak function
// Or connected to the actual QMK console
static void console_receive(uint8_t *data, uint8_t length) {
    (void)data;
    (void)length;
}

void console_task(void) {
    uint8_t buffer[CONSOLE_EPSIZE];
    size_t  size = 0;
    do {
        size = chnReadTimeout(&drivers.console_driver.driver, buffer, sizeof(buffer), TIME_IMMEDIATE);
        if (size > 0) {
            console_receive(buffer, size);
        }
    } while (size > 0);
}

#endif /* CONSOLE_ENABLE */

#ifdef RAW_ENABLE
void raw_hid_send(uint8_t *data, uint8_t length) {
    // TODO: implement variable size packet
    if (length != RAW_EPSIZE) {
        return;
    }
    chnWrite(&drivers.raw_driver.driver, data, length);
}

__attribute__((weak)) void raw_hid_receive(uint8_t *data, uint8_t length) {
    // Users should #include "raw_hid.h" in their own code
    // and implement this function there. Leave this as weak linkage
    // so users can opt to not handle data coming in.
}

void raw_hid_task(void) {
    uint8_t buffer[RAW_EPSIZE];
    size_t  size = 0;
    do {
        size = chnReadTimeout(&drivers.raw_driver.driver, buffer, sizeof(buffer), TIME_IMMEDIATE);
        if (size > 0) {
            raw_hid_receive(buffer, size);
        }
    } while (size > 0);
}

#endif

