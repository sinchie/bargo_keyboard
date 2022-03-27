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
#include "app_error.h"

#include "bargo_usb.h"


#define APP_USBD_INTERFACE_KBD   0

/**
 * @brief User event handler, HID keyboard
 */
static void hid_kbd_user_ev_handler(app_usbd_class_inst_t const * p_inst,
                                    app_usbd_hid_user_event_t event);

/**
 * @brief Global HID keyboard instance
 */
APP_USBD_HID_KBD_GLOBAL_DEF(m_app_hid_kbd,
                            APP_USBD_INTERFACE_KBD,
                            NRF_DRV_USBD_EPIN1,
                            hid_kbd_user_ev_handler,
                            APP_USBD_HID_SUBCLASS_BOOT
);

// usb是否通电
static bool usb_power = false;

// usb是否通电
bool usb_power_enable(void)
{
    return usb_power;
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
            NRF_LOG_DEBUG("APP_USBD_HID_USER_EVT_IN_REPORT_DONE");
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

    NRF_LOG_INFO("USBD HID composite example started.");

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

// usb事件处理主循环
void bargo_usb_process()
{
   while (app_usbd_event_queue_process())
   {
      /* Nothing to do */
   }
}