#include <Arduino.h>
#include "BluetoothSerial.h"

//Commands codes
#define COMCODE_WAIT 0x00
#define COMCODE_SOUND_LEDS 0x01
//Leds defines
#define NUMBER_OF_COLORS 3  //R/G/B - 8bits each

class PackageLedsBT{
    public:
        uint8_t commandCode;
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