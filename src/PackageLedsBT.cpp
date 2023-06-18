#include "PackageLedsBT.hpp"

PackageLedsBT::~PackageLedsBT(){
    if(ledsArray){
        delete[] ledsArray;
    }
}

void PackageLedsBT::fillPackageFromBT(BluetoothSerial & serialBT){
    commandCode = (uint8_t)serialBT.read();
    if(commandCode == COMCODE_LEDS_ARRAY){
        ledsArraySize = read2ByteNumberFromBT(serialBT);
        allocateArrayForLeds();
        serialBT.readBytes(ledsArray, ledsArraySize * NUMBER_OF_COLORS);
    }else if((commandCode == COMCODE_SOLID_COLOR) || 
            (commandCode == COMCODE_RGB_EQ_SOUND) || 
            (commandCode == COMCODE_ONE_COLOR_EQ_SOUND)){
        color.red = (uint8_t)serialBT.read();
        color.green = (uint8_t)serialBT.read();
        color.blue = (uint8_t)serialBT.read();
    }
}

void PackageLedsBT::allocateArrayForLeds(){
    if(ledsArray){
        delete[] ledsArray;
    }
    ledsArray = new uint8_t[ledsArraySize * NUMBER_OF_COLORS];
}


/*! @brief Reads 2 bytes from BT buffer and convert it into uint16_t.
*   Firstly comes higher byte then lower.
*/
uint16_t PackageLedsBT::read2ByteNumberFromBT(BluetoothSerial & serialBT){
    uint8_t higer = (uint8_t)serialBT.read();
    uint8_t lower = (uint8_t)serialBT.read();
    return (uint16_t)((higer<<8) | lower);
}

