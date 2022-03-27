#ifndef BARGO_KEYBOARD_H
#define BARGO_KEYBOARD_H

#include "report.h"
#include <stdbool.h>
#include "app_timer.h"
#include "bargo_config.h"

// 键盘Tick
APP_TIMER_DEF(m_keyboard_tick_timer_id);

// 发送键盘按键
bool bargo_keyboard_send_report(report_keyboard_t *report);

// 键盘矩阵扫描初始化
void bargo_keyboard_martix_sacn_init();

// 阵列准备睡眠
void matrix_wakeup_prepare(void);

// 重置休眠计数
void bargo_keyboard_reset_sleep_counter();

// 重置慢速扫描计数
void bargo_keyboard_reset_slow_counter();

// 初始化看门狗
void bargo_keyboard_wdt_init(void);

// 游戏模式开关
void bargo_ble_game_mode_toggle();

#endif

