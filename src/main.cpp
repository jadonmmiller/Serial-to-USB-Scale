/*********************************************************************
 Adafruit invests time and resources providing this open source code,
 please support Adafruit and open-source hardware by purchasing
 products from Adafruit!

 MIT license, check LICENSE for more information
 Copyright (c) 2019 Ha Thach for Adafruit Industries
 All text above, and the splash screen below must be included in
 any redistribution
*********************************************************************/
#include "Adafruit_TinyUSB.h"

void hid_report_callback(uint8_t report_id, hid_report_type_t report_type, uint8_t const *buffer, uint16_t bufsize);

#define ARDUINO_FUNHOUSE_ESP32S2

/* This sketch demonstrates USB HID keyboard.
 * - PIN A0-A3 is used to send digit '0' to '3' respectively
 *   (On the RP2040, pins D0-D5 used)
 * - LED and/or Neopixels will be used as Capslock indicator
 */

// HID report descriptor
const uint8_t descriptor[] = {
    0x05, 0x8D,                   // Usage Page (Scale Page)
    0x09, 0x20,                   // Usage (Scale Device)
    0xA1, 0x01,                   // Collection (Application)
    0x09, 0x30,                   //   Usage (Scale Attribute Report)
    0xA1, 0x02,                   //   Collection (Logical)
    0x85, 0x01,                   //     Report ID (1)
    0x75, 0x08,                   //     Report Size (8)
    0x95, 0x01,                   //     Report Count (1)
    0x15, 0x01,                   //     Logical Minimum (1)
    0x09, 0x27,                   //     Usage (Scale Class III English)
    0xA1, 0x02,                   //     Collection (Logical)
    0x25, 0x0A,                   //       Logical Maximum (10)
    0x19, 0x21,                   //       Usage Minimum (Scale Class)
    0x29, 0x2A,                   //       Usage Maximum (Scale Class Generic)
    0xB1, 0x00,                   //       Feature (Data,Array,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
    0xC0,                         //     End Collection
    0x09, 0x50,                   //     Usage (Weight Unit)
    0xA1, 0x02,                   //     Collection (Logical)
    0x25, 0x0C,                   //       Logical Maximum (12)
    0x19, 0x51,                   //       Usage Minimum (Weight Unit Milligram)
    0x29, 0x5C,                   //       Usage Maximum (Weight Unit Pound)
    0xB1, 0x00,                   //       Feature (Data,Array,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
    0xC0,                         //     End Collection
    0xC0,                         //   End Collection
    0x09, 0x31,                   //   Usage (Scale Control Report)
    0xA1, 0x02,                   //   Collection (Logical)
    0x85, 0x02,                   //     Report ID (2)
    0x75, 0x01,                   //     Report Size (1)
    0x95, 0x02,                   //     Report Count (2)
    0x15, 0x00,                   //     Logical Minimum (0)
    0x25, 0x01,                   //     Logical Maximum (1)
    0x09, 0x81,                   //     Usage (Enforced Zero Return)
    0x09, 0x80,                   //     Usage (Zero Scale)
    0x91, 0x02,                   //     Output (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
    0x75, 0x06,                   //     Report Size (6)
    0x95, 0x01,                   //     Report Count (1)
    0x91, 0x03,                   //     Output (Const,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
    0xC0,                         //   End Collection
    0x09, 0x32,                   //   Usage (Scale Data Report)
    0xA1, 0x02,                   //   Collection (Logical)
    0x85, 0x03,                   //     Report ID (3)
    0x75, 0x08,                   //     Report Size (8)
    0x15, 0x01,                   //     Logical Minimum (1)
    0x09, 0x70,                   //     Usage (Scale Status)
    0xA1, 0x02,                   //     Collection (Logical)
    0x25, 0x08,                   //       Logical Maximum (8)
    0x19, 0x71,                   //       Usage Minimum (Scale Status Fault)
    0x29, 0x78,                   //       Usage Maximum (Scale Status Requires Rezeroing)
    0x81, 0x00,                   //       Input (Data,Array,Abs,No Wrap,Linear,Preferred State,No Null Position)
    0xC0,                         //     End Collection
    0x09, 0x50,                   //     Usage (Weight Unit)
    0xA1, 0x02,                   //     Collection (Logical)
    0x25, 0x0C,                   //       Logical Maximum (12)
    0x19, 0x51,                   //       Usage Minimum (Weight Unit Milligram)
    0x29, 0x5C,                   //       Usage Maximum (Weight Unit Pound)
    0x81, 0x00,                   //       Input (Data,Array,Abs,No Wrap,Linear,Preferred State,No Null Position)
    0xC0,                         //     End Collection
    0x15, 0x81,                   //     Logical Minimum (-127)
    0x25, 0x7F,                   //     Logical Maximum (127)
    0x09, 0x41,                   //     Usage (Data Scaling)
    0x81, 0x02,                   //     Input (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position)
    0x75, 0x10,                   //     Report Size (16)
    0x15, 0x00,                   //     Logical Minimum (0)
    0x27, 0xFF, 0xFF, 0x00, 0x00, //     Logical Maximum (65534)
    0x09, 0x40,                   //     Usage (Data Weight)
    0x81, 0x02,                   //     Input (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position)
    0xC0,                         //   End Collection
    0x09, 0x34,                   //   Usage (Scale Weight Limit Report)
    0xA1, 0x02,                   //   Collection (Logical)
    0x85, 0x05,                   //     Report ID (5)
    0x75, 0x08,                   //     Report Size (8)
    0x95, 0x01,                   //     Report Count (1)
    0x09, 0x50,                   //     Usage (Weight Unit)
    0xA1, 0x02,                   //     Collection (Logical)
    0x15, 0x01,                   //       Logical Minimum (1)
    0x25, 0x0C,                   //       Logical Maximum (12)
    0x19, 0x51,                   //       Usage Minimum (Weight Unit Milligram)
    0x29, 0x5C,                   //       Usage Maximum (Weight Unit Pound)
    0xB1, 0x00,                   //       Feature (Data,Array,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
    0xC0,                         //     End Collection
    0x15, 0x81,                   //     Logical Minimum (-127)
    0x25, 0x7F,                   //     Logical Maximum (127)
    0x09, 0x41,                   //     Usage (Data Scaling)
    0xB1, 0x02,                   //     Feature (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
    0x75, 0x10,                   //     Report Size (16)
    0x15, 0x00,                   //     Logical Minimum (0)
    0x27, 0xFF, 0xFF, 0x00, 0x00, //     Logical Maximum (65534)
    0x09, 0x40,                   //     Usage (Data Weight)
    0xB1, 0x03,                   //     Feature (Const,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
    0xC0,                         //   End Collection
    0x09, 0x35,                   //   Usage (Scale Statistics Report)
    0xA1, 0x02,                   //   Collection (Logical)
    0x85, 0x06,                   //     Report ID (6)
    0x75, 0x10,                   //     Report Size (16)
    0x95, 0x02,                   //     Report Count (2)
    0x15, 0x00,                   //     Logical Minimum (0)
    0x27, 0xFF, 0xFF, 0x00, 0x00, //     Logical Maximum (65534)
    0x09, 0x60,                   //     Usage (Calibration Count)
    0x09, 0x61,                   //     Usage (Re-Zero Count)
    0xB1, 0x03,                   //     Feature (Const,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
    0xC0,                         //   End Collection
    0x09, 0x30,                   //   Usage (Scale Attribute Report)
    0xA1, 0x02,                   //   Collection (Logical)
    0x85, 0x07,                   //     Report ID (7)
    0x75, 0x08,                   //     Report Size (8)
    0x95, 0x0F,                   //     Report Count (15)
    0x15, 0x00,                   //     Logical Minimum (0)
    0x25, 0x7F,                   //     Logical Maximum (127)
    0x09, 0x40,                   //     Usage (Data Weight)
    0xB1, 0x02,                   //     Feature (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
    0xC0,                         //   End Collection
    0xC0,                         // End Collection
};

// Constants
#define USAGE_SCALE_CLASS 1
#define USAGE_CLASS_I_METRIC 2
#define USAGE_CLASS_II_METRIC 3
#define USAGE_CLASS_III_METRIC 4
#define USAGE_CLASS_IIIL_METRIC 5
#define USAGE_CLASS_IV_METRIC 6
#define USAGE_CLASS_III_ENGLISH 7
#define USAGE_CLASS_IIIL_ENGLISH 8
#define USAGE_CLASS_IV_ENGLISH 9
#define USAGE_SCALE_CLASS_GENERIC 10
#define USAGE_WEIGHT_UNIT_MILLIGRAM 1
#define USAGE_WEIGHT_UNIT_GRAM 2
#define USAGE_WEIGHT_UNIT_KILOGRAM 3
#define USAGE_WEIGHT_UNIT_CARATS 4
#define USAGE_WEIGHT_UNIT_TAELS 5
#define USAGE_WEIGHT_UNIT_GRAINS 6
#define USAGE_WEIGHT_UNIT_PENNYWEIGHTS 7
#define USAGE_WEIGHT_UNIT_METRIC_TON 8
#define USAGE_WEIGHT_UNIT_AVOIR_TON 9
#define USAGE_WEIGHT_UNIT_TROY_OUNCE 10
#define USAGE_WEIGHT_UNIT_OUNCE 11
#define USAGE_WEIGHT_UNIT_POUND 12
#define USAGE_STATUS_FAULT 1
#define USAGE_STATUS_STABLE_AT_ZERO 2
#define USAGE_STATUS_IN_MOTION 3
#define USAGE_STATUS_WEIGHT_STABLE 4
#define USAGE_STATUS_UNDER_ZERO 5
#define USAGE_STATUS_OVER_LIMIT 6
#define USAGE_STATUS_REQUIRES_CAL 7
#define USAGE_STATUS_REQUIRES_ZERO 8

#define SCALE_DATA_REPORT_ID 3

#define KG_TO_LB_CONVERSION_FACTOR 2.20462262

// Configuration Settings
#define SOFTWARE_VERSION 0.4
#define HARDWARE_VERSION 0.2

#define ENABLE_HEARTBEAT
#ifdef ENABLE_HEARTBEAT
#define HEARTBEAT_PIN LED_BUILTIN
#define HEARTBEAT_INTERVAL 250
#endif

// #define ENABLE_DEBUG
#ifdef ENABLE_DEBUG
#define DEBUG_BAUD 9600
// #define DEBUG_HID
#define DEBUG_SCALES
#ifdef DEBUG_SCALES
// #define DEBUG_SCALES_RECEIVE
// #define DEBUG_SCALES_VERIFY
// #define DEBUG_SCALES_PARSE
#endif
#endif

#define HID_PRECISION_DIGITS 2

#define SCALES_BRAND_AVERY
#ifdef SCALES_BRAND_AVERY
#define SCALES_BAUD 9600
#define SCALES_POLL_INTERVAL 1500
#define SCALES_POLL_STRING "p"
#define SCALES_RESPONSE_SIZE 75
#define SCALES_NUMBER_OF_VALUES 3
#define SCALES_WEIGHT_NUMBER 0
#define SCALES_TERMINATION "\n\n"
#define SCALES_RESPONSE_FORMAT_LB "GROSS WT:     0.00 lb\r\nCOUNT:            0\r\nPIECE WT: -------- lb\r\n\n"
#define SCALES_RESPONSE_FORMAT_KG "GROSS WT:     0.00 kg\r\nCOUNT:            0\r\nPIECE WT: -------- kg\r\n\n"
#define SCALES_RESPONSE_FORMAT_MASK "00000000001111111100000000000011111111111100000000000011111111000000" // Zeros indicate data that nevver changes, and ones indicate variables
#define SCALES_MAX_WEIGHT_LB 100.00
#define SCALES_MIN_WEIGHT_LB -100.00
#endif

// HID Variables
typedef struct
{
  uint8_t status = USAGE_STATUS_WEIGHT_STABLE;
  uint8_t unit;
  int8_t scaling;
  uint16_t weight;
} __packed dataReport_t;
dataReport_t dataReport;

// USB HID object
Adafruit_USBD_HID usb_hid;

//------------- Input Pins -------------//
// Array of pins and its keycode.
// Notes: these pins can be replaced by PIN_BUTTONn if defined in setup()
#ifdef ARDUINO_ARCH_RP2040
uint8_t pins[] = {15, 26, 27, 28};
#else
uint8_t pins[] = {15, 26, 27, 28};
#endif

// number of pins
uint8_t pincount = sizeof(pins) / sizeof(pins[0]);

// For keycode definition check out https://github.com/hathach/tinyusb/blob/master/src/class/hid/hid.h
uint8_t hidcode[] = {HID_KEY_0, HID_KEY_1, HID_KEY_2, HID_KEY_3};

#if defined(ARDUINO_SAMD_CIRCUITPLAYGROUND_EXPRESS) || defined(ARDUINO_NRF52840_CIRCUITPLAY) || defined(ARDUINO_FUNHOUSE_ESP32S2)
bool activeState = true;
#else
bool activeState = false;
#endif

// the setup function runs once when you press reset or power the board
void setup()
{
  // Manual begin() is required on core without built-in support e.g. mbed rp2040
  if (!TinyUSBDevice.isInitialized())
  {
    TinyUSBDevice.begin(0);
  }

  // Setup HID
  usb_hid.setBootProtocol(HID_ITF_PROTOCOL_NONE);
  usb_hid.setPollInterval(2);
  usb_hid.setReportDescriptor(descriptor, sizeof(descriptor));
  usb_hid.setStringDescriptor("Scales Adapter");

  usb_hid.begin();

  // If already enumerated, additional class driverr begin() e.g msc, hid, midi won't take effect until re-enumeration
  if (TinyUSBDevice.mounted())
  {
    TinyUSBDevice.detach();
    delay(10);
    TinyUSBDevice.attach();
  }

  // led pin
  pinMode(29, OUTPUT);
  digitalWrite(29, HIGH);
}
void process_scales()
{
  // skip if hid is not ready e.g still transferring previous report
  if (!usb_hid.ready())
    return;

  digitalWrite(29, LOW);

  // Create a data report
  dataReport.unit = USAGE_WEIGHT_UNIT_POUND;
  dataReport.scaling = -HID_PRECISION_DIGITS; // -2 makes the scale expect 1.23 to be encoded as 123
  dataReport.weight = 125;

  usb_hid.sendReport(SCALE_DATA_REPORT_ID, (uint8_t *)&dataReport, sizeof(dataReport));
}


void loop()
{
#ifdef TINYUSB_NEED_POLLING_TASK
  // Manual call tud_task since it isn't called by Core's background
  TinyUSBDevice.task();
#endif

  // not enumerated()/mounted() yet: nothing to do
  if (!TinyUSBDevice.mounted())
  {
    return;
  }

  // poll gpio once each 2 ms
  static uint32_t ms = 0;
  if (millis() - ms > 2)
  {
    ms = millis();
    process_scales();
  }
}