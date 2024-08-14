/**
 * Project:  Arduino Coder to send programming advertising packet to Skarper 
 * @file     SkarperProgrammer.ino
 * @brief    Reads in a 3 digit hex number from switches and sends an advertising packet
 *           with 3 digits in it
 *
 *
 * @authors  Ted Wood
 * @copyright Copyright (c) 2024 c) Blue Sky IP DBA Skarper
 **/
#include <ArduinoBLE.h>

BLEService myService("fff0");
BLEIntCharacteristic myCharacteristic("fff1", BLERead | BLEBroadcast);

// Advertising parameters should have a global scope. Do NOT define them in 'setup' or in 'loop'
const uint8_t completeRawAdvertisingData[] = {0x02,0x01,0x06,0x09,0xff,0x01,0x01,0x00,0x01,0x02,0x03,0x04,0x05};   
/**
* Name string - leaving space for hex digits and terminator at end
*/
char Name[20] = "Skarper Prog";

// Switch values as 8-bit number 0 - 15
char SwitchVal[3] = {0};

/**
 * @brief convert an integer between 0 and 15 to a hex character
 * Max Filter Size is set at default NUM_TAPS in fir.h
 *
 * @param c
 * @param cfs - coefficient array
 * @return Nothing
 */
char hex2char(uint8_t val)
{
  char rv =' ';
  if (val > 9)
  { // A - F
    rv = 'A' + (val-10);
  }
  else
  {
    if (val > 15)
    {
      //Invalid number
      rv = '$';
    }
    else
    { // 0 - 9
      rv = '0' + val;
    }
  }
return rv;
}




void makeName(void)
{
   
  for (int i = 2; i >= 0; i--)
  {
      SwitchVal[i] =0xF & ~SwitchVal[i];
      //Serial.print(digitVal[i],HEX);
      Name[14-i] = hex2char(SwitchVal[i]);
  }
   
  Name[15]=0;
}

void readSwitches(void)
{
SwitchVal[0] = SwitchVal[1] =SwitchVal[2] = 0;
for (uint8_t i = 2; i <=5; i++)
  {

    if (digitalRead(i))
    {
      SwitchVal[0]  |= 1 << (i-2);
    }
  }

  for (uint8_t i = 9; i <=12; i++)
  {
   if (digitalRead(i))
    {
      SwitchVal[1]  |= 1 << (i-9);
    }
  }
   for (uint8_t i = 14; i <=17; i++)
  {
  if (digitalRead(i))
    {
      SwitchVal[2]  |= 1 << (i-14);
    }
  }
}


void setup() {
  //start serial connection
  Serial.begin(115200);
  Serial.println("Skarper Programmer");
  //configure pin 2 as an input and enable the internal pull-up resistor
  // Initialise 3 groups of 4-bit inputs
  for (uint8_t i = 2; i <=5; i++)
  {
    pinMode(i, INPUT_PULLUP);
    pinMode(i+7, INPUT_PULLUP);
    pinMode(i+12, INPUT_PULLUP);
  }
  readSwitches();
  makeName();
  Serial.println(Name);
#if 1
  if (!BLE.begin()) {
    Serial.println("failed to initialize BLE!");
    while (1);
  }

  myService.addCharacteristic(myCharacteristic);
  BLE.addService(myService);

  // Build advertising data packet
  BLEAdvertisingData advData;
  // If a packet has a raw data parameter, then all the other parameters of the packet will be ignored
  advData.setRawData(completeRawAdvertisingData, sizeof(completeRawAdvertisingData));  
  // Copy set parameters in the actual advertising packet
  BLE.setAdvertisingData(advData);

  // Build scan response data packet
  BLEAdvertisingData scanData;
  scanData.setLocalName(Name);
  // Copy set parameters in the actual scan response packet
  BLE.setScanResponseData(scanData);
  
  BLE.advertise();

  Serial.println("advertising ...");
  #endif
}

void loop() {
  Serial.println(Name);
  BLE.poll();
#if 0
  if (millis() > 5000)
  {
    exit (0);
  }
#endif
}
  
 
