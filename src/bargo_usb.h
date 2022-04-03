#ifndef BARGO_USB_H
#define BARGO_USB_H

#include <stdbool.h>
#include "report.h"

// 初始化usb
void bargo_usb_init(void);

// usb发送按键
void bargo_usb_keys_send(report_keyboard_t *report);

// usb发送系统按键
int bargo_usb_send_system(uint16_t data);

// usb发送媒体按键
int bargo_usb_send_consumer(uint16_t data);

// usb事件处理主循环
void bargo_usb_process(void);

// usb是否通电
bool usb_power_enable(void);

#endif

