#ifndef BARGO_KEYBOARD_H
#define BARGO_KEYBOARD_H

#include "report.h"
#include <stdbool.h>
#include "app_timer.h"

// 键盘休眠定时器
APP_TIMER_DEF(m_keyboard_sleep_timer_id);

// 键盘矩阵扫描间隔（毫秒）
#define KEYBOARD_MATRIX_SCAN_INTERVAL       APP_TIMER_TICKS(3)

// 发送键盘按键
bool bargo_keyboard_send_report(report_keyboard_t *report);

// 键盘矩阵扫描初始化
void bargo_keyboard_martix_sacn_init();

// 阵列准备睡眠
void matrix_wakeup_prepare(void);

#endif

