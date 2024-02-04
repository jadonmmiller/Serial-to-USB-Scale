/* This emulates the following descriptor from a Pitney Bowes scale:

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
*/

#include <Arduino.h>
#include <USBComposite.h>

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

typedef struct
{
  uint8_t reportID = 3;
  uint8_t status = 4;
  uint8_t unit = 12;
  int8_t scaling = 1;
  uint16_t weight;

} __packed dataReport_t;

dataReport_t dataReport;

USBHID HID;

HIDReportDescriptor dataReportDescriptor = {descriptor, sizeof(descriptor)};

HIDReporter dataReporter(HID, &dataReportDescriptor, (uint8_t *)&dataReport, sizeof(dataReport), dataReport.reportID);

void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);
  HID.begin(descriptor, sizeof(descriptor));
  while (!USBComposite)
    ;
}

void loop()
{
  dataReport.weight = 6;

  dataReporter.sendReport();

  delay(1000);
  digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
}