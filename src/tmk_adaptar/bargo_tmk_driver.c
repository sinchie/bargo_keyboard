#include "host.h"
#include "host_driver.h"
#include "bargo_tmk_driver.h"
#include "bargo_keyboard.h"

/* Host driver */
static uint8_t keyboard_leds(void);
static void send_keyboard(report_keyboard_t *report);
static void send_mouse(report_mouse_t *report);
static void send_system(uint16_t data);
static void send_consumer(uint16_t data);

host_driver_t bargo_nrf_driver = {
    keyboard_leds,
    send_keyboard,
    send_mouse,
    send_system,
    send_consumer
};


static uint8_t keyboard_leds(void)
{
    return 0;
}

static void send_keyboard(report_keyboard_t *report)
{
    // 发送键盘按键
    bargo_keyboard_send_report(report);
}

static void send_mouse(report_mouse_t *report)
{
}

static void send_system(uint16_t data)
{
    bargo_keyboard_system_send_report(data);
}

static void send_consumer(uint16_t data)
{
    bargo_keyboard_media_send_report(data);
}