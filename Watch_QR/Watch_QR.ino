#define  RED            0x03
#define WHITE           0xFF
#include <SPI.h>
#include <STBLE.h>
#include <TinyScreen.h>
#include <SPI.h>
#include <Wire.h>
#include "qrcode.h"

/*
Note:
- This is a limited running prototype that was designed to use Bluetooth instead of Wifi
- Development for usage of Wifi to send QR code to watch requires an additional hardware component 
(Wifi shield to connect TinyCircuit directly to a wifi network / Prototyping shield to connect the TinyCircuit to the 
breadboard used in the secondary Arduino board used in this project, which then connects to a Wifi Network)

To run:
1. Download QRCode library by Richard Moore
- Sketch -> Include Library -> Manage Libraries
2. Upload codes to TinyCircuit and ensure it is running
3. Connect phone to TinyCircuit via Bluetooth on the nRF connect app
- Default name of the project is set to Feeduino, you can change it if you like in the uart file
- Note that both the name and local_name variables have to be changed for it to be reflected correctly in the nRF connect app
4. Using the UART RX Characteristic, send a link 
- Must be less than 20 characters long
- Consider using a URL shortener
5. Link will be converted into a QR code on the watch
6. As an added security feature, QR code would be removed afetr 30s

Additional future implementations:
1. Connect the TinyCircuit to the power source that is used to power the secondary Arduino board
2. Allow the TinyCircuit to connect to a Wifi network / breadboard used for the secondary Arduino board, which in turn connects to a Wifi network
*/

// Initilization of Tiny Screen
TinyScreen display = TinyScreen(0);

// Constant values to centre QR Code
uint8_t x_offset = 19;
uint8_t y_offset = 3;
char* code = "none";

//Debug output adds extra flash and memory requirements!
#ifndef BLE_DEBUG
#define BLE_DEBUG false
#endif

#if defined (ARDUINO_ARCH_AVR)
#define SerialUSB Serial
#elif defined(ARDUINO_ARCH_SAMD)
#define SerialUSB SerialUSB
#endif


uint8_t ble_rx_buffer[40];
uint8_t ble_rx_buffer_len = 0;
uint8_t ble_connection_state = false;
char* cmd = "uuid:";
#define PIPE_UART_OVER_BTLE_UART_TX_TX 0

void setup() {
  // put your setup code here, to run once:
  SerialUSB.begin(9600);
  Wire.begin();
  display.begin();
  display.setFlip(true);
  // Set screen brightness to max (15)
  display.setBrightness(15);
  draw_start_screen();

//  if u want to display qr code without bt
//  code = "https://youtu.be/dQw4w9WgXcQ";
//  SerialUSB.println("Printing QR...");
//  SerialUSB.print("Code entered: ");
//  SerialUSB.println(code);
//  genQRCode();
//  SerialUSB.println("Done");
  
  //while (!SerialUSB); //This line will block until a serial monitor is opened with TinyScreen+!
  BLEsetup();
}

void loop() {
  aci_loop();//Process any ACI commands or events from the NRF8001- main BLE handler, must run often. Keep main loop short.
  if (ble_rx_buffer_len) {//Check if data is available
    code = (char*)ble_rx_buffer;
    SerialUSB.println("Printing QR...");
    SerialUSB.print("Code entered: ");
    SerialUSB.println(code);
    genQRCode();
    SerialUSB.println("Done");
    ble_rx_buffer_len = 0;//clear afer reading
    for (int i=1; i<31; i++) 
    {
      SerialUSB.print("Time Delay: ");
      SerialUSB.print(i);
      SerialUSB.println("s");
      delay(1000);
    }
    draw_start_screen();
    SerialUSB.println("QR code erased");
  }
}

void genQRCode() {
  // Initializing QR Code
  draw_white_rect();
  QRCode qrcode;
  uint8_t qrcodeBytes[qrcode_getBufferSize(3)];
  uint8_t color;
  // Sets QRCode Content
  qrcode_initText(&qrcode, qrcodeBytes, 3, ECC_LOW, code);
  // Start of display
  display.startData();
  //lib_aci_send_data(0,"Displaying QR Code",18);
  for (uint8_t y = 0; y < qrcode.size; y++) {
    for (uint8_t x = 0; x < qrcode.size; x++) {
      // Getting color of QR Code at Location X Y
      color = qrcode_getModule(&qrcode, x , y) ? TS_16b_Black : TS_16b_White;
      if (color != TS_16b_White) {
        // Scales 1 pixel to 4
        display.drawPixel(x_offset + x * 2, y_offset + y * 2, color);
        display.drawPixel(x_offset + x * 2 + 1, y_offset + y * 2, color);
        display.drawPixel(x_offset + x * 2, y_offset + y * 2 + 1, color);
        display.drawPixel(x_offset + x * 2 + 1, y_offset + y * 2 + 1, color);
      }
    }
  }
  // End of Display
  display.endTransfer();
}

void draw_white_rect() {
  // draw white rect 
  display.drawRect(0, 0, 96, 64, TSRectangleFilled, TS_16b_White);
}

void draw_start_screen() {
  draw_white_rect();
  display.setFont(liberationSans_10ptFontInfo);
  display.setCursor(20, 15);
  display.fontColor(RED, WHITE);
  display.print("PLS CALL");
  display.setCursor(15, 30);
  display.print("9-SAFETY-1");
}
