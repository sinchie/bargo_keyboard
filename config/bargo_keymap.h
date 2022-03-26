#ifndef BARGO_KEYMAP_H
#define BARGO_KEYMAP_H

#include <stdint.h>
#include "keymap.h"
#include "bargo_config.h"

extern const uint8_t keymaps[][MATRIX_ROWS][MATRIX_COLS];
extern const action_t fn_actions[];

#define KEYMAP( \
	K00, K01, K02, K03, K04, K05, K06, K07, K08, K09, K0A, K0B, K0C,      K0D, \
        K10, K11, K12, K13, K14, K15, K16, K17, K18, K19, K1A, K1B, K1C, K1D, K1F, \
        K20, K21, K22, K23, K24, K25, K26, K27, K28, K29, K2A, K2B, K2C, K2D, K2F, \
        K30, K31, K32, K33, K34, K35, K36, K37, K38, K39, K3A, K3B, K3C,      K3D, \
        K40, K41, K42, K43, K44, K45, K46, K47, K48, K49, K4A, K4B,      K4C, K4D, \
        K50, K51, K52,           K53,                K54, K55, K56, K57, K58, K59 \
) { \
     {K00, K01, K02, K03, K04, K05, K06, K07, K08, K09, K0A, K0B, K0C, KC_NO, K0D}, \
     {K10, K11, K12, K13, K14, K15, K16, K17, K18, K19, K1A, K1B, K1C, K1D, K1F}, \
     {K20, K21, K22, K23, K24, K25, K26, K27, K28, K29, K2A, K2B, K2C, K2D, K2F}, \
     {K30, K31, K32, K33, K34, K35, K36, K37, K38, K39, K3A, K3B, K3C, KC_NO, K3D}, \
     {K40, K41, K42, K43, K44, K45, K46, K47, K48, K49, K4A, K4B, KC_NO, K4C, K4D}, \
     {K50, K51, K52, KC_NO, KC_NO, K53, KC_NO, KC_NO, KC_NO, K54, K55, K56, K57, K58, K59} \
}

#endif

