#include <Arduino.h>
#include "BluetoothSerial.h"

//Commands codes
#define COMCODE_WAIT 0x00 //format: COMCODE    - set all LEDS to LIGHT_BRIGHTNESS
#define COMCODE_LEDS_ARRAY 0x01  //format: COMCODE, number_of_leds(2bytes), [0..LEDs](color for each led(3bytes each))     - fill LED tape by colors array
#define COMCODE_SOLID_COLOR 0x02  //format: COMCODE, color(r/g/b(3bytes))    -fill whole tape by solid color
#define COMCODE_RGB_EQ_SOUND 0x03  //format: COMCODE, color(r/g/b(3bytes))    -separate tape into low(r)/mid(g)/high(b) freq zones and fill them by colors
#define COMCODE_ONE_COLOR_EQ_SOUND 0x04  //format: COMCODE, color(r/g/b(3bytes))    -fill whole tape by 1 color which is not zero(e.g. FF0000).(if more then 1 color entered(e.g.FF00FF) -> will be chosen first)
//Leds defines
#define NUMBER_OF_COLORS 3  //R/G/B - 8bits each

class PackageLedsBT{
    public:
        uint8_t commandCode;
        struct _Color{
            uint8_t red = 0;
            uint8_t green = 0;
            uint8_t blue = 0;
        }color;
        uint16_t ledsArraySize;
        uint8_t * ledsArray;

    PackageLedsBT(): commandCode(0), ledsArraySize(0), ledsArray(nullptr){};
    PackageLedsBT(const PackageLedsBT & parent): 
    commandCode(parent.commandCode), ledsArraySize(parent.ledsArraySize), ledsArray(parent.ledsArray){};
    ~PackageLedsBT();

    void fillPackageFromBT(BluetoothSerial & serialBT);
    void allocateArrayForLeds();  //array will be a matrix[newPackage.ledsArraySize][NUMBER_OF_COLORS]

    private:
        uint16_t read2ByteNumberFromBT(BluetoothSerial & serialBT);
};