#include "bargo_keymap.h"
#include "action.h"
#include "bootloader.h"
#include "bargo_switch.h"
#include "bargo_config.h"
#include "bargo_ble.h"
#include "bargo_service.h"
#include "bargo_keyboard.h"

enum bargo_fn_enum {
    SWITCH_DEVICE_USB,
    SWITCH_DEVICE_BLE_1,
    SWITCH_DEVICE_BLE_2,
    SWITCH_DEVICE_BLE_3,
    SWITCH_DEVICE_BLE_4,
    SWITCH_DEVICE_BLE_REBOND,
    ENTER_BOOTLOADER,
    ENTER_SLEEP_MODE,
    ENTER_RESET,
    ENTER_BLE_GAME_MODE,
};

const uint8_t keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
     KEYMAP(
              KC_ESC, KC_F1, KC_F2, KC_F3, KC_F4, KC_F5, KC_F6, KC_F7, KC_F8, KC_F9, KC_F10, KC_F11, KC_F12, KC_INSERT, 
              KC_GRV, KC_1, KC_2, KC_3, KC_4, KC_5, KC_6, KC_7, KC_8, KC_9, KC_0, KC_MINS, KC_EQL, KC_BSPC, KC_HOME, 
              KC_TAB, KC_Q, KC_W, KC_E, KC_R, KC_T, KC_Y, KC_U, KC_I, KC_O, KC_P, KC_LBRC, KC_RBRC, KC_BSLS, KC_PGUP,
              KC_CAPS, KC_A, KC_S, KC_D, KC_F, KC_G, KC_H, KC_J, KC_K, KC_L, KC_SCLN, KC_QUOT, KC_ENT, KC_PGDN,
              KC_LSFT, KC_Z, KC_X, KC_C, KC_V, KC_B, KC_N, KC_M, KC_COMM, KC_DOT, KC_SLSH, KC_RSFT, KC_UP, KC_END,
              KC_LCTL, KC_LGUI, KC_LALT, KC_SPC, KC_RALT, KC_FN0, KC_DEL, KC_LEFT, KC_DOWN, KC_RIGHT),
     KEYMAP(
        KC_FN6, KC_FN1, KC_FN2, KC_FN3, KC_FN4, KC_FN5, KC_FN8, KC_FN9, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_FN10, 
        KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, 
        KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
        KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
        KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_FN11, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_MPLY, KC_VOLU, KC_TRNS,
        KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_MUTE, KC_MPRV, KC_VOLD, KC_MNXT),
     KEYMAP(
        KC_FN7, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, 
        KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, 
        KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
        KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
        KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
        KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS)
};

const action_t fn_actions[] = {
    ACTION_LAYER_MOMENTARY(1), // FN0 切换到1层

    ACTION_FUNCTION(SWITCH_DEVICE_BLE_1), // FN1 切换到蓝牙1
    ACTION_FUNCTION(SWITCH_DEVICE_BLE_2), // FN2 切换到蓝牙2
    ACTION_FUNCTION(SWITCH_DEVICE_BLE_3), // FN3 切换到蓝牙3
    ACTION_FUNCTION(SWITCH_DEVICE_BLE_4), // FN4 切换到蓝牙3
    ACTION_FUNCTION(SWITCH_DEVICE_BLE_REBOND), // FN5 解绑蓝牙
    ACTION_FUNCTION(SWITCH_DEVICE_USB), // FN6 切换到USB

    ACTION_FUNCTION(ENTER_BOOTLOADER), // FN7 跳转bootloader
    ACTION_FUNCTION(ENTER_SLEEP_MODE), // FN8 睡眠模式
    ACTION_FUNCTION(ENTER_RESET), // FN9 重启
    ACTION_FUNCTION(ENTER_BLE_GAME_MODE), // FN10 游戏模式（蓝牙持续高扫描速度）

    ACTION_LAYER_MOMENTARY(2), // FN11 切换到2层
};

// 自定义函数
void action_function(keyrecord_t *record, uint8_t id, uint8_t opt) {
    enum bargo_fn_enum action;
    action = (enum bargo_fn_enum) id;

    if(action == SWITCH_DEVICE_BLE_1) {
        bargo_switch_device_select(BARGO_SWITCH_INDEX_BLE1);
    }
    if(action == SWITCH_DEVICE_BLE_2) {
        bargo_switch_device_select(BARGO_SWITCH_INDEX_BLE2);
    }
    if(action == SWITCH_DEVICE_BLE_3) {
        bargo_switch_device_select(BARGO_SWITCH_INDEX_BLE3);
    }
    if(action == SWITCH_DEVICE_BLE_4) {
        bargo_switch_device_select(BARGO_SWITCH_INDEX_BLE4);
    }
    if(action == SWITCH_DEVICE_BLE_REBOND) {
        bargo_delete_bond();
    }
    if(action == SWITCH_DEVICE_USB) {
        bargo_switch_device_select(BARGO_SWITCH_INDEX_USB);
    }

    if(action == ENTER_BOOTLOADER) {
        bootloader_jump();
    }
    if(action == ENTER_SLEEP_MODE) {
        bargo_sleep_mode_enter();
    }
    if(action == ENTER_RESET) {
        bargo_reset();
    }
    if(action == ENTER_BLE_GAME_MODE) {
        bargo_ble_game_mode_toggle();
    }
}