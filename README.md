# ESP32-TTS-LVGL-DEMO
A Text-To-Speech demo for ESP32 using ESP8266SAM &amp; LVGL 8 Arduino libraries.

![espsam1](https://github.com/OldMate6288/ESP32-TTS-LVGL-DEMO/assets/93004427/c1206b1e-ceed-450f-b15a-d61c6c97c41f), ![espsam2](https://github.com/OldMate6288/ESP32-TTS-LVGL-DEMO/assets/93004427/2d9e0fe9-a801-4379-bc18-99a5aa8871d1)


### ** HAS CURRENTLY ONLY BEEN TESTED ON ESP32-02432S028 [FULLY WORKING WITHOUT DAC] **

# SETUP:
1. Make sure you have the following libraries installed in Arduino IDE:
   - LVGL (8.3.6)
   - ESP8266Audio
   - ESP8266SAM (https://github.com/earlephilhower/ESP8266SAM)

2. Edit lgfx_ESP32_2432S028.h to match your screen driver, touch & pin config.

3. Change I2S output pins (in setup) to your speaker config.

4. Compile & Upload!

(You can alternatively upload the "SerialToSpeech" code to run ESP8266SAM on any ESP32/ESP8266 board, this sends messages to SAM via the serial monitor rather than using a display.)

This is just a basic demo, there is no gain control so expect a loud output...

### [EXTRA] USEFUL RESOURCES FOR ESP32-02432S028 (CHEAP YELLOW DISPLAY):

- https://macsbug.wordpress.com/2022/08/17/esp32-2432s028/
- https://github.com/rzeldent/esp32-smartdisplay
- https://www.openhasp.com/0.7.0/hardware/sunton/esp32-2432s028/
