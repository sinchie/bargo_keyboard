#include <stdint.h>
#include <stdbool.h>
#include "matrix.h"
#include "nrf_gpio.h"
#include "wait.h"
#include "bargo_config.h"
#include "nrf_log.h"

/*
 * scan matrix
 */
static bool debouncing = false;

/* matrix state(1:on, 0:off) */
static matrix_row_t matrix[MATRIX_ROWS];
static matrix_row_t matrix_debouncing[MATRIX_ROWS];

static void select_row(uint8_t row);
static matrix_row_t read_cols(void);
static void unselect_row(uint8_t row);
static void unselect_rows(void);

inline
uint8_t matrix_rows(void)
{
    return MATRIX_ROWS;
}

inline
uint8_t matrix_cols(void)
{
    return MATRIX_COLS;
}

#define LED_ON()    do { /*palSetPad(TEENSY_PIN13_IOPORT, TEENSY_PIN13);*/} while (0)
#define LED_OFF()   do { /*palClearPad(TEENSY_PIN13_IOPORT, TEENSY_PIN13);*/} while (0)
#define LED_TGL()   do { /*palTogglePad(TEENSY_PIN13_IOPORT, TEENSY_PIN13);*/} while (0)


/* Column pin configuration
 */
static void init_cols_rows(void)
{
    for (uint8_t i = 0;  i < MATRIX_COLS; i++) {
        nrf_gpio_cfg_default(col_pins[i]);
    }
    for (uint8_t i = 0;  i < MATRIX_ROWS; i++) {
        nrf_gpio_cfg_default(row_pins[i]);
    }

    for (uint8_t i = 0;  i < MATRIX_COLS; i++) {
        nrf_gpio_cfg_input(col_pins[i], NRF_GPIO_PIN_PULLUP);
    }
    for (uint8_t i = 0;  i < MATRIX_ROWS; i++) {
        nrf_gpio_cfg_output(row_pins[i]);
    }
}


/* Row pin configuration
 */
static void unselect_row(uint8_t row)
{
   // 设置高电平
   nrf_gpio_pin_set((uint32_t)row_pins[row]);
}

static void unselect_rows(void)
{
  for (uint8_t i = 0; i < MATRIX_ROWS; i++) {
    // 设置高电平
    nrf_gpio_pin_set((uint32_t)row_pins[i]);
  }
}

static void select_row(uint8_t row)
{
    // 设置低电平
    nrf_gpio_pin_clear((uint32_t)row_pins[row]);
}

void matrix_init(void)
{
    // initialize row and col
    init_cols_rows();
    unselect_rows();
    // initialize matrix state: all keys off
    for (uint8_t i=0; i < MATRIX_ROWS; i++) {
        matrix[i] = 0;
        matrix_debouncing[i] = 0;
    }

}

uint8_t matrix_scan(void)
{
    for (uint8_t i = 0; i < MATRIX_ROWS; i++) {
        select_row(i);
        wait_us(30);  // without this wait read unstable value.
        matrix_row_t cols = read_cols();

        if (matrix_debouncing[i] != cols) {
            matrix_debouncing[i] = cols;
            debouncing = true;
        }
        unselect_row(i);
    }

    if (debouncing) {
        debouncing = false;
    } else {
        for (uint8_t i = 0; i < MATRIX_ROWS; i++) {
            matrix[i] = matrix_debouncing[i];
        }
    }

    return 1;
}

inline
matrix_row_t matrix_get_row(uint8_t row)
{
    return matrix[row];
}



/* Returns status of switches(1:on, 0:off) */
static matrix_row_t read_cols(void)
{
    uint32_t result = 0;
    for (uint8_t i = 0; i < MATRIX_COLS; i++) {
        if (!nrf_gpio_pin_read(((uint32_t)col_pins[i]))) {
            result |= 1 << i;
        }
    }

    return result;
}


void matrix_print(void)
{
    
}