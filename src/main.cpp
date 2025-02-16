/*********************************************************************
This code communicates with an industrial scales over RS232 via a
MAX3232 chip. It then emulates a USB HID scales for integration with
shipping software such as Veeqo.

Created by Jadon Miller, Miller Industrial Services - 2/15/2025
License: The Unlicense
*********************************************************************/

#include <Arduino.h>
#include "Adafruit_TinyUSB.h"

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

// ---------- Structs and Enums ----------
// Scales RS232 Profile
struct scalesProfile_t
{
  const char name[11];
  const int baudRate;
  const char requestStr[3];
  const int requestInterval;
  const char responseTermination[3];
  const byte numResponseValues;
  const byte responseWeightValueIndex;
  const char responseFormatLbs[200];
  const char responseFormatKgs[200];
  const char responseValueMask[200];
  const float minWeight; // lbs
  const float maxWeight;
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
#include <FastLED.h>
CRGB statusLED[1];
#define STATUS_LED_PIN 16
#define STATUS_LED_BRIGHTNESS 150
#endif

#define ENABLE_DEBUG
#ifdef ENABLE_DEBUG
#define DEBUG_PORT Serial
#define DEBUG_BAUD 9600
#define DEBUG_HID
#define DEBUG_SCALES
#ifdef DEBUG_SCALES
#define DEBUG_SCALES_RECEIVE
#define DEBUG_SCALES_VERIFY
#define DEBUG_SCALES_PARSE
#endif
#endif

// USB Configuration
#define HID_PRECISION_DIGITS 2 // Precision for HID reports
#define SCALE_DATA_REPORT_ID 3

// Scales Configuration
#define SCALES_MAX_RESPONSE_SIZE 75 // The most characters a scale will send over serial
scalesProfile_t scalesProfile[1] = {
    {
        "Avery",                                                                       // Avery ZK830 Indicator
        9600,                                                                          // Baud Rate
        "p",                                                                           // Request String
        1500,                                                                          // Request Interval
        "\n\n",                                                                        // Response Termination
        3,                                                                             // Number of Values in Response
        0,                                                                             // Index of Weight Value in Response
        "GROSS WT:     0.00 lb\r\nCOUNT:            0\r\nPIECE WT: -------- lb\r\n\n", // Response Format in Pounds
        "GROSS WT:     0.00 kg\r\nCOUNT:            0\r\nPIECE WT: -------- kg\r\n\n", // Response Format in Kilograms
        // Response mask, zeros indicate data that nevver changes, and ones indicate variables
        "00000000001111111100000000000011111111111100000000000011111111000000",
        100.00, // Maximum Weight
        -50.00  // Minimum Weight
    }};

// ---------- Runtime Variables ----------
// HID Instance
Adafruit_USBD_HID usb_hid;

// Weight Formated for HID Response
int HIDWeight = 0;

// ---------- Function Definitions ----------
void hid_report_callback(uint8_t report_id, hid_report_type_t report_type, uint8_t const *buffer, uint16_t bufsize);
void statusLEDInit();
void statusLEDUpdate();
void debugInit();
void HIDInit();
void HIDUpdate();
void scalesInit();
void scalesPoll();
void scalesReceive();
void scalesParse(char *data);
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
// Update the heartbeat
#ifdef ENABLE_HEARTBEAT
  heartbeatUpdate();
#endif

  // Send an HID report
  HIDUpdate();

  // Check if it's time to poll the scales
  //scalesPoll();

  // Check for incoming scales data
  //scalesReceive();
}

#ifdef ENABLE_STATUS_LED
// Sets up the status LED
void statusLEDInit()
{
  FastLED.addLeds<WS2812, STATUS_LED_PIN, RGB>(statusLED, 1);
  FastLED.setBrightness(STATUS_LED_BRIGHTNESS);
  statusLED[0] = CRGB::Red;
  FastLED.show();
}

// Toggles the heartbeat LED
void statusLEDUpdate()
{
}
#endif

#ifdef ENABLE_DEBUG
// Initializes the debug serial link
void debugInit()
{
  Serial.begin(DEBUG_BAUD);
  Serial.println("RS-232 to USB Scales Adapter");
  Serial.println("Hardware Version: " + String(HARDWARE_VERSION) + ", Software Version: " + String(SOFTWARE_VERSION));
  Serial.println("---------------------------------------------------");
}
#endif

// Initializes the HID Components
void HIDInit()
{
#ifdef DEBUG_HID
  Serial.print("Starting HID");
#endif

  usb_hid.setBootProtocol(HID_ITF_PROTOCOL_NONE);
  usb_hid.setPollInterval(2);
  usb_hid.setReportDescriptor(descriptor, sizeof(descriptor));
  usb_hid.setStringDescriptor("Scales Adapter");

  usb_hid.begin();

#ifdef ENABLE_DEBUG
  Serial.println();
  Serial.println("HID Started");
#endif
}

// Sends a report to the USB Host
void HIDUpdate()
{
#ifdef DEBUG_HID
  Serial.println("Sending HID Report");
#endif

  // Create a data report
  dataReport.unit = USAGE_WEIGHT_UNIT_POUND;
  dataReport.scaling = -HID_PRECISION_DIGITS; // -2 makes the scale expect 1.23 to be encoded as 123
  dataReport.weight = HIDWeight;

  // Send the report
  // dataReporter.sendReport();

#ifdef DEBUG_HID
  Serial.println("HID Report Sent");
#endif
}

// Starts the serial link with the scales
void scalesInit()
{
#ifdef DEBUG_SCALES
  Serial.println("Starting Scales");
#endif

  // Serial2.begin(SCALES_BAUD);

#ifdef ENABLE_DEBUG
  Serial.println("Scales Started");
#endif
}
/*
// Polls the scale after a certain amount of time
void scalesPoll()
{
  static unsigned long scalesPollTime = 0;
  if (millis() - scalesPollTime >= scalesProfile[0].requestInterval)
  {
#ifdef DEBUG_SCALES
    Serial.println("Requesting Data from Scales - Sent \"" + String(scalesProfile[0].requestStr) + "\"");
#endif
    // Request Weight Information from the Scales
    // Serial2.println(SCALES_POLL_STRING);
    scalesPollTime = millis();
  }
}

// Watches the serial line and receives data
void scalesReceive()
{
  static char receivedData[SCALES_MAX_RESPONSE_SIZE + 1] = "\0"; // Add 1 for null termination

  static byte bufferIndex = 0;
  /*
    // Receive new data
    if (Serial2.available() > 0)
    {
  #ifdef DEBUG_SCALES_RECEIVE
      Serial.println("Receiving '" + String(char(Serial2.peek())) + "'");
  #endif
      receivedData[bufferIndex] = Serial2.read();
      receivedData[bufferIndex + 1] = '\0';       // Terminate the string
      if (bufferIndex < SCALES_RESPONSE_SIZE - 1) // Check for buffer overflows
      {
        bufferIndex++;
      }
      else
      {
  #ifdef ENABLE_DEBUG
        Serial.println("Scales Receive Buffer Overflow! - Size: " + String(bufferIndex + 1));
  #endif
        bufferIndex = 0; // Start overwriting the buffer
      }

      // Watch for the transmit termination
      if (strstr(receivedData, SCALES_TERMINATION) != NULL)
      {
        bufferIndex = 0; // Reset the reading procedure
  #ifdef DEBUG_SCALES_RECEIVE
        Serial.println("Termination Found: " + String(SCALES_TERMINATION));
  #endif
  #ifdef DEBUG_SCALES
        Serial.println("Data Received:");
        Serial.println(receivedData);
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
  char formatLB[SCALES_MAX_RESPONSE_SIZE + 1] = 
  scalesProfile[0].responseFormatLbs;
  char formatKG[SCALES_MAX_RESPONSE_SIZE + 1] = scalesProfile[0].responseFormatKgs[0];
  char mask[SCALES_MAX_RESPONSE_SIZE + 1] = scalesProfile[0].responseValueMask[0];

  if (verifyResponse(data, formatLB, mask))
  {
    // We've received a good response in lb format
    responseUnit = lb;

#ifdef DEBUG_SCALES
    Serial.println("Good Response Received - Pounds");
#endif
  }
  else if (verifyResponse(data, formatKG, mask))
  {
    // Good response in kg format
    responseUnit = kg;

#ifdef DEBUG_SCALES
    Serial.println("Good Response Received - Kilograms");
#endif
  }
  else
  {
    // Bad Response
#ifdef ENABLE_DEBUG
    Serial.println("Scales response isn't in a known format!");
    return;
#endif
  }

  // Start parsing the data
  static float parsedNumbers[scalesProfile[0].numResponseValues + 1] = {0};

#ifdef DEBUG_SCALES_PARSE
  Serial.println("Parsing Data");
#endif

  byte numberIndex = 0; // Counts the different values we pull from the data

  // Loop through the data, pulling out numbers
  for (int i = 0; data[i] != '\0' || i >= SCALES_MAX_RESPONSE_SIZE - 1; i++)
  {

#ifdef DEBUG_SCALES_PARSE
    Serial.println("Parsing: " + String(char(data[i])));
#endif

    if (isNumeric(data[i])) // Starting a number
    {
#ifdef DEBUG_SCALES_PARSE
      Serial.println("Starting Number");
#endif
      parsedNumbers[numberIndex] = strtof(data + i, NULL);
#ifdef DEBUG_SCALES_PARSE
      Serial.println("Number Parsed: " + String(parsedNumbers[numberIndex]));
#endif
      // Loop through the rest of the number's characters
      while (isNumeric(data[i + 1]))
      {
        i++;
#ifdef DEBUG_SCALES_PARSE
        Serial.println("Ignoring: " + String(char(data[i])));
#endif
      }

      // Get ready to receive the next value
      if (numberIndex < scalesProfile[0].numResponseValues)
      {
        numberIndex++;
      }
      else // We're gonna overflow the array
      {
#ifdef ENABLE_DEBUG
        Serial.println("Too Many Scales Values Found! - " + String(numberIndex + 1)); // Add 1 to account for zero-based array
#endif
        break;
      }
    }
  } // Finished looping through data

#ifdef DEBUG_SCALES
  Serial.println("Weight Parsed: " + String(parsedNumbers[scalesProfile[0].responseWeightValueIndex]));
#endif

  // Convert and save the weight
  scalesCalcWeight(parsedNumbers[scalesProfile[0].responseWeightValueIndex], responseUnit);
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
    Serial.println("Weight Converted to Lbs: " + String(weightLB));
#endif
  }

  // Check if the weight is within scales limits
  if (weightLB >= scalesProfile[0].minWeight && weightLB <= scalesProfile[0].maxWeight)
  {
#ifdef DEBUG_SCALES
    Serial.println("Weight Valid");
#endif

    // Calculate Weight
    float convertedWeight = 0.00;                               // We need a float variable for our float math below to avoid rounding issues
    convertedWeight = weightLB * pow(10, HID_PRECISION_DIGITS); // Convert float to integer
    HIDWeight = round(convertedWeight);                         // Now our math is done, so we can convert it to an integer

#ifdef DEBUG_SCALES
    Serial.println("Converted Weight: " + String(HIDWeight));
#endif
  }
  else
  {
#ifdef DEBUG_SCALES
    Serial.println("Weight Invalid!");
#endif
  }
}

// Loops through a serial response, comparing it to a good response and using a mask to eliminate variables
bool verifyResponse(char *data, char *format, char *mask)
{
#ifdef DEBUG_SCALES_VERIFY
  Serial.println("Verifying Response");
#endif

  for (int i = 0; data[i] != '\0' || i >= SCALES_MAX_RESPONSE_SIZE - 1; i++)
  {

    // Only verify the response if it's not a variable
    if (mask[i] == '0')
    {
#ifdef DEBUG_SCALES_VERIFY
      Serial.print(String(char(data[i])));
#endif

      if (data[i] != format[i]) // There's a discrepency
      {
#ifdef DEBUG_SCALES_VERIFY
        Serial.print(" != ");
        Serial.println(String(char(format[i])));
        Serial.println("Verification Failed!");
#endif
        return false;
      }
      else
      {
#ifdef DEBUG_SCALES_VERIFY
        Serial.print(" = ");
#endif
      }

#ifdef DEBUG_SCALES_VERIFY
      Serial.println(String(char(format[i])));
#endif
    }
  }

// If we reached this point, there are no discrepencies
#ifdef DEBUG_SCALES_VERIFY
  Serial.println("Verification Succeeded");
#endif
  return true;
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