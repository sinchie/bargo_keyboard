#include <stdint.h>
#include <stdbool.h>
#include "bargo_switch.h"
#include "bargo_storage.h"
#include "bargo_ble.h"
#include "app_error.h"
#include "fds.h"
#include "nrf_log.h"

// 蓝牙切换当前配置
__ALIGN(4) bargo_switch_config_t bargo_switch_config;

// FDS记录结构
fds_record_t const bargo_switch_config_record =
{
    .file_id           = BARGO_SWITCH_STORAGE_FILE,
    .key               = BARGO_SWITCH_STORAGE_RECORD,
    .data.p_data       = &bargo_switch_config,
    /* The length of a record is always expressed in 4-byte units (words). */
    .data.length_words = (sizeof(bargo_switch_config) + 3) / sizeof(uint32_t),
};

// 更新操作
static void bargo_switch_config_persistent()
{
    ret_code_t rc;

    fds_record_desc_t desc = {0};
    fds_find_token_t  tok  = {0};
    
    // 查询FDS文件描述符
    rc = fds_record_find(BARGO_SWITCH_STORAGE_FILE, BARGO_SWITCH_STORAGE_RECORD, &desc, &tok);
    if (rc == NRF_SUCCESS) {
        // 更新数据
        rc = fds_record_update(&desc, &bargo_switch_config_record);
        if ((rc != NRF_SUCCESS) && (rc == FDS_ERR_NO_SPACE_IN_FLASH))
        {
            NRF_LOG_INFO("bargo_switch_config_persistent No space in flash, delete some records to update the config file.");
        }
        else
        {
            APP_ERROR_CHECK(rc);
        }
    } 
    else 
    {
        APP_ERROR_CHECK(rc);
    }
}

// 获取当前设备
uint8_t bargo_switch_get_device()
{
    return bargo_switch_config.current_device;
}

// 是否正在使用USB通讯
bool bargo_switch_is_using_usb()
{
    if(bargo_switch_config.current_device == BARGO_SWITCH_INDEX_USB) {
        return true;
    }

    return false;
}

/**
 * @brief 切换连接设备.
 *
 * @param[in] id  要切换的设备的ID号 [0-3]
 */
void bargo_switch_device_select(uint8_t id)
{
    NRF_LOG_INFO("bargo_switch_device_select: %d", id);

    // 如果重复切换，则直接退出，不做任何操作
    if (id == bargo_switch_config.current_device){
        return;
    }

    // 更新数据
    bargo_switch_config.current_device = id;
    // 持久化存储
    bargo_switch_config_persistent();


    // 断开所有连接，并关闭广播
    bargo_ble_disconnect();

    if(!bargo_switch_is_using_usb()) {
        // 重新开始广播
        bargo_advertising_start();
    }
}

// 设置蓝牙设备配对ID
void bargo_switch_set_ble_peerid(uint16_t peer_id)
{
    // 更新数据
    bargo_switch_config.ble_device_peer_id[bargo_switch_config.current_device] = peer_id;
    bargo_switch_config.ble_device_paired[bargo_switch_config.current_device] = true;
    // 持久化存储
    bargo_switch_config_persistent();
}

// 获取蓝牙设备配对ID
uint16_t bargo_switch_get_ble_peerid()
{
    return bargo_switch_config.ble_device_peer_id[bargo_switch_config.current_device];
}

// 获取蓝牙设备配对ID
uint16_t bargo_switch_get_ble_peerid_by_device(uint8_t device_num)
{
    return bargo_switch_config.ble_device_peer_id[device_num];
}

// 当前设备是否配对过
bool bargo_switch_device_ble_paired()
{
    return bargo_switch_config.ble_device_paired[bargo_switch_config.current_device];
}

// 设置蓝牙设备配对无效
void bargo_switch_invalidate_ble_peerid()
{
    // 更新数据
    bargo_switch_config.ble_device_paired[bargo_switch_config.current_device] = false;
    // 持久化存储
    bargo_switch_config_persistent();
}

// 初始化设备切换配置
void bargo_switch_init()
{
    ret_code_t rc;

    fds_record_desc_t desc = {0};
    fds_find_token_t  tok  = {0};
    
    // 查询FDS文件描述符
    rc = fds_record_find(BARGO_SWITCH_STORAGE_FILE, BARGO_SWITCH_STORAGE_RECORD, &desc, &tok);
    if (rc == NRF_SUCCESS)
    {
        /* A config file is in flash. Let's update it. */
        fds_flash_record_t config = {0};

        /* Open the record and read its contents. */
        rc = fds_record_open(&desc, &config);
        APP_ERROR_CHECK(rc);

        /* Copy the configuration from flash into bargo_switch_config. */
        memcpy(&bargo_switch_config, config.p_data, sizeof(bargo_switch_config_t));

        NRF_LOG_INFO("bargo_switch_config file found.");

        /* Close the record when done reading. */
        rc = fds_record_close(&desc);
        APP_ERROR_CHECK(rc);
    }
    else
    {
        /* bargo_switch_config not found; write a new one. */
        NRF_LOG_INFO("Writing bargo_switch_config file...");

        // 默认值
        memset(&bargo_switch_config, 0, sizeof(bargo_switch_config_t));
        bargo_switch_config.current_device = BARGO_SWITCH_INDEX_USB;
        int8_t device_num = BARGO_SWITCH_DEVICE_COUNT;
        while(1){
            device_num--;
            if(device_num < 0) {
                break;
            }

            bargo_switch_config.ble_device_paired[BARGO_SWITCH_DEVICE_COUNT]=false;
            bargo_switch_config.ble_device_peer_id[BARGO_SWITCH_DEVICE_COUNT]=0;
        }

        rc = fds_record_write(&desc, &bargo_switch_config_record);
        if ((rc != NRF_SUCCESS) && (rc == FDS_ERR_NO_SPACE_IN_FLASH))
        {
            NRF_LOG_INFO("bargo_switch_init No space in flash, delete some records to update the config file.");
        }
        else if(rc != NRF_SUCCESS)
        {
            APP_ERROR_CHECK(rc);
        }
    }


    if(!bargo_switch_is_using_usb()) {
        // 开始广播
        bargo_advertising_start();
    }
}

