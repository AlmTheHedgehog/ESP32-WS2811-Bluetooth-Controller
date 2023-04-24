#include <Arduino.h>
#include "BluetoothSerial.h"

#include "esp32WS2811/esp32WS2811.h"
#include "PackageLedsBT.hpp"

#define NUMBER_OF_LEDS 100
#define LED_STRIP_DATA_PIN 4
// #define LEVEL_SHIFTER_PIN 14

//colors
#define LIGHT_BRIGHTNESS 40


BluetoothSerial SerialBT;
WS2811 ws2811(LED_STRIP_DATA_PIN, NUMBER_OF_LEDS);

void setup() {
    // Serial.begin(115200);
    #ifdef LEVEL_SHIFTER_PIN
        pinMode(LEVEL_SHIFTER_PIN, OUTPUT);
        digitalWrite(LEVEL_SHIFTER_PIN, HIGH);
    #endif

    SerialBT.begin("ESP32-SOUND-LED-CONTROLLER"); //Bluetooth device name

    ws2811.begin();
    ws2811.setAll(0, 0, 0);
    ws2811.show();
}

void loop() {
    delay(1);

    if(SerialBT.available()){
        PackageLedsBT package;
        package.fillPackageFromBT(SerialBT);
        if(package.commandCode == COMCODE_SOUND_LEDS){
            for(uint16_t i = 0; i < package.ledsArraySize; ++i){
                ws2811.setPixel(i, package.ledsArray[(i*NUMBER_OF_COLORS)], package.ledsArray[(i*NUMBER_OF_COLORS) + 2], 
                                package.ledsArray[(i*NUMBER_OF_COLORS) + 1]); // last 2 elements are were exchanged because in library smth wrong and it mixed up green and blue
            }
        }else{
            ws2811.setAll(LIGHT_BRIGHTNESS, LIGHT_BRIGHTNESS, LIGHT_BRIGHTNESS);
        }
        ws2811.show();
    }
}