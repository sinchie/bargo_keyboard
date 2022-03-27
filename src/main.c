#include "bargo_service.h"
#include "bargo_storage.h"
#include "bargo_keyboard.h"
#include "bargo_usb.h"
#include "bargo_ble.h"

/**@brief Function for application main entry.
 */
int main(void)
{
    // 初始化日志
    bargo_log_init();
    // 初始化定时器
    bargo_timers_init();
    // 初始化电源管理
    bargo_power_management_init();
    // 初始化存储
    bargo_storage_init();

    // USB相关 
    bargo_usb_init();

    // 蓝牙相关
    // 初始化蓝牙协议栈
    bargo_ble_stack_init();
    // 初始化任务调度
    bargo_scheduler_init();
    // 初始化电池电量检测
    bargo_battery_level_init();
    // 初始化蓝牙通用接入规范参数
    bargo_gap_params_init();
    // 初始化蓝牙通用属性
    bargo_gatt_init(); 
    // 初始化蓝牙广播
    bargo_advertising_init();
    // 配对管理初始化
    bargo_peer_manager_init();
    // 初始化蓝牙相关服务（队列、hid、电池）
    bargo_ble_services_init();
    
    // 初始化设备切换
    bargo_switch_init();

    // 键盘矩阵扫描初始化
    bargo_keyboard_martix_sacn_init();

    // 主循环
    for (;;)
    {
        bargo_idle_state_handle();
    }
}
