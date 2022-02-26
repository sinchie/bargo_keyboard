#ifndef BARGO_BLE_H
#define BARGO_BLE_H

#include <stdint.h>
#include <string.h>
#include "nordic_common.h"
#include "nrf.h"
#include "nrf_assert.h"
#include "app_error.h"
#include "ble.h"
#include "ble_err.h"
#include "ble_hci.h"
#include "ble_srv_common.h"
#include "ble_advertising.h"
#include "ble_advdata.h"
#include "ble_hids.h"
#include "ble_bas.h"
#include "app_scheduler.h"
#include "nrf_sdh.h"
#include "nrf_sdh_soc.h"
#include "nrf_sdh_ble.h"
#include "app_timer.h"
#include "peer_manager.h"
#include "ble_conn_state.h"
#include "nrf_ble_gatt.h"
#include "nrf_ble_qwr.h"
#include "peer_manager_handler.h"
#include "peer_manager_types.h"

// 电量检测依赖
#include "nrf_saadc.h"
#include "nrf_drv_saadc.h"

// 日志依赖
#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"

// 设备切换
#include "bargo_switch.h"

// 全局配置
#include "bargo_config.h"
#include "bargo_keyboard.h"


// 蓝牙设备名称
#define DEVICE_NAME                         "Bargo_Keyboard"                          /**< Name of device. Will be included in the advertising data. */

// 应用观察者优先级
#define APP_BLE_OBSERVER_PRIO               3                                          /**< Application's BLE observer priority. You shouldn't need to modify this value. */
#define APP_BLE_CONN_CFG_TAG                1                                          /**< A tag identifying the SoftDevice BLE configuration. */

// 蓝牙广播间隔事件
#define APP_ADV_FAST_INTERVAL               0x0028                                     /**< Fast advertising interval (in units of 0.625 ms. This value corresponds to 25 ms.). */
#define APP_ADV_SLOW_INTERVAL               0x0C80                                     /**< Slow advertising interval (in units of 0.625 ms. This value corrsponds to 2 seconds). */
// 蓝牙广播持续时间
#define APP_ADV_FAST_DURATION               3000                                       /**< The advertising duration of fast advertising in units of 10 milliseconds. */
#define APP_ADV_SLOW_DURATION               18000                                      /**< The advertising duration of slow advertising in units of 10 milliseconds. */

// 蓝牙通讯间隔时间
/*lint -emacro(524, MIN_CONN_INTERVAL) // Loss of precision */
#define MIN_CONN_INTERVAL                   MSEC_TO_UNITS(7.5, UNIT_1_25_MS)           /**< Minimum connection interval (7.5 ms) */
#define MAX_CONN_INTERVAL                   MSEC_TO_UNITS(30, UNIT_1_25_MS)            /**< Maximum connection interval (30 ms). */
#define SLAVE_LATENCY                       6                                          /**< Slave latency. */
#define CONN_SUP_TIMEOUT                    MSEC_TO_UNITS(430, UNIT_10_MS)             /**< Connection supervisory timeout (430 ms). */

// 电量检测间隔时间（毫秒）
#define BATTERY_LEVEL_MEAS_INTERVAL         APP_TIMER_TICKS(30000)

// 蓝牙绑定相关参数
#define SEC_PARAM_BOND                      1                                          /**< Perform bonding. */
#define SEC_PARAM_MITM                      0                                          /**< Man In The Middle protection not required. */
#define SEC_PARAM_LESC                      0                                          /**< LE Secure Connections not enabled. */
#define SEC_PARAM_KEYPRESS                  0                                          /**< Keypress notifications not enabled. */
#define SEC_PARAM_IO_CAPABILITIES           BLE_GAP_IO_CAPS_NONE                       /**< No I/O capabilities. */
#define SEC_PARAM_OOB                       0                                          /**< Out Of Band data not available. */
#define SEC_PARAM_MIN_KEY_SIZE              7                                          /**< Minimum encryption key size. */
#define SEC_PARAM_MAX_KEY_SIZE              16                                         /**< Maximum encryption key size. */

// 蓝牙HID相关参数
#define OUTPUT_REPORT_INDEX                 0                                          /**< Index of Output Report. */
#define OUTPUT_REPORT_MAX_LEN               1                                          /**< Maximum length of Output Report. */
#define INPUT_REPORT_KEYS_INDEX             0                                          /**< Index of Input Report. */
#define OUTPUT_REPORT_BIT_MASK_CAPS_LOCK    0x02                                       /**< CAPS LOCK bit in Output Report (based on 'LED Page (0x08)' of the Universal Serial Bus HID Usage Tables). */
#define INPUT_REP_REF_ID                    0                                          /**< Id of reference to Keyboard Input Report. */
#define OUTPUT_REP_REF_ID                   0                                          /**< Id of reference to Keyboard Output Report. */
#define FEATURE_REP_REF_ID                  0                                          /**< ID of reference to Keyboard Feature Report. */
#define FEATURE_REPORT_MAX_LEN              2                                          /**< Maximum length of Feature Report. */
#define FEATURE_REPORT_INDEX                0                                          /**< Index of Feature Report. */
#define BASE_USB_HID_SPEC_VERSION           0x0101                                     /**< Version number of base USB HID Specification implemented by this application. */
// 蓝牙键盘上报相关参数
#define INPUT_REPORT_KEYS_MAX_LEN           8                                          /**< Maximum length of the Input Report characteristic. */
#define SCAN_CODE_POS                       2                                          /**< The start position of the key scan code in a HID Report. */
#define MAX_KEYS_IN_ONE_REPORT              (INPUT_REPORT_KEYS_MAX_LEN - SCAN_CODE_POS)/**< Maximum number of key presses that can be sent in one Input Report. */

// 断言调用参数
#define DEAD_BEEF                           0xDEADBEEF                                 /**< Value used as error code on stack dump, can be used to identify stack location on stack unwind. */

// 定时器任务调度参数
#define SCHED_MAX_EVENT_DATA_SIZE           APP_TIMER_SCHED_EVENT_DATA_SIZE            /**< Maximum size of scheduler events. */
#define SCHED_QUEUE_SIZE                    20                                         /**< Maximum number of events in the scheduler queue. */

// HID服务
BLE_HIDS_DEF(m_hids,                                                /**< Structure used to identify the HID service. */
             NRF_SDH_BLE_TOTAL_LINK_COUNT,
             INPUT_REPORT_KEYS_MAX_LEN,
             OUTPUT_REPORT_MAX_LEN,
             FEATURE_REPORT_MAX_LEN);
// 电量服务
BLE_BAS_DEF(m_bas);                                                 /**< Structure used to identify the battery service. */
// GATT服务
NRF_BLE_GATT_DEF(m_gatt);                                           /**< GATT module instance. */
// 蓝牙发送队列服务
NRF_BLE_QWR_DEF(m_qwr);                                             /**< Context for the Queued Write module.*/
// 蓝牙广播服务
BLE_ADVERTISING_DEF(m_advertising);                                 /**< Advertising module instance. */

// 蓝牙boot模式标识变量
static bool              m_in_boot_mode = false;                    /**< Current protocol mode. */
// 蓝牙连接句柄变量
static uint16_t          m_conn_handle  = BLE_CONN_HANDLE_INVALID;  /**< Handle of the current connection. */
// 蓝牙绑定句柄变量
static pm_peer_id_t      m_peer_id;                                 /**< Device reference handle to the current bonded central. */

// 蓝牙广播的UUID类型
static ble_uuid_t m_adv_uuids[] = {{BLE_UUID_HUMAN_INTERFACE_DEVICE_SERVICE, BLE_UUID_TYPE_BLE}};

// hid事件回调
static void on_hids_evt(ble_hids_t * p_hids, ble_hids_evt_t * p_evt);

/**@brief Function for initializing the BLE stack.
 *
 * @details Initializes the SoftDevice and the BLE event interrupt.
 */
void bargo_ble_stack_init(void);


/**@brief Function for the Event Scheduler initialization.
 */
void bargo_scheduler_init(void);


/**@brief Function for the GAP initialization.
 *
 * @details This function sets up all the necessary GAP (Generic Access Profile) parameters of the
 *          device including the device name, appearance, and the preferred connection parameters.
 */
void bargo_gap_params_init(void);

/**@brief Function for initializing the GATT module.
 */
void bargo_gatt_init(void);

/**@brief Function for initializing the Advertising functionality.
 */
void bargo_advertising_init(void);

/**@brief Function for initializing services that will be used by the application.
 */
void bargo_ble_services_init(void);

/**@brief Function for the Peer Manager initialization.
 */
void bargo_peer_manager_init(void);


/**@brief Function for starting advertising.
 */
void bargo_advertising_start(void);

/**@brief   Function for transmitting a key scan Press & Release Notification.
 *
 * @warning This handler is an example only. You need to analyze how you wish to send the key
 *          release.
 *
 * @param[in]  p_instance     Identifies the service for which Key Notifications are requested.
 * @param[in]  p_key_pattern  Pointer to key pattern.
 * @param[in]  pattern_len    Length of key pattern. 0 < pattern_len < 7.
 * @return     NRF_SUCCESS on success, NRF_ERROR_RESOURCES in case transmission could not be
 *             completed due to lack of transmission buffer or other error codes indicating reason
 *             for failure.
 */
uint32_t bargo_send_key_scan_press_release(ble_hids_t * p_hids,
                                            uint8_t    * p_key_pattern,
                                            uint16_t     pattern_len);


/**@brief Function for sending sample key presses to the peer.
 *
 * @param[in]   key_pattern_len   Pattern length.
 * @param[in]   p_key_pattern     Pattern to be sent.
 */
void bargo_ble_keys_send(uint8_t key_pattern_len, uint8_t * p_key_pattern);

// 初始化电量检测
void bargo_battery_level_init();

// 断开所有已经连接的设备，并设置不再广播
void bargo_ble_disconnect();

// 删除绑定
void delete_bond(void);

// 睡眠模式
void bargo_sleep_mode_enter(void);

#endif