#include "bargo_keyboard.h"
#include "bargo_ble.h"
#include "bargo_usb.h"
#include "bargo_switch.h"
#include "bargo_tmk_driver.h"
#include "nrf_gpio.h"

// 发送键盘按键
bool bargo_keyboard_send_report(report_keyboard_t *report) {
    uint64_t result =0;
    for (uint8_t i = 0; i < 8; i++) {
       result |= report->raw[i] << (8 * i);
    }

    if(bargo_switch_is_using_usb()) {
        // usb发送按键
        bargo_usb_keys_send(8, report->raw);
    } else {
        // 蓝牙发送按键
        bargo_ble_keys_send(8, report->raw);
    }

    // 刷新睡眠定时器
    ret_code_t err_code;
    err_code = app_timer_stop(m_keyboard_sleep_timer_id);
    APP_ERROR_CHECK(err_code);
    err_code = app_timer_start(m_keyboard_sleep_timer_id, APP_TIMER_TICKS(BARGO_KEYBOARD_SLEEP_INTERVAL_MS), NULL);
    APP_ERROR_CHECK(err_code);
   
    return true;
}

// 键盘矩阵扫描事件回调
static void bargo_keyboard_task_timeout_handler(void * p_context)
{
    UNUSED_PARAMETER(p_context);
    keyboard_task();
}

// 键盘睡眠触发回调
static void bargo_keyboard_sleep_timeout_handler(void * p_context)
{
    UNUSED_PARAMETER(p_context);

    bargo_sleep_mode_enter();
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
    APP_TIMER_DEF(m_matrix_timer_id);
    err_code = app_timer_create(&m_matrix_timer_id,
                                APP_TIMER_MODE_REPEATED,
                                bargo_keyboard_task_timeout_handler);
    APP_ERROR_CHECK(err_code);

    err_code = app_timer_start(m_matrix_timer_id, KEYBOARD_MATRIX_SCAN_INTERVAL, NULL);
    APP_ERROR_CHECK(err_code);

    // 键盘睡眠定时器
    err_code = app_timer_create(&m_keyboard_sleep_timer_id,
                                APP_TIMER_MODE_SINGLE_SHOT,
                                bargo_keyboard_sleep_timeout_handler);
    APP_ERROR_CHECK(err_code);
    err_code = app_timer_start(m_keyboard_sleep_timer_id, APP_TIMER_TICKS(BARGO_KEYBOARD_SLEEP_INTERVAL_MS), NULL);
    APP_ERROR_CHECK(err_code);
}

/**
 * @brief 阵列准备睡眠
 * 
 */
void matrix_wakeup_prepare(void)
{
    // 这里监听所有按键作为唤醒按键，所以真正的唤醒判断应该在main的初始化过程中
    for (uint8_t i = 0; i < MATRIX_COLS; i++) {
        nrf_gpio_cfg_sense_input(col_pins[i], NRF_GPIO_PIN_PULLDOWN, NRF_GPIO_PIN_SENSE_HIGH);
    }
    for (uint8_t i = 0; i < MATRIX_ROWS; i++) {
        nrf_gpio_cfg_output(row_pins[i]);
        nrf_gpio_pin_set(row_pins[i]);
    }
}