# 1003-feeduino
Automated fish feeder for ICT1003

**Note:**
1. TinyCircuit and secondary Arduino board are 2 separate components

# TinyCiruit QR:
_Note:_
  1. This is a limited running prototype that was designed to use Bluetooth instead of Wifi
  2. Development for usage of Wifi to send QR code to watch requires an additional hardware component 
     (Wifi shield to connect TinyCircuit directly to a wifi network / Prototyping shield to connect the TinyCircuit to the
     breadboard used in the secondary Arduino board used in this project, which then connects to a Wifi Network)

_To run:_
  1. Download QRCode library by Richard Moore
  - Sketch -> Include Library -> Manage Libraries
  2. Upload Watch_QR codes to TinyCircuit and ensure it is running
  3. Connect phone to TinyCircuit via Bluetooth on the nRF connect app
  - Default name of the project is set to Feeduino, you can change it if you like in the uart file
  - Note that both the name and local_name variables have to be changed for it to be reflected correctly in the nRF connect app
  4. Using the UART RX Characteristic, send a link
  - Must be less than 20 characters long
  - Consider using a URL shortener
  5. Link will be converted into a QR code on the watch
  6. As an added security feature, QR code would be removed afetr 30s

_Additional future implementations:_
  1. Connect the TinyCircuit to the power source that is used to power the secondary Arduino board
  2. Allow the TinyCircuit to connect to a Wifi network / breadboard used for the secondary Arduino board, which in turn connects to a Wifi network

# Secondary Arduino Board (Arduino WeMos D1 Wifi)
