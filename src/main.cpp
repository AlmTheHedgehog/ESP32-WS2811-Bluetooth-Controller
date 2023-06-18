#include <Arduino.h>
#include "BluetoothSerial.h"

#include "esp32WS2811/esp32WS2811.h"
#include "PackageLedsBT.hpp"

#define NUMBER_OF_LEDS (100)
#define LED_STRIP_DATA_PIN (4)
// #define LEVEL_SHIFTER_PIN 14

//colors
#define LIGHT_BRIGHTNESS (40)

/*SOUND LEDS*/
#define NUMBER_OF_RED_IN_PATTERN (2)
#define NUMBER_OF_GREEN_IN_PATTERN (2)
#define NUMBER_OF_BLUE_IN_PATTERN (2)


BluetoothSerial SerialBT;
WS2811 ws2811(LED_STRIP_DATA_PIN, NUMBER_OF_LEDS);

void fillTapeBySolidColor(uint8_t r, uint8_t g, uint8_t b);
void setPixelInTape(uint16_t index, uint8_t r, uint8_t g, uint8_t b);

void execute_COMCODE_WAIT(const PackageLedsBT& package);
void execute_COMCODE_LEDS_ARRAY(const PackageLedsBT& package);
void execute_COMCODE_SOLID_COLOR(const PackageLedsBT& package);
void execute_COMCODE_RGB_EQ_SOUND(const PackageLedsBT& package);
void execute_COMCODE_ONE_COLOR_EQ_SOUND(const PackageLedsBT& package);

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
        if(package.commandCode == COMCODE_LEDS_ARRAY){
            execute_COMCODE_LEDS_ARRAY(package);
        }else if(package.commandCode == COMCODE_SOLID_COLOR){
            execute_COMCODE_SOLID_COLOR(package);
        }else if(package.commandCode == COMCODE_RGB_EQ_SOUND){
            execute_COMCODE_RGB_EQ_SOUND(package);
        }else if(package.commandCode == COMCODE_ONE_COLOR_EQ_SOUND){
            execute_COMCODE_ONE_COLOR_EQ_SOUND(package);
        }else{
            execute_COMCODE_WAIT(package);
        }
        ws2811.show();
    }
}

void fillTapeBySolidColor(uint8_t red, uint8_t green, uint8_t blue){
    ws2811.setAll(red, blue, green);  // last 2 elements are were exchanged because in library smth wrong and it mixed up green and blue
}

void setPixelInTape(uint16_t index, uint8_t r, uint8_t g, uint8_t b){
    ws2811.setPixel(index, r, b, g);  // last 2 elements are were exchanged because in library smth wrong and it mixed up green and blue
}

void execute_COMCODE_WAIT(const PackageLedsBT &package){
    fillTapeBySolidColor(LIGHT_BRIGHTNESS, LIGHT_BRIGHTNESS, LIGHT_BRIGHTNESS);
}

void execute_COMCODE_LEDS_ARRAY(const PackageLedsBT& package){
    for(uint16_t i = 0; i < package.ledsArraySize; ++i){
        setPixelInTape(i, package.ledsArray[(i*NUMBER_OF_COLORS)], package.ledsArray[(i*NUMBER_OF_COLORS) + 1], 
                        package.ledsArray[(i*NUMBER_OF_COLORS) + 2]);
    }
}

void execute_COMCODE_SOLID_COLOR(const PackageLedsBT& package){
    fillTapeBySolidColor(package.color.red, package.color.green, package.color.blue);
}

void execute_COMCODE_RGB_EQ_SOUND(const PackageLedsBT& package){
    //patern for numberOfLefs % 10 == 0 - 2low(red)-2mid(green)-2high(blue)-2mid(green)-2low(red)
    for(uint16_t i = 0; i < NUMBER_OF_LEDS; i += 10){
        uint16_t j = 0;
        for(j; j < NUMBER_OF_RED_IN_PATTERN; ++j){
            setPixelInTape(i+j, package.color.red, 0, 0);
        }
        for(j; j < (NUMBER_OF_RED_IN_PATTERN + NUMBER_OF_GREEN_IN_PATTERN); ++j){
            setPixelInTape(i+j, 0, package.color.green, 0);
        }
        for(j; j < (NUMBER_OF_RED_IN_PATTERN + 
                    NUMBER_OF_GREEN_IN_PATTERN +
                    NUMBER_OF_BLUE_IN_PATTERN); ++j){
            setPixelInTape(i+j, 0, 0, package.color.blue);
        }
        for(j; j < (NUMBER_OF_RED_IN_PATTERN + 
                    NUMBER_OF_GREEN_IN_PATTERN +
                    NUMBER_OF_BLUE_IN_PATTERN +
                    NUMBER_OF_GREEN_IN_PATTERN); ++j){
            setPixelInTape(i+j, 0, package.color.green, 0);
        }
        for(j; j < (NUMBER_OF_RED_IN_PATTERN + 
                    NUMBER_OF_GREEN_IN_PATTERN +
                    NUMBER_OF_BLUE_IN_PATTERN +
                    NUMBER_OF_GREEN_IN_PATTERN +
                    NUMBER_OF_RED_IN_PATTERN); ++j){
            setPixelInTape(i+j, package.color.red, 0, 0);
        }
        if((i+10) >= NUMBER_OF_LEDS){
            break;
        }
    }
}

void execute_COMCODE_ONE_COLOR_EQ_SOUND(const PackageLedsBT &package){
    if(package.color.red > 0){
        fillTapeBySolidColor(package.color.red, 0, 0);
    }else if(package.color.green > 0){
        fillTapeBySolidColor(0, package.color.green, 0);
    }else{
        fillTapeBySolidColor(0, 0, package.color.blue);
    }
}
