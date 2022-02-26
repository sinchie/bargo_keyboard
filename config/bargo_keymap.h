#ifndef BARGO_KEYMAP_H
#define BARGO_KEYMAP_H

#include <stdint.h>
#include "keymap.h"
#include "bargo_config.h"

extern const uint8_t keymaps[][MATRIX_ROWS][MATRIX_COLS];
extern const action_t fn_actions[];

#define KEYMAP( \
	K00, K01, K02, \
	K03, K04, K05 \
) { \
	{ K00, K01, K02 }, \
	{ K03, K04, K05 } \
}

#endif
