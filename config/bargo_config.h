#ifndef BARGO_CONFIG_H
#define BARGO_CONFIG_H

#include "nrf_gpio.h"

// USB 设备描述信息
#define VENDOR_ID       0xFEED
#define PRODUCT_ID      0x6868
#define DEVICE_VER      0x0001
#define MANUFACTURER    "Bargo"
#define PRODUCT         "Bargo Keyboard"
#define DESCRIPTION     "Bargo keyboard firmware nrf52 & tmk"

// 键盘矩阵大小
#define MATRIX_ROWS 6
#define MATRIX_COLS 15

// 按键锁定支持. Use KC_LCAP, KC_LNUM or KC_LSCR instead in keymap
#define LOCKING_SUPPORT_ENABLE
// Locking resynchronize hack
#define LOCKING_RESYNC_ENABLE

// command组合键
#define IS_COMMAND() ( \
    keyboard_report->mods == (MOD_BIT(KC_LSHIFT) | MOD_BIT(KC_RSHIFT)) \
)

// 电池电量检测引脚
#define BATTERY_PIN NRF_SAADC_INPUT_AIN0

// 电量检测间隔时间（毫秒）
#define BATTERY_LEVEL_MEAS_INTERVAL         APP_TIMER_TICKS(60000)

// 键盘矩阵引脚
static const uint8_t row_pins[MATRIX_ROWS] = {
    NRF_GPIO_PIN_MAP(1,11),NRF_GPIO_PIN_MAP(1,10),NRF_GPIO_PIN_MAP(0,3),NRF_GPIO_PIN_MAP(0,28),NRF_GPIO_PIN_MAP(1,13),NRF_GPIO_PIN_MAP(0,29)
};
static const uint8_t col_pins[MATRIX_COLS] = {
    NRF_GPIO_PIN_MAP(0,31),NRF_GPIO_PIN_MAP(0,30),NRF_GPIO_PIN_MAP(0,26),NRF_GPIO_PIN_MAP(0,6),NRF_GPIO_PIN_MAP(0,5),
    NRF_GPIO_PIN_MAP(0,8),NRF_GPIO_PIN_MAP(1,9),NRF_GPIO_PIN_MAP(0,4),NRF_GPIO_PIN_MAP(0,12),NRF_GPIO_PIN_MAP(0,7),
    NRF_GPIO_PIN_MAP(0,15),NRF_GPIO_PIN_MAP(0,17),NRF_GPIO_PIN_MAP(0,20),NRF_GPIO_PIN_MAP(0,13),NRF_GPIO_PIN_MAP(0,22)
};

// 闲置多少秒进入休眠
#define BARGO_KEYBOARD_IDLE_SLEEP_SECOND 600

// 键盘矩阵扫描间隔（毫秒）快速
#define KEYBOARD_MATRIX_SCAN_INTERVAL_FAST  APP_TIMER_TICKS(4)

// 键盘矩阵扫描间隔（毫秒）慢速
#define KEYBOARD_MATRIX_SCAN_INTERVAL_SLOW  APP_TIMER_TICKS(100)

// 闲置多少秒进入慢速扫描模式
#define KEYBOARD_MATRIX_IDLE_SLOW_SECOND 10

// 开启看门狗
#define BARGO_WATCH_DOG_ENABLE

// 关闭TMK调试
#define NO_DEBUG

// 关闭TMK打印
#define NO_PRINT

#endif