/*********************************************************************
This code communicates with an industrial scales over RS232 via a
MAX3232 chip. It then emulates a USB HID scales for integration with
shipping software such as Veeqo.

Created by Jadon Miller, Miller Industrial Services - 2/15/2025
License: The Unlicense
*********************************************************************/

#include <Arduino.h>
#include "Adafruit_TinyUSB.h"
#include <FastLED.h>

// ---------- Constants ----------
// HID Spec Constants
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

// Math
#define KG_TO_LB_CONVERSION_FACTOR 2.20462262

// HID Report Descriptor
const uint8_t descriptor[] = {
    0x05,
    0x8D, // Usage Page (Scale Page)
    0x09,
    0x20, // Usage (Scale Device)
    0xA1,
    0x01, // Collection (Application)
    0x09,
    0x30, //   Usage (Scale Attribute Report)
    0xA1,
    0x02, //   Collection (Logical)
    0x85,
    0x01, //     Report ID (1)
    0x75,
    0x08, //     Report Size (8)
    0x95,
    0x01, //     Report Count (1)
    0x15,
    0x01, //     Logical Minimum (1)
    0x09,
    0x27, //     Usage (Scale Class III English)
    0xA1,
    0x02, //     Collection (Logical)
    0x25,
    0x0A, //       Logical Maximum (10)
    0x19,
    0x21, //       Usage Minimum (Scale Class)
    0x29,
    0x2A, //       Usage Maximum (Scale Class Generic)
    0xB1,
    0x00, //       Feature (Data,Array,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
    0xC0, //     End Collection
    0x09,
    0x50, //     Usage (Weight Unit)
    0xA1,
    0x02, //     Collection (Logical)
    0x25,
    0x0C, //       Logical Maximum (12)
    0x19,
    0x51, //       Usage Minimum (Weight Unit Milligram)
    0x29,
    0x5C, //       Usage Maximum (Weight Unit Pound)
    0xB1,
    0x00, //       Feature (Data,Array,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
    0xC0, //     End Collection
    0xC0, //   End Collection
    0x09,
    0x31, //   Usage (Scale Control Report)
    0xA1,
    0x02, //   Collection (Logical)
    0x85,
    0x02, //     Report ID (2)
    0x75,
    0x01, //     Report Size (1)
    0x95,
    0x02, //     Report Count (2)
    0x15,
    0x00, //     Logical Minimum (0)
    0x25,
    0x01, //     Logical Maximum (1)
    0x09,
    0x81, //     Usage (Enforced Zero Return)
    0x09,
    0x80, //     Usage (Zero Scale)
    0x91,
    0x02, //     Output (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
    0x75,
    0x06, //     Report Size (6)
    0x95,
    0x01, //     Report Count (1)
    0x91,
    0x03, //     Output (Const,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
    0xC0, //   End Collection
    0x09,
    0x32, //   Usage (Scale Data Report)
    0xA1,
    0x02, //   Collection (Logical)
    0x85,
    0x03, //     Report ID (3)
    0x75,
    0x08, //     Report Size (8)
    0x15,
    0x01, //     Logical Minimum (1)
    0x09,
    0x70, //     Usage (Scale Status)
    0xA1,
    0x02, //     Collection (Logical)
    0x25,
    0x08, //       Logical Maximum (8)
    0x19,
    0x71, //       Usage Minimum (Scale Status Fault)
    0x29,
    0x78, //       Usage Maximum (Scale Status Requires Rezeroing)
    0x81,
    0x00, //       Input (Data,Array,Abs,No Wrap,Linear,Preferred State,No Null Position)
    0xC0, //     End Collection
    0x09,
    0x50, //     Usage (Weight Unit)
    0xA1,
    0x02, //     Collection (Logical)
    0x25,
    0x0C, //       Logical Maximum (12)
    0x19,
    0x51, //       Usage Minimum (Weight Unit Milligram)
    0x29,
    0x5C, //       Usage Maximum (Weight Unit Pound)
    0x81,
    0x00, //       Input (Data,Array,Abs,No Wrap,Linear,Preferred State,No Null Position)
    0xC0, //     End Collection
    0x15,
    0x81, //     Logical Minimum (-127)
    0x25,
    0x7F, //     Logical Maximum (127)
    0x09,
    0x41, //     Usage (Data Scaling)
    0x81,
    0x02, //     Input (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position)
    0x75,
    0x10, //     Report Size (16)
    0x15,
    0x00, //     Logical Minimum (0)
    0x27,
    0xFF,
    0xFF,
    0x00,
    0x00, //     Logical Maximum (65534)
    0x09,
    0x40, //     Usage (Data Weight)
    0x81,
    0x02, //     Input (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position)
    0xC0, //   End Collection
    0x09,
    0x34, //   Usage (Scale Weight Limit Report)
    0xA1,
    0x02, //   Collection (Logical)
    0x85,
    0x05, //     Report ID (5)
    0x75,
    0x08, //     Report Size (8)
    0x95,
    0x01, //     Report Count (1)
    0x09,
    0x50, //     Usage (Weight Unit)
    0xA1,
    0x02, //     Collection (Logical)
    0x15,
    0x01, //       Logical Minimum (1)
    0x25,
    0x0C, //       Logical Maximum (12)
    0x19,
    0x51, //       Usage Minimum (Weight Unit Milligram)
    0x29,
    0x5C, //       Usage Maximum (Weight Unit Pound)
    0xB1,
    0x00, //       Feature (Data,Array,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
    0xC0, //     End Collection
    0x15,
    0x81, //     Logical Minimum (-127)
    0x25,
    0x7F, //     Logical Maximum (127)
    0x09,
    0x41, //     Usage (Data Scaling)
    0xB1,
    0x02, //     Feature (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
    0x75,
    0x10, //     Report Size (16)
    0x15,
    0x00, //     Logical Minimum (0)
    0x27,
    0xFF,
    0xFF,
    0x00,
    0x00, //     Logical Maximum (65534)
    0x09,
    0x40, //     Usage (Data Weight)
    0xB1,
    0x03, //     Feature (Const,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
    0xC0, //   End Collection
    0x09,
    0x35, //   Usage (Scale Statistics Report)
    0xA1,
    0x02, //   Collection (Logical)
    0x85,
    0x06, //     Report ID (6)
    0x75,
    0x10, //     Report Size (16)
    0x95,
    0x02, //     Report Count (2)
    0x15,
    0x00, //     Logical Minimum (0)
    0x27,
    0xFF,
    0xFF,
    0x00,
    0x00, //     Logical Maximum (65534)
    0x09,
    0x60, //     Usage (Calibration Count)
    0x09,
    0x61, //     Usage (Re-Zero Count)
    0xB1,
    0x03, //     Feature (Const,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
    0xC0, //   End Collection
    0x09,
    0x30, //   Usage (Scale Attribute Report)
    0xA1,
    0x02, //   Collection (Logical)
    0x85,
    0x07, //     Report ID (7)
    0x75,
    0x08, //     Report Size (8)
    0x95,
    0x0F, //     Report Count (15)
    0x15,
    0x00, //     Logical Minimum (0)
    0x25,
    0x7F, //     Logical Maximum (127)
    0x09,
    0x40, //     Usage (Data Weight)
    0xB1,
    0x02, //     Feature (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
    0xC0, //   End Collection
    0xC0, // End Collection
};

// ---------- Structs and Enums ----------
// Scales RS232 Profile
struct scalesProfile_t
{
  char name[20];
  CRGB LEDColor;
  int baudRate;
  char requestStr[5];
  int requestInterval;
  int requestTimeout;
  char responseTermination[3];
  byte numResponseValues;
  byte responseWeightValueIndex;
  char responseFormatLbs[200];
  char responseFormatKgs[200];
  char responseValueMask[200];
  float minWeight; // lbs
  float maxWeight;
};

// HID Report
typedef struct
{
  uint8_t status = USAGE_STATUS_WEIGHT_STABLE;
  uint8_t unit;
  int8_t scaling;
  uint16_t weight;
} __packed dataReport_t;
dataReport_t dataReport;

// Unit Enum
enum unit_t
{
  lb,
  kg
};

// ---------- Configuration -----------
#define SOFTWARE_VERSION 1.0
#define HARDWARE_VERSION 1.0

// Status and Debug
#define ENABLE_STATUS_LED
#ifdef ENABLE_STATUS_LED
CRGB statusLED[1];    // FastLED Object
int statusLEDHue = 0; // Tracks hue for animations
#define STATUS_LED_PIN 16
#define STATUS_LED_BRIGHTNESS 20
#define STATUS_LED_FAST_BLINK_MS 150
#endif

#define ENABLE_DEBUG
#ifdef ENABLE_DEBUG
#define DEBUG_PORT Serial
#define DEBUG_BAUD 115200
// #define DEBUG_HID
// #define DEBUG_SCALES
#ifdef DEBUG_SCALES
// #define DEBUG_SCALES_SORT
// #define DEBUG_SCALES_SEARCH
// #define DEBUG_SCALES_RECEIVE
// #define DEBUG_SCALES_VERIFY
// #define DEBUG_SCALES_PARSE
#endif
#endif

// USB Configuration
#define HID_PRECISION_DIGITS 2 // Precision for HID reports
#define SCALE_DATA_REPORT_ID 3

// Scales Configuration
#define SCALES_PORT Serial1         // The serial port used by the RS232 hardware
#define SCALES_MAX_RESPONSE_SIZE 75 // The most characters a scale will send over serial
#define SCALES_NUM_PROFILES 2       // The number of scales profiles defined
scalesProfile_t scalesProfile[SCALES_NUM_PROFILES] = {
    {
        "Avery ZK830",                                                                 // Avery ZK830 Indicator
        CRGB::DarkGreen,                                                               // LED Color
        9600,                                                                          // Baud Rate
        "p",                                                                           // Request String
        1500,                                                                          // Request Interval
        5000,                                                                          // Request Timeout
        "\n\n",                                                                        // Response Termination
        3,                                                                             // Number of Values in Response
        0,                                                                             // Index of Weight Value in Response
        "GROSS WT:     0.00 lb\r\nCOUNT:            0\r\nPIECE WT: -------- lb\r\n\n", // Response Format in Pounds
        "GROSS WT:     0.00 kg\r\nCOUNT:            0\r\nPIECE WT: -------- kg\r\n\n", // Response Format in Kilograms
        "00000000001111111100000000000011111111111100000000000011111111000000",        // Response mask, zeros indicate data that never changes, and ones indicate variables
        -50.00,                                                                        // Minimum Weight
        100.00,                                                                        // Maximum Weight
    },
    {
        "Mettler Generic",       // Common Mettler Scales
        CRGB::DarkBlue,          // LED Color
        115200,                  // Baud Rate
        "S\r\n",                 // Request String
        1000,                    // Request Interval
        3000,                    // Request Timeout
        "\r\n",                  // Response Termination
        1,                       // Number of Values in Response
        0,                       // Index of Weight Value in Response
        "S S       0.00 lb\r\n", // Response Format in Pounds
        "S S       0.00 kg\r\n", // Response Format in Kilograms
        "0000111111111100000",   // Response mask, zeros indicate data that never changes, and ones indicate variables
        -50.00,                  // Minimum Weight
        100.00,                  // Maximum Weight
    }};

// ---------- Runtime Variables ----------
// HID Instance
Adafruit_USBD_HID usb_hid;

// Weight Formated for HID Response
int HIDWeight = 0;

// Connected to a scales
bool scalesConnected = false;
// Last scales response time
unsigned long lastScalesResponseTime = 0;

// Scales Profile in Use
byte activeScalesProfile = 0;

// ---------- Function Definitions ----------
void hid_report_callback(uint8_t report_id, hid_report_type_t report_type, uint8_t const *buffer, uint16_t bufsize);
void statusLEDInit();
void statusLEDUpdate();
void debugInit();
void HIDInit();
void HIDUpdate();
void scalesInit();
void scalesSearch();
void scalesPoll();
void scalesReceive();
void scalesParse(char *data);
void flagGoodReceive();
void scalesCalcWeight(float raw, unit_t unit);
bool verifyResponse(char *data, char *format, char *mask);
bool isNumeric(char c);

// Setup
// Inits and configures the system on boot
void setup()
{
// Init the status LED
#ifdef ENABLE_STATUS_LED
  statusLEDInit();
#endif

// Start the debugger
#ifdef ENABLE_DEBUG
  debugInit();
#endif

  // Start the HID program
  HIDInit();

  // Start the scales communications
  scalesInit();
}

void loop()
{
// Update the status LED
#ifdef ENABLE_STATUS_LED
  statusLEDUpdate();
#endif

  // Send an HID report
  HIDUpdate();

  // If the scales are not connected, search for them
  if (!scalesConnected)
  {
    scalesSearch();
  }

  // Check if it's time to poll the scales
  scalesPoll();

  // Check for incoming scales data
  scalesReceive();
}

#ifdef ENABLE_STATUS_LED
// Sets up the status LED
void statusLEDInit()
{
  FastLED.addLeds<WS2812, STATUS_LED_PIN, RGB>(statusLED, 1);
  FastLED.setBrightness(STATUS_LED_BRIGHTNESS);
}

// Updates the status LED
void statusLEDUpdate()
{
  // Blink a fast red error code if the device is ever not connected over HID
  if (!TinyUSBDevice.mounted())
  {
    EVERY_N_MILLISECONDS(STATUS_LED_FAST_BLINK_MS)
    {
      if (statusLED[0] == CRGB::Red)
      {
        statusLED[0] = CRGB::Black;
      }
      else
      {
        statusLED[0] = CRGB::Red;
      }
      FastLED.show();
    }
  }
  else if (!scalesConnected)
  {
    // If the connected over USB, but not to the serial device do a fast rainbow animation
    EVERY_N_MILLISECONDS(50)
    {
      statusLEDHue += 40; // Increment the hue
      statusLED[0].setHue(statusLEDHue);
      FastLED.show();
    }
  }
  else if (scalesConnected)
  {
    if (statusLED[0] != scalesProfile[activeScalesProfile].LEDColor)
    {
      statusLED[0] = scalesProfile[activeScalesProfile].LEDColor;
      FastLED.show();
    }
  }
}
#endif

#ifdef ENABLE_DEBUG
// Initializes the debug serial link
void debugInit()
{
  DEBUG_PORT.begin(DEBUG_BAUD);
  delay(2000); // Wait for the serial connection to be established
  DEBUG_PORT.println("Miller Industrial Services");
  DEBUG_PORT.println("RS-232 to USB HID Scales Adapter");
  DEBUG_PORT.println("Hardware Version: " + String(HARDWARE_VERSION) + ", Software Version: " + String(SOFTWARE_VERSION));
  DEBUG_PORT.println("---------------------------------------------------");
}
#endif

// Initializes the HID Components
void HIDInit()
{
#ifdef ENABLE_DEBUG
  DEBUG_PORT.print("Starting HID... ");
#endif

  // Configure HID
  usb_hid.setBootProtocol(HID_ITF_PROTOCOL_NONE);
  usb_hid.setPollInterval(10);
  usb_hid.setReportDescriptor(descriptor, sizeof(descriptor));
  usb_hid.setStringDescriptor("MIS Scales Adapter");

  // Start the HID and report failure
  if (!usb_hid.begin())
  {
#ifdef ENABLE_DEBUG
    DEBUG_PORT.println("HID Failed to Start");
#endif
  }

  // Wait for HID to connect then check the mounting status
  delay(500);
  if (TinyUSBDevice.mounted())
  {
#ifdef ENABLE_DEBUG
    DEBUG_PORT.println("HID Connected");
#endif
  }
  else
  {
#ifdef ENABLE_DEBUG
    DEBUG_PORT.println("HID Failed to Mount");
#endif
  }
}

// Sends a report to the USB Host
void HIDUpdate()
{
  // If HID is busy (e.g. sending previous report, skip the update)
  if (!usb_hid.ready())
  {
#ifdef DEBUG_HID
    DEBUG_PORT.println("HID Report Failed to Send - HID Busy");
#endif
    return;
  }

#ifdef DEBUG_HID
  DEBUG_PORT.println("Sending HID Report");
#endif

  // Create a data report
  dataReport.unit = USAGE_WEIGHT_UNIT_POUND;
  dataReport.scaling = -HID_PRECISION_DIGITS; // -2 makes the scale expect 1.23 to be encoded as 123
  dataReport.weight = HIDWeight;

  // Send the report
  usb_hid.sendReport(SCALE_DATA_REPORT_ID, (uint8_t *)&dataReport, sizeof(dataReport));

#ifdef DEBUG_HID
  DEBUG_PORT.println("HID Report Sent");
#endif
}

// Starts the serial link with the scales
void scalesInit()
{
#ifdef DEBUG_SCALES
  DEBUG_PORT.print("Starting RS232... ");
#endif

  // Start the Serial Port
  SCALES_PORT.begin(scalesProfile[activeScalesProfile].baudRate);

#ifdef ENABLE_DEBUG
  DEBUG_PORT.println("RS232 Started");
#endif
}

// Searches for the scales by cycling through profiles from fastest baud to slowest and polling each one
void scalesSearch()
{
  // Sort the scales profiles by baud rate
  static scalesProfile_t sortedScaleProfiles[SCALES_NUM_PROFILES] = {};
  if (sortedScaleProfiles[0].baudRate == 0) // The array is uninitialized
  {
#ifdef DEBUG_SCALES_SORT
    DEBUG_PORT.println("Sorting Scales Profiles by Baud Rate");
#endif

    // Create a sorted list of scales profiles based on baud rate
    for (byte i = 0; i < SCALES_NUM_PROFILES; i++)
    {
      sortedScaleProfiles[i] = scalesProfile[i];
    }
    // Simple Bubble Sort
    for (byte i = 0; i < SCALES_NUM_PROFILES - 1; i++)
    {
      for (byte j = 0; j < SCALES_NUM_PROFILES - i - 1; j++)
      {
        if (sortedScaleProfiles[j].baudRate < sortedScaleProfiles[j + 1].baudRate)
        {
#ifdef DEBUG_SCALES_SORT
          DEBUG_PORT.println("Baud Rate " + String(sortedScaleProfiles[j].baudRate) + "(" + String(sortedScaleProfiles[j].name) + ") < " + String(sortedScaleProfiles[j + 1].baudRate) + "(" + String(sortedScaleProfiles[j + 1].name) + "), swapping");
#endif
          scalesProfile_t temp = sortedScaleProfiles[j];
          sortedScaleProfiles[j] = sortedScaleProfiles[j + 1];
          sortedScaleProfiles[j + 1] = temp;
        }
        else
        {
#ifdef DEBUG_SCALES_SORT
          DEBUG_PORT.println("Baud Rate " + String(sortedScaleProfiles[j].baudRate) + "(" + String(sortedScaleProfiles[j].name) + ") > " + String(sortedScaleProfiles[j + 1].baudRate) + "(" + String(sortedScaleProfiles[j + 1].name) + "), ignoring");
#endif
        }
      }
    }

#ifdef DEBUG_SCALES_SORT
    DEBUG_PORT.println("Scales Profiles Sorted:");
    for (byte i = 0; i < SCALES_NUM_PROFILES; i++)
    {
      DEBUG_PORT.println("  " + String(sortedScaleProfiles[i].name) + " - " + String(sortedScaleProfiles[i].baudRate) + " Baud");
    }
#endif
  }

  // Check for a timeout on the current profile
  if (millis() - lastScalesResponseTime >= scalesProfile[activeScalesProfile].requestTimeout)
  {

#ifdef DEBUG_SCALES_SEARCH
    DEBUG_PORT.println("No response from " + String(scalesProfile[activeScalesProfile].name) + " in " + String(scalesProfile[activeScalesProfile].requestTimeout) + "ms, switching profiles");
#endif

    // Move to the next profile
    activeScalesProfile++;
    if (activeScalesProfile >= SCALES_NUM_PROFILES)
    {
      activeScalesProfile = 0;
    }

    // Update the timeout
    lastScalesResponseTime = millis();

#ifdef DEBUG_SCALES_SEARCH
    DEBUG_PORT.println("Switched to " + String(scalesProfile[activeScalesProfile].name) + " - " + String(scalesProfile[activeScalesProfile].baudRate) + " Baud");
#endif

    // Update the serial port to the new profile
    SCALES_PORT.end();
    SCALES_PORT.begin(scalesProfile[activeScalesProfile].baudRate);
    delay(1000); // Wait for the serial port to initialize

  }
}

// Polls the scale after a certain amount of time
void scalesPoll()
{
  static unsigned long scalesPollTime = 0;
  if (millis() - scalesPollTime >= scalesProfile[activeScalesProfile].requestInterval)
  {
#ifdef DEBUG_SCALES
    DEBUG_PORT.println("Requesting Data from Scales - Sent \"" + String(scalesProfile[activeScalesProfile].requestStr) + "\"");
#endif
    // Request Weight Information from the Scales
    SCALES_PORT.println(scalesProfile[activeScalesProfile].requestStr);
    scalesPollTime = millis();
  }
}

// Watches the serial line and receives data
void scalesReceive()
{
  // Watch for a timeout
  if (millis() - lastScalesResponseTime >= scalesProfile[activeScalesProfile].requestTimeout)
  {
    scalesConnected = false;
  }

  static char receivedData[SCALES_MAX_RESPONSE_SIZE + 1] = "\0"; // Add 1 for null termination

  static byte bufferIndex = 0;

  // Receive new data
  if (SCALES_PORT.available() > 0)
  {
#ifdef DEBUG_SCALES_RECEIVE
    DEBUG_PORT.println("Receiving '" + String(char(SCALES_PORT.peek())) + "'");
#endif
    receivedData[bufferIndex] = SCALES_PORT.read();
    receivedData[bufferIndex + 1] = '\0';           // Terminate the string
    if (bufferIndex < SCALES_MAX_RESPONSE_SIZE - 1) // Check for buffer overflows
    {
      bufferIndex++;
    }
    else
    {
#ifdef ENABLE_DEBUG
      DEBUG_PORT.println("Scales Receive Buffer Overflow! - Size: " + String(bufferIndex + 1));
#endif
      bufferIndex = 0; // Start overwriting the buffer
    }

    // Watch for the scales response termination
    if (strstr(receivedData, scalesProfile[activeScalesProfile].responseTermination) != NULL)
    {
      bufferIndex = 0; // Reset the reading procedure
#ifdef DEBUG_SCALES_RECEIVE
      DEBUG_PORT.println("Termination Found: " + String(scalesProfile[activeScalesProfile].responseTermination));
#endif
#ifdef DEBUG_SCALES
      DEBUG_PORT.println("Data Received:");
      DEBUG_PORT.println(receivedData);
#endif

      // Parse the recieved data
      scalesParse(receivedData);
    }
  }
}

// Verifies and parses the data received from the scales
void scalesParse(char *data)
{
  // Compare the response to known formats, and identify the unit
  unit_t responseUnit;
  char formatLB[SCALES_MAX_RESPONSE_SIZE + 1];
  char formatKG[SCALES_MAX_RESPONSE_SIZE + 1];
  char mask[SCALES_MAX_RESPONSE_SIZE + 1];
  strcpy(formatLB, scalesProfile[activeScalesProfile].responseFormatLbs);
  strcpy(formatKG, scalesProfile[activeScalesProfile].responseFormatKgs);
  strcpy(mask, scalesProfile[activeScalesProfile].responseValueMask);

  if (verifyResponse(data, formatLB, mask))
  {
    // We've received a good response in lb format
    responseUnit = lb;
    flagGoodReceive();

#ifdef DEBUG_SCALES
    DEBUG_PORT.println("Good Response Received - Pounds");
#endif
  }
  else if (verifyResponse(data, formatKG, mask))
  {
    // Good response in kg format
    responseUnit = kg;
    flagGoodReceive();

#ifdef DEBUG_SCALES
    DEBUG_PORT.println("Good Response Received - Kilograms");
#endif
  }
  else
  {
    // Bad Response
    scalesConnected = false;

#ifdef ENABLE_DEBUG
    DEBUG_PORT.println("Scales response isn't in a known format!");
    return;
#endif
  }

  // Start parsing the data
  float parsedNumbers[scalesProfile[activeScalesProfile].numResponseValues + 1] = {0};

#ifdef DEBUG_SCALES_PARSE
  DEBUG_PORT.println("Parsing Data");
#endif

  byte numberIndex = 0; // Counts the different values we pull from the data

  // Loop through the data, pulling out numbers
  for (int i = 0; data[i] != '\0' || i >= SCALES_MAX_RESPONSE_SIZE - 1; i++)
  {

#ifdef DEBUG_SCALES_PARSE
    DEBUG_PORT.println("Parsing: " + String(char(data[i])));
#endif

    if (isNumeric(data[i])) // Starting a number
    {
#ifdef DEBUG_SCALES_PARSE
      DEBUG_PORT.println("Starting Number");
#endif
      parsedNumbers[numberIndex] = strtof(data + i, NULL);
#ifdef DEBUG_SCALES_PARSE
      DEBUG_PORT.println("Number Parsed: " + String(parsedNumbers[numberIndex]));
#endif
      // Loop through the rest of the number's characters
      while (isNumeric(data[i + 1]))
      {
        i++;
#ifdef DEBUG_SCALES_PARSE
        DEBUG_PORT.println("Ignoring: " + String(char(data[i])));
#endif
      }

      // Get ready to receive the next value
      if (numberIndex < scalesProfile[activeScalesProfile].numResponseValues)
      {
        numberIndex++;
      }
      else // We're gonna overflow the array
      {
#ifdef ENABLE_DEBUG
        DEBUG_PORT.println("Too Many Scales Values Found! - " + String(numberIndex + 1)); // Add 1 to account for zero-based array
#endif
        break;
      }
    }
  } // Finished looping through data

#ifdef DEBUG_SCALES
  DEBUG_PORT.println("Weight Parsed: " + String(parsedNumbers[scalesProfile[activeScalesProfile].responseWeightValueIndex]));
#endif

  // Convert and save the weight
  scalesCalcWeight(parsedNumbers[scalesProfile[activeScalesProfile].responseWeightValueIndex], responseUnit);
}

// Converts the weight from the scales to what the computer wants
void scalesCalcWeight(float raw, unit_t unit)
{
  // Convert the weight to lbs if necessary
  float weightLB = 0; // The weight, converted to lbs
  if (unit == lb)     // the value is in pounds
  {
    weightLB = raw;
  }
  else if (unit == kg) // kilograms
  {
    weightLB = raw * KG_TO_LB_CONVERSION_FACTOR; // Convert to lbs

#ifdef DEBUG_SCALES
    DEBUG_PORT.println("Weight Converted to Lbs: " + String(weightLB));
#endif
  }

  // Check if the weight is within scales limits
  if (weightLB >= scalesProfile[activeScalesProfile].minWeight && weightLB <= scalesProfile[activeScalesProfile].maxWeight)
  {
#ifdef DEBUG_SCALES
    DEBUG_PORT.println("Weight Valid");
#endif

    // Calculate Weight
    float convertedWeight = 0.00;                               // We need a float variable for our float math below to avoid rounding issues
    convertedWeight = weightLB * pow(10, HID_PRECISION_DIGITS); // Convert float to integer
    HIDWeight = round(convertedWeight);                         // Now our math is done, so we can convert it to an integer

#ifdef DEBUG_SCALES
    DEBUG_PORT.println("Converted Weight: " + String(HIDWeight));
#endif
  }
  else
  {
#ifdef DEBUG_SCALES
    DEBUG_PORT.println("Weight Invalid!");
#endif
  }
}

// Loops through a serial response, comparing it to a good response and using a mask to eliminate variables
bool verifyResponse(char *data, char *format, char *mask)
{
#ifdef DEBUG_SCALES_VERIFY
  DEBUG_PORT.println("Verifying Response");
#endif

  for (int i = 0; data[i] != '\0' || i >= SCALES_MAX_RESPONSE_SIZE - 1; i++)
  {

    // Only verify the response if it's not a variable
    if (mask[i] == '0')
    {
#ifdef DEBUG_SCALES_VERIFY
      DEBUG_PORT.print(String(char(data[i])));
#endif

      if (data[i] != format[i]) // There's a discrepency
      {
#ifdef DEBUG_SCALES_VERIFY
        DEBUG_PORT.print(" != ");
        DEBUG_PORT.println(String(char(format[i])));
        DEBUG_PORT.println("Verification Failed!");
#endif
        return false;
      }
      else
      {
#ifdef DEBUG_SCALES_VERIFY
        DEBUG_PORT.print(" = ");
#endif
      }

#ifdef DEBUG_SCALES_VERIFY
      DEBUG_PORT.println(String(char(format[i])));
#endif
    }
  }

// If we reached this point, there are no discrepencies
#ifdef DEBUG_SCALES_VERIFY
  DEBUG_PORT.println("Verification Succeeded");
#endif
  return true;
}

// Flags that a good response was received from the scales
void flagGoodReceive()
{
  scalesConnected = true;
  lastScalesResponseTime = millis();
}

// Returns true if the character is a number or decimal point.
bool isNumeric(char c)
{
  switch (c)
  {
  case '0':
  case '1':
  case '2':
  case '3':
  case '4':
  case '5':
  case '6':
  case '7':
  case '8':
  case '9':
  case '.':
    return true;
    break;
  default:
    return false;
    break;
  }
}

/*
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
}*/