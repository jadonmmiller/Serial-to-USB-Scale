# Serial-to-USB-Scale
This is a STM32F1-based Serial to USBHID Scales Converter.

It started when I bought a shipping scales with a serial connection a few months ago. I wanted to connect that to my shipping software, which expected a scales running the USB-HID communications protocol. That's when this adapter came to be!

I use the Bluepill to send a character to the scales, requesting data. The scales then responds with a text string containing the weight and some other details. The adapter parses the string, then sends the data onto the computer.

The wiring simply consists of a MAX232-based RS-232 DB9 to UART adapter running into the Serial2 port of the BluePill, TX->A2 and RX->A3. Power and ground of the adapter go to the 3.3V and GND pins of the Bluepill. I've also added provisions for a serial connection to a computer via pins A9 and A10 on the Bluepill. This is helpful for debugging the adapter (the debugging detail level can be configured with #define statements at the top of the code).

I'm currently using it with an Avery Weight-Tronix ZK830 scales and the Veeqo shipping software. However, other scales could be added fairly easily if you're familiar with their communications protocols.

Feel free to use this code for your own use, including tinkering with it and reposting it. Unfortunately though, I can provide very little ongoing support due to time constraints.
