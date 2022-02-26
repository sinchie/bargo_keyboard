#ifndef BARGO_SWITCH_H
#define BARGO_SWITCH_H

#include <stdint.h>
#include <stdbool.h>

/* 
  File IDs should be in the range 0x0000 - 0xBFFF. The value 0xFFFF is used by the system. 
  The values from 0xC000 to 0xFFFE are reserved for use by the Peer Manager module and
  can only be used in applications that do not include Peer Manager.
 */
// 存储文件ID
#define BARGO_SWITCH_STORAGE_FILE  (0xB000)
/* 
  Record keys should be in the range 0x0001 - 0xBFFF. 
  The value 0x0000 is reserved by the system. The values from 0xC000 to 0xFFFF are reserved for use by the Peer Manager module and
  can only be used in applications that do not include Peer Manager.
 */
// 存储记录KEY - 当前蓝牙设备序号
#define BARGO_SWITCH_STORAGE_RECORD  (0xB001)

// 切换设备index
#define BARGO_SWITCH_INDEX_USB 0 
#define BARGO_SWITCH_INDEX_BLE1 1
#define BARGO_SWITCH_INDEX_BLE2 2 
#define BARGO_SWITCH_INDEX_BLE3 3 

// 总切换设备数量（USB+蓝牙设备数量）
#define BARGO_SWITCH_DEVICE_COUNT 4

// 蓝牙切换状态数据结构
typedef struct
{
    // 当前选中设备
    uint8_t current_device;
    // 每个蓝牙设备是否配对过
    bool ble_device_paired[BARGO_SWITCH_DEVICE_COUNT];
    // 每个蓝牙设备的配对ID
    uint16_t ble_device_peer_id[BARGO_SWITCH_DEVICE_COUNT];

} bargo_switch_config_t;

// 初始化蓝牙设备切换配置
void bargo_switch_init();

// 切换设备
void bargo_switch_device_select(uint8_t current_device);

// 获取当前设备
uint8_t bargo_switch_get_device();

// 设置蓝牙设备配对ID
void bargo_switch_set_ble_peerid(uint16_t peer_id);

// 获取蓝牙设备配对ID
uint16_t bargo_switch_get_ble_peerid();

// 当前设备是否配对过
bool bargo_switch_device_ble_paired();

// 设置蓝牙设备配对无效
void bargo_switch_invalidate_ble_peerid();

// 是否正在使用USB通讯
bool bargo_switch_is_using_usb();

// 获取蓝牙设备配对ID
uint16_t bargo_switch_get_ble_peerid_by_device(uint8_t device_num);


#endif

