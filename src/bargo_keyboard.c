#include "bargo_keyboard.h"
#include "bargo_ble.h"
#include "bargo_usb.h"
#include "bargo_switch.h"
#include "bargo_config.h"
#include "bargo_tmk_driver.h"
#include "nrf_gpio.h"
#include "nrf_drv_wdt.h"
#include "keyboard.h"
#include "host.h"


// 矩阵扫描定时器
APP_TIMER_DEF(m_matrix_scan_timer_id);


// 游戏模式flag
static bool ble_game_mode = false;

// 睡眠计数器
static uint32_t sleep_counter;

// 慢速计数器
static uint32_t slow_counter;
// 是否在慢速扫描模式中
static bool in_slow_scan_mode = false;

// 看门狗
#ifdef BARGO_WATCH_DOG_ENABLE
// 看门狗是否初始化标记
bool bargo_watch_dog_inited = false;
// 看门狗渠道ID
nrf_drv_wdt_channel_id m_wdt_channel_id;
// 看门超时狗回调
void bargo_wdt_event_handler(void)
{
    // do soming
}
// 看门狗初始化
void bargo_watch_dog_init()
{
    uint32_t err_code = NRF_SUCCESS;
    nrf_drv_wdt_config_t config = NRF_DRV_WDT_DEAFULT_CONFIG;
    err_code = nrf_drv_wdt_init(&config, bargo_wdt_event_handler);
    APP_ERROR_CHECK(err_code);
    err_code = nrf_drv_wdt_channel_alloc(&m_wdt_channel_id);
    APP_ERROR_CHECK(err_code);
    nrf_drv_wdt_enable();

    bargo_watch_dog_inited = true;
}

#endif

// 重置计数器
static bool bargo_reset_counter()
{
    // 重置慢速扫描计数
    bargo_keyboard_reset_slow_counter();
    // 重置休眠计数
    bargo_keyboard_reset_sleep_counter();
}

// 是否应该发送usb
static bool bargo_should_send_usb()
{
    if(bargo_switch_is_using_usb() && usb_power_enable()) {
        return true;
    }

    return false;
}

// 发送键盘按键
bool bargo_keyboard_send_report(report_keyboard_t *report)
{
    // 重置计数器
    bargo_reset_counter();
    // 发送按键
    if(bargo_should_send_usb()) {
        // usb发送按键
        bargo_usb_keys_send(report);
    } else {
        // 蓝牙发送按键
        bargo_ble_keys_send(report);
    }
   
    return true;
}

// 发送 system 键盘按键
bool bargo_keyboard_system_send_report(uint16_t data)
{
    // 重置计数器
    bargo_reset_counter();

    // 发送按键
    if(bargo_should_send_usb()) {
        // usb发送按键
        bargo_usb_send_system(data);
    } else {
        // 蓝牙发送按键
        bargo_ble_send_system(data);
    }
   
    return true;
}

// 发送 media 键盘按键
bool bargo_keyboard_media_send_report(uint16_t data)
{
    // 重置计数器
    bargo_reset_counter();

    // 发送按键
    if(bargo_should_send_usb()) {
        // usb发送按键
        bargo_usb_send_consumer(data);
    } else {
        // 蓝牙发送按键
        bargo_ble_send_consumer(data);
    }
   
    return true;
}

// 键盘矩阵扫描事件回调
static void bargo_keyboard_task_timeout_handler(void * p_context)
{
    UNUSED_PARAMETER(p_context);
    keyboard_task();
}

// 键盘tick触发回调
static void bargo_keyboard_tick_handler(void * p_context)
{
    UNUSED_PARAMETER(p_context);

#ifdef BARGO_WATCH_DOG_ENABLE
    if (bargo_watch_dog_inited) {
        // 喂狗
        nrf_drv_wdt_channel_feed(m_wdt_channel_id);
    }
#endif

    ret_code_t err_code;
    // 慢速扫描计数
    slow_counter++;
    if (slow_counter >= KEYBOARD_MATRIX_IDLE_SLOW_SECOND) {
        slow_counter = KEYBOARD_MATRIX_IDLE_SLOW_SECOND;
        // 进入慢速扫描
        if (!in_slow_scan_mode && !usb_power_enable() && !ble_game_mode) {
            in_slow_scan_mode = true;
            err_code = app_timer_stop(m_matrix_scan_timer_id);
            APP_ERROR_CHECK(err_code);
            err_code = app_timer_start(m_matrix_scan_timer_id, KEYBOARD_MATRIX_SCAN_INTERVAL_SLOW, NULL);
            APP_ERROR_CHECK(err_code);
        }
    } else {
        // 退出慢速扫描模式，进入快速扫描模式
        if (in_slow_scan_mode) {
            in_slow_scan_mode = false;
            err_code = app_timer_stop(m_matrix_scan_timer_id);
            APP_ERROR_CHECK(err_code);
            err_code = app_timer_start(m_matrix_scan_timer_id, KEYBOARD_MATRIX_SCAN_INTERVAL_FAST, NULL);
            APP_ERROR_CHECK(err_code);
        }
    }

    // 闲置时间计数溢出，蓝牙模式触发休眠
    sleep_counter++;
    if (sleep_counter >= BARGO_KEYBOARD_IDLE_SLEEP_SECOND) {
        bargo_keyboard_reset_sleep_counter();
        if (!usb_power_enable()) {
            bargo_sleep_mode_enter();
        }
    }
}

// 游戏模式开关
void bargo_ble_game_mode_toggle()
{
    ble_game_mode = !ble_game_mode;
}

// 重置慢速扫描计数
void bargo_keyboard_reset_slow_counter()
{
    slow_counter = 0;
}

// 重置休眠计数
void bargo_keyboard_reset_sleep_counter()
{
    sleep_counter = 0;
}

// 键盘矩阵扫描
void bargo_keyboard_martix_sacn_init()
{
    ret_code_t err_code;
    // 初始化tmk
    keyboard_init();
    // 设置tmk主机驱动
    host_set_driver(&bargo_nrf_driver);

    // 键盘矩阵扫描定时器
    err_code = app_timer_create(&m_matrix_scan_timer_id,
                                APP_TIMER_MODE_REPEATED,
                                bargo_keyboard_task_timeout_handler);
    APP_ERROR_CHECK(err_code);

    err_code = app_timer_start(m_matrix_scan_timer_id, KEYBOARD_MATRIX_SCAN_INTERVAL_SLOW, NULL);
    APP_ERROR_CHECK(err_code);
    in_slow_scan_mode = true;

    // 键盘tick定时器
    err_code = app_timer_create(&m_keyboard_tick_timer_id,
                                APP_TIMER_MODE_REPEATED,
                                bargo_keyboard_tick_handler);
    APP_ERROR_CHECK(err_code);
    err_code = app_timer_start(m_keyboard_tick_timer_id, APP_TIMER_TICKS(1000), NULL);
    APP_ERROR_CHECK(err_code);

#ifdef BARGO_WATCH_DOG_ENABLE
    // 初始化看门狗
    bargo_watch_dog_init();
#endif
}

/**
 * @brief 阵列准备睡眠
 * 
 */
void matrix_wakeup_prepare(void)
{
    for (uint8_t i = 0;  i < MATRIX_COLS; i++) {
        nrf_gpio_cfg_default(col_pins[i]);
    }
    for (uint8_t i = 0;  i < MATRIX_ROWS; i++) {
        nrf_gpio_cfg_default(row_pins[i]);
    }

    // 监听所有按键作为唤醒按键
    for (uint8_t i = 0; i < MATRIX_COLS; i++) {
        nrf_gpio_cfg_sense_input(col_pins[i], NRF_GPIO_PIN_PULLUP, NRF_GPIO_PIN_SENSE_LOW);
    }
    for (uint8_t i = 0; i < MATRIX_ROWS; i++) {
        nrf_gpio_cfg_output(row_pins[i]);
        nrf_gpio_pin_clear(row_pins[i]);
    }
}