#include "bootloader.h"
#include "nrf.h"
#include "nrf_soc.h"
#include "nrf_nvic.h"
#include "bargo_usb.h"

// adafruit bootloader, send "dfu" to debug serial port
void bootloader_jump(void) {
    if(!usb_power_enable()) {
        return;
    }
    NRF_POWER->GPREGRET = 0x57; // 0xA8 OTA, 0x4e Serial
    NVIC_SystemReset();         // or sd_nvic_SystemReset();
}