#include "bargo_keymap.h"
#include "action.h"
#include "bootloader.h"
#include "bargo_switch.h"
#include "bargo_config.h"
#include "bargo_ble.h"


const uint8_t keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
   KEYMAP(KC_FN0, KC_FN1, KC_FN2, KC_D, KC_E, KC_F)
};

const action_t fn_actions[] = {
    ACTION_FUNCTION(0),
    ACTION_FUNCTION(1),
    ACTION_FUNCTION(2),
};

// 自定义函数
void action_function(keyrecord_t *record, uint8_t id, uint8_t opt) {
    NRF_LOG_INFO("action_function id: %d", id);
    if(id==0) {
      //bootloader_jump();
      bargo_switch_device_select(BARGO_SWITCH_INDEX_USB);
    }
    if(id==1) {
      bargo_switch_device_select(BARGO_SWITCH_INDEX_BLE1);
    }
    if(id==2) {
      delete_bond();
    }
}