#ifndef BARGO_USB_H
#define BARGO_USB_H

#include <stdbool.h>
#include "report.h"

// 初始化usb
void bargo_usb_init(void);

// usb发送按键
void bargo_usb_keys_send(report_keyboard_t *report);

// usb事件处理主循环
void bargo_usb_process(void);

// usb是否通电
bool usb_power_enable(void);

#endif

