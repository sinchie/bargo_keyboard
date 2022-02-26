#ifndef BARGO_CONFIG_H
#define BARGO_CONFIG_H

/* USB 设备描述信息 */
#define VENDOR_ID       0xFEED
#define PRODUCT_ID      0x6464
#define DEVICE_VER      0x0001
#define MANUFACTURER    Bargo
#define PRODUCT         Bargo
#define DESCRIPTION     Bargo keyboard firmware nrf52 & tmk

/* 键盘矩阵大小 */
#define MATRIX_ROWS 2
#define MATRIX_COLS 3

/* 设置按键去抖时间（毫秒） */
#define DEBOUNCE  5

/* 按键锁定支持. Use KC_LCAP, KC_LNUM or KC_LSCR instead in keymap */
#define LOCKING_SUPPORT_ENABLE
/* Locking resynchronize hack */
#define LOCKING_RESYNC_ENABLE

/* command组合键 */
#define IS_COMMAND() ( \
    keyboard_report->mods == (MOD_BIT(KC_LSHIFT) | MOD_BIT(KC_RSHIFT)) \
)

/* 背光等级 */
#define BACKLIGHT_LEVELS 0

/* 电池电量检测引脚 */
#define BATTERY_PIN NRF_SAADC_INPUT_AIN0

/* 键盘矩阵引脚 */
static const uint8_t row_pins[MATRIX_ROWS] = {17, 20 };
static const uint8_t col_pins[MATRIX_COLS] = {13, 22, 24 };

/* 键盘休眠触发间隔时间 */
#define BARGO_KEYBOARD_SLEEP_INTERVAL_MS 30000


/* 关闭TMK调试 */
#define NO_DEBUG

/* 关闭TMK打印 */
#define NO_PRINT

#endif