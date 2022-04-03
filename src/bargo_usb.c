#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#include "nrf.h"
#include "nrf_drv_usbd.h"
#include "nrf_drv_power.h"
#include "nrf_log.h"
#include "app_usbd.h"
#include "app_usbd_core.h"
#include "app_usbd_hid_kbd.h"
#include "app_usbd_hid_generic.h"
#include "app_error.h"
#include "bargo_config.h"
#include "bargo_usb.h"
#include "hid_report_data.h"
#include "report.h"


// usb 接口枚举
enum {
    APP_USBD_INTERFACE_KBD=0,
    APP_USBD_INTERFACE_EXTRA
};

#define EXTRA_REPORT_IN_QUEUE_SIZE    1
#define EXTRA_REPORT_OUT_MAXSIZE  0
#define EXTRA_REPORT_FEATURE_MAXSIZE 0

#define EXTRA_ENDPOINT_LIST()                                \
(                                                            \
        NRF_DRV_USBD_EPIN2                                   \
)

// 扩展键码描述符
static const uint8_t extrakey_report[] =
{
    HID_RI_USAGE_PAGE(8, 0x01), /* Generic Desktop */
    HID_RI_USAGE(8, 0x80), /* System Control */
    HID_RI_COLLECTION(8, 0x01), /* Application */
        HID_RI_REPORT_ID(8, REPORT_ID_SYSTEM),
        HID_RI_LOGICAL_MINIMUM(16, 0x0001),
        HID_RI_LOGICAL_MAXIMUM(16, 0x0003),
        HID_RI_USAGE_MINIMUM(16, 0x0081), /* System Power Down */
        HID_RI_USAGE_MAXIMUM(16, 0x0083), /* System Wake Up */
        HID_RI_REPORT_SIZE(8, 16),
        HID_RI_REPORT_COUNT(8, 1),
        HID_RI_INPUT(8, HID_IOF_DATA | HID_IOF_ARRAY | HID_IOF_ABSOLUTE),
    HID_RI_END_COLLECTION(0),

    HID_RI_USAGE_PAGE(8, 0x0C), /* Consumer */
    HID_RI_USAGE(8, 0x01), /* Consumer Control */
    HID_RI_COLLECTION(8, 0x01), /* Application */
        HID_RI_REPORT_ID(8, REPORT_ID_CONSUMER),
        HID_RI_LOGICAL_MINIMUM(16, 0x0001),
        HID_RI_LOGICAL_MAXIMUM(16, 0x029C),
        HID_RI_USAGE_MINIMUM(16, 0x0001), /* +10 */
        HID_RI_USAGE_MAXIMUM(16, 0x029C), /* AC Distribute Vertically */
        HID_RI_REPORT_SIZE(8, 16),
        HID_RI_REPORT_COUNT(8, 1),
        HID_RI_INPUT(8, HID_IOF_DATA | HID_IOF_ARRAY | HID_IOF_ABSOLUTE),
    HID_RI_END_COLLECTION(0),
};

static const app_usbd_hid_subclass_desc_t extra_dsc = {
    sizeof(extrakey_report),
    APP_USBD_DESCRIPTOR_REPORT,
    extrakey_report
};

static const app_usbd_hid_subclass_desc_t * reps[] = {&extra_dsc};

/**
 * @brief Class specific event handler.
 *
 * @param p_inst    Class instance.
 * @param event     Class specific event.
 * */
static void hid_extra_user_ev_handler(app_usbd_class_inst_t const * p_inst,
                                      app_usbd_hid_user_event_t event);

// 初始化键盘补充键码实例
APP_USBD_HID_GENERIC_GLOBAL_DEF(m_app_hid_extra,
                                APP_USBD_INTERFACE_EXTRA,
                                hid_extra_user_ev_handler,
                                EXTRA_ENDPOINT_LIST(),
                                reps,
                                EXTRA_REPORT_IN_QUEUE_SIZE,
                                EXTRA_REPORT_OUT_MAXSIZE,
                                EXTRA_REPORT_FEATURE_MAXSIZE,
                                APP_USBD_HID_SUBCLASS_NONE,
                                APP_USBD_HID_PROTO_GENERIC
);


/**
 * @brief User event handler, HID keyboard 键盘事件回调
 */
static void hid_kbd_user_ev_handler(app_usbd_class_inst_t const * p_inst,
                                    app_usbd_hid_user_event_t event);

/**
 * @brief Global HID keyboard instance 初始化键盘实例
 */
APP_USBD_HID_KBD_GLOBAL_DEF(m_app_hid_kbd,
                            APP_USBD_INTERFACE_KBD,
                            NRF_DRV_USBD_EPIN1,
                            hid_kbd_user_ev_handler,
                            APP_USBD_HID_SUBCLASS_BOOT
);

/**
 * @brief Class specific event handler.
 *
 * @param p_inst    Class instance.
 * @param event     Class specific event.
 * */
static void hid_extra_user_ev_handler(app_usbd_class_inst_t const * p_inst,
                                      app_usbd_hid_user_event_t event)
{
    UNUSED_PARAMETER(p_inst);
    switch (event) {
        case APP_USBD_HID_USER_EVT_OUT_REPORT_READY:
            /* No output report defined for HID mouse.*/
            ASSERT(0);
            break;
        case APP_USBD_HID_USER_EVT_IN_REPORT_DONE:
            break;
        default:
            break;
    }
}


/**
 * @brief Class specific event handler.
 *
 * @param p_inst    Class instance.
 * @param event     Class specific event.
 * */
static void hid_kbd_user_ev_handler(app_usbd_class_inst_t const * p_inst,
                                    app_usbd_hid_user_event_t event)
{
    UNUSED_PARAMETER(p_inst);
    switch (event) {
        case APP_USBD_HID_USER_EVT_OUT_REPORT_READY:
            /* Only one output report IS defined for HID keyboard class. Update LEDs state. */
            break;
        case APP_USBD_HID_USER_EVT_IN_REPORT_DONE:
            usbd_clear_sending_flag();
            usbd_send_keyboard_buffered(&m_app_hid_kbd);
            break;
        case APP_USBD_HID_USER_EVT_SET_BOOT_PROTO:
            UNUSED_RETURN_VALUE(hid_kbd_clear_buffer(p_inst));
#if PROPAGATE_PROTOCOL
            hid_mouse_on_set_protocol(&m_app_hid_mouse, APP_USBD_HID_USER_EVT_SET_BOOT_PROTO);
#endif
            break;
        case APP_USBD_HID_USER_EVT_SET_REPORT_PROTO:
            UNUSED_RETURN_VALUE(hid_kbd_clear_buffer(p_inst));
#if PROPAGATE_PROTOCOL
            hid_mouse_on_set_protocol(&m_app_hid_mouse, APP_USBD_HID_USER_EVT_SET_REPORT_PROTO);
#endif
            break;
        default:
            break;
    }
}

// usb是否通电
static bool usb_power = false;

// usb是否通电
bool usb_power_enable(void)
{
    return usb_power;
}

/**
 * @brief USBD library specific event handler.
 *
 * @param event     USBD library event.
 * */
static void usbd_user_ev_handler(app_usbd_event_type_t event)
{
    switch (event)
    {
        case APP_USBD_EVT_DRV_SOF:
            break;
        case APP_USBD_EVT_DRV_SUSPEND:
            app_usbd_suspend_req(); // Allow the library to put the peripheral into sleep mode
            break;
        case APP_USBD_EVT_DRV_RESUME:
            break;
        case APP_USBD_EVT_STARTED:
            break;
        case APP_USBD_EVT_STOPPED:
            app_usbd_disable();
            break;
        case APP_USBD_EVT_POWER_DETECTED:
            NRF_LOG_INFO("USB power detected");
            if (!nrf_drv_usbd_is_enabled())
            {
                app_usbd_enable();
            }
            break;
        case APP_USBD_EVT_POWER_REMOVED:
            NRF_LOG_INFO("USB power removed");
            app_usbd_stop();
            usb_power = false;
            break;
        case APP_USBD_EVT_POWER_READY:
            NRF_LOG_INFO("USB ready");
            app_usbd_start();
            usb_power = true;
            break;
        default:
            break;
    }
}

// 初始化usb
void bargo_usb_init()
{
    ret_code_t ret;
    static const app_usbd_config_t usbd_config = {
        .ev_state_proc = usbd_user_ev_handler,
    };

    ret = app_usbd_init(&usbd_config);
    APP_ERROR_CHECK(ret);

    app_usbd_class_inst_t const * class_inst_kbd;
    class_inst_kbd = app_usbd_hid_kbd_class_inst_get(&m_app_hid_kbd);
    ret = app_usbd_class_append(class_inst_kbd);
    APP_ERROR_CHECK(ret);

    app_usbd_class_inst_t const * class_inst_extra;
    class_inst_extra = app_usbd_hid_generic_class_inst_get(&m_app_hid_extra);
    ret = app_usbd_class_append(class_inst_extra);
    APP_ERROR_CHECK(ret);

    NRF_LOG_INFO("USBD HID composite started.");

    ret = app_usbd_power_events_enable();
    APP_ERROR_CHECK(ret);
}

// usb发送按键
void bargo_usb_keys_send(report_keyboard_t *report) {

    usbd_send_kbd_report(&m_app_hid_kbd, report);

    if (nrf_drv_usbd_suspend_check()) {
      app_usbd_wakeup_req();
    }
}

typedef struct {
  uint8_t report_id;
  uint16_t usage;
}__attribute__ ((packed)) report_extra_t;

// usb发送系统按键
int bargo_usb_send_system(uint16_t data) 
{
    report_extra_t report;
    report.report_id = REPORT_ID_SYSTEM;
    report.usage = data;
    uint32_t ret = app_usbd_hid_generic_in_report_set(&m_app_hid_extra, &report, sizeof(report_extra_t));

    return ret;
}

// usb发送媒体按键
int bargo_usb_send_consumer(uint16_t data) 
{
    report_extra_t report;
    report.report_id = REPORT_ID_CONSUMER;
    report.usage = data;
    uint32_t ret = app_usbd_hid_generic_in_report_set(&m_app_hid_extra, &report, sizeof(report_extra_t));
    return ret;
}

// usb事件处理主循环
void bargo_usb_process()
{
   while (app_usbd_event_queue_process())
   {
      /* Nothing to do */
   }
}