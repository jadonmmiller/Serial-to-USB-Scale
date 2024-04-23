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

// Configuration Settings
#define SOFTWARE_VERSION 0.3
#define HARDWARE_VERSION 0.2

#define ENABLE_DEBUG
#ifdef ENABLE_DEBUG
#define DEBUG_BAUD 9600
#define HEARTBEAT_PIN LED_BUILTIN
#define HEARTBEAT_INTERVAL 250
// #define DEBUG_HID
#define DEBUG_SCALES
#endif

#define HID_PRECISION_DIGITS 2

#define SCALES_BRAND_AVERY
#ifdef SCALES_BRAND_AVERY
#define SCALES_BAUD 115200
#define SCALES_POLL_INTERVAL 1500
#define SCALES_POLL_STRING "p"
#define SCALES_RESPONSE_SIZE 75
#define SCALES_NUMBER_OF_VALUES 3
#define SCALES_WEIGHT_NUMBER 0
#define SCALES_TERMINATION "\n\n"
#define SCALES_RESPONSE_FORMAT_LB "GR WT:     0.12 lb\r\nCOUNT:            0\n\rPIECE WT: -------- lb\r\n\n"
#define SCALES_MAX_WEIGHT 100.00
#define SCALES_MIN_WEIGHT -100.00
#endif

// HID Variables
typedef struct
{
  uint8_t reportID = 3;
  uint8_t status = USAGE_STATUS_WEIGHT_STABLE;
  uint8_t unit;
  int8_t scaling;
  uint16_t weight;

} __packed dataReport_t;
dataReport_t dataReport;
USBHID HID;
HIDReportDescriptor reportDescriptor = {descriptor, sizeof(descriptor)};
HIDReporter dataReporter(HID, &reportDescriptor, (uint8_t *)&dataReport, sizeof(dataReport), dataReport.reportID);

// Scales Variables
int scalesWeight = 0;

// Function Definitions
void debugInit();
void debugUpdate();
void HIDInit();
void HIDUpdate();
void scalesInit();
void scalesPoll();
void scalesReceive();
void scalesParse(char *data);
void scalesCalcWeight(float raw);
bool isNumeric(char c);

void setup()
{
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
// Update the debugger
#ifdef ENABLE_DEBUG
  debugUpdate();
#endif

  // Send an HID report
  HIDUpdate();

  // Check if it's time to poll the scales
  scalesPoll();

  // Check for incoming scales data
  scalesReceive();
}

#ifdef ENABLE_DEBUG
// Initializes the debug serial link and heartbeat LED
void debugInit()
{
  // Init the heartbeat LED
  pinMode(HEARTBEAT_PIN, OUTPUT);
  digitalWrite(HEARTBEAT_PIN, HIGH);

  // Init the debug link
  Serial.begin(DEBUG_BAUD);
  Serial.println("RS-232 to USB Scales Adapter");
  Serial.println("Hardware Version: " + String(HARDWARE_VERSION) + ", Software Version: " + String(SOFTWARE_VERSION));
  Serial.println("---------------------------------------------------");
}

// Toggles the heartbeat LED
void debugUpdate()
{
  static unsigned long heartbeatToggleTime = 0;
  if (millis() - heartbeatToggleTime >= HEARTBEAT_INTERVAL)
  {
    digitalWrite(HEARTBEAT_PIN, !digitalRead(HEARTBEAT_PIN));
    heartbeatToggleTime = millis();
  }
}
#endif

// Initializes the HID Components
void HIDInit()
{
#ifdef DEBUG_HID
  Serial.print("Starting HID");
#endif

  HID.begin(descriptor, sizeof(descriptor));

  while (!USBComposite)
  {
#ifdef DEBUG_HID
    Serial.print(".");
    delay(250);
#endif
  }

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
  dataReport.weight = scalesWeight;

  // Send the report
  dataReporter.sendReport();

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

  Serial2.begin(SCALES_BAUD);

#ifdef ENABLE_DEBUG
  Serial.println("Scales Started");
#endif
}

// Polls the scale after a certain amount of time
void scalesPoll()
{
  static unsigned long scalesPollTime = 0;
  if (millis() - scalesPollTime >= SCALES_POLL_INTERVAL)
  {
#ifdef DEBUG_SCALES
    Serial.println("Requesting Data from Scales - Sent \"" + String(SCALES_POLL_STRING) + "\"");
#endif
    // Request Weight Information from the Scales
    Serial2.println(SCALES_POLL_STRING);
    scalesPollTime = millis();
  }
}

// Watches the serial line and receives data
void scalesReceive()
{
  static char receivedData[SCALES_RESPONSE_SIZE + 1] = "\0"; // Add 1 for null termination

  static byte bufferIndex = 0;

  // Receive new data
  if (Serial2.available() > 0)
  {
#ifdef DEBUG_SCALES
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
#ifdef DEBUG_SCALES
      Serial.println("Termination Found: " + String(SCALES_TERMINATION));
      Serial.println("Data Received:");
      Serial.println(receivedData);
#endif

      // Parse the recieved data
      scalesParse(receivedData);
    }
  }
}

// Parses the data received from the scales
void scalesParse(char *data)
{
  static float parsedNumbers[SCALES_NUMBER_OF_VALUES + 1] = {0};

#ifdef DEBUG_SCALES
  Serial.println("Parsing Data");
#endif

  byte numberIndex = 0; // Counts the different values we pull from the data

  // Loop through the data, pulling out numbers
  for (int i = 0; data[i] != '\0' || i >= SCALES_RESPONSE_SIZE - 1; i++)
  {

#ifdef DEBUG_SCALES
    Serial.println("Parsing: " + String(char(data[i])));
#endif

    if (isNumeric(data[i])) // Starting a number
    {
#ifdef DEBUG_SCALES
      Serial.println("Starting Number");
#endif
      parsedNumbers[numberIndex] = strtof(data + i, NULL);
#ifdef DEBUG_SCALES
      Serial.println("Number Parsed: " + String(parsedNumbers[numberIndex]));
#endif
      // Loop through the rest of the number's characters
      while (isNumeric(data[i + 1]))
      {
        i++;
#ifdef DEBUG_SCALES
        Serial.println("Ignoring: " + String(char(data[i])));
#endif
      }

      // Get ready to receive the next value
      if (numberIndex < SCALES_NUMBER_OF_VALUES)
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
  Serial.println("Weight Parsed: " + String(parsedNumbers[SCALES_WEIGHT_NUMBER]));
#endif

  // Convert and save the weight
  scalesCalcWeight(parsedNumbers[SCALES_WEIGHT_NUMBER]);
}

// Converts the weight from the scales to what the computer wants
void scalesCalcWeight(float raw)
{
  if (raw >= SCALES_MIN_WEIGHT && raw <= SCALES_MAX_WEIGHT) // Weight is within scales limits
  {
#ifdef DEBUG_SCALES
    Serial.println("Weight Valid");
#endif

    // Calculate Weight
    float convertedWeight = 0.00;                          // We need a float variable for our float math below to avoid rounding issues
    convertedWeight = raw * pow(10, HID_PRECISION_DIGITS); // Convert float to integer
    scalesWeight = convertedWeight;                        // Now our math is done, so we can convert it to an integer

#ifdef DEBUG_SCALES
    Serial.println("Converted Weight: " + String(scalesWeight));
#endif
  }
  else
  {
#ifdef DEBUG_SCALES
    Serial.println("Weight Invalid!");
#endif
  }
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