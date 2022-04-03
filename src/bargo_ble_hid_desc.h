#ifndef BARGO_HID_DESC_H
#define BARGO_HID_DESC_H

#include <stdint.h>
#include "hid_report_data.h"

// system & media
#define REPORT_SYSTEM_INDEX   0
#define REPORT_CONSUMER_INDEX 1
#define SYSTEM_REPORT_SIZE 2
#define CONSUMER_REPORT_SIZE 2

// hid描述符
enum {
    COMPOSITE_REPORT_ID_KEYBOARD = 1,
    COMPOSITE_REPORT_ID_SYSTEM,
    COMPOSITE_REPORT_ID_CONSUMER,
};
enum {
    COMPOSITE_REPORT_INDEX_KEYBOARD = 0,
    COMPOSITE_REPORT_INDEX_SYSTEM,
    COMPOSITE_REPORT_INDEX_CONSUMER,
    COMPOSITE_REPORT_COUNT,
};

const static uint8_t composite_report_map[] =
{
    HID_RI_USAGE_PAGE(8, 0x01), /* Generic Desktop */
    HID_RI_USAGE(8, 0x06), /* Keyboard */
    HID_RI_COLLECTION(8, 0x01), /* Application */
        HID_RI_REPORT_ID(8, COMPOSITE_REPORT_ID_KEYBOARD),
        HID_RI_USAGE_PAGE(8, 0x07), /* Key Codes */
        HID_RI_USAGE_MINIMUM(8, 0xE0), /* Keyboard Left Control */
        HID_RI_USAGE_MAXIMUM(8, 0xE7), /* Keyboard Right GUI */
        HID_RI_LOGICAL_MINIMUM(8, 0x00),
        HID_RI_LOGICAL_MAXIMUM(8, 0x01),
        HID_RI_REPORT_COUNT(8, 0x08),
        HID_RI_REPORT_SIZE(8, 0x01),
        HID_RI_INPUT(8, HID_IOF_DATA | HID_IOF_VARIABLE | HID_IOF_ABSOLUTE),

        HID_RI_REPORT_COUNT(8, 0x01),
        HID_RI_REPORT_SIZE(8, 0x08),
        HID_RI_INPUT(8, HID_IOF_CONSTANT),  /* reserved */

        HID_RI_USAGE_PAGE(8, 0x08), /* LEDs */
        HID_RI_USAGE_MINIMUM(8, 0x01), /* Num Lock */
        HID_RI_USAGE_MAXIMUM(8, 0x05), /* Kana */
        HID_RI_REPORT_COUNT(8, 0x05),
        HID_RI_REPORT_SIZE(8, 0x01),
        HID_RI_OUTPUT(8, HID_IOF_DATA | HID_IOF_VARIABLE | HID_IOF_ABSOLUTE | HID_IOF_NON_VOLATILE),
        HID_RI_REPORT_COUNT(8, 0x01),
        HID_RI_REPORT_SIZE(8, 0x03),
        HID_RI_OUTPUT(8, HID_IOF_CONSTANT),

        HID_RI_USAGE_PAGE(8, 0x07), /* Keyboard */
        HID_RI_USAGE_MINIMUM(8, 0x00), /* Reserved (no event indicated) */
        HID_RI_USAGE_MAXIMUM(8, 0xFF), /* Keyboard Application */
        HID_RI_LOGICAL_MINIMUM(8, 0x00),
        HID_RI_LOGICAL_MAXIMUM(16, 0x00FF),
        HID_RI_REPORT_COUNT(8, 0x06),
        HID_RI_REPORT_SIZE(8, 0x08),
        HID_RI_INPUT(8, HID_IOF_DATA | HID_IOF_ARRAY | HID_IOF_ABSOLUTE),
    HID_RI_END_COLLECTION(0),

    HID_RI_USAGE_PAGE(8, 0x01), /* Generic Desktop */
    HID_RI_USAGE(8, 0x80), /* System Control */
    HID_RI_COLLECTION(8, 0x01), /* Application */
        HID_RI_REPORT_ID(8, COMPOSITE_REPORT_ID_SYSTEM),
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
        HID_RI_REPORT_ID(8, COMPOSITE_REPORT_ID_CONSUMER),
        HID_RI_LOGICAL_MINIMUM(16, 0x0001),
        HID_RI_LOGICAL_MAXIMUM(16, 0x029C),
        HID_RI_USAGE_MINIMUM(16, 0x0001), /* +10 */
        HID_RI_USAGE_MAXIMUM(16, 0x029C), /* AC Distribute Vertically */
        HID_RI_REPORT_SIZE(8, 16),
        HID_RI_REPORT_COUNT(8, 1),
        HID_RI_INPUT(8, HID_IOF_DATA | HID_IOF_ARRAY | HID_IOF_ABSOLUTE),
    HID_RI_END_COLLECTION(0),
};

#endif

