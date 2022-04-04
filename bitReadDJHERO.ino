#include <Wire.h>
#define BAUDRATE 19200
#define ADDRESS 0x52

void setup(){
   Serial.begin(BAUDRATE);

   Wire.begin();
   Wire.beginTransmission(ADDRESS);
   Wire.write(0xF0);
   Wire.write(0x55);
   Wire.endTransmission();
   delay(1);
   Wire.beginTransmission(ADDRESS);
   Wire.write(0xFB);
   Wire.write((uint8_t)0x00);
   Wire.endTransmission();
}

void loop(){
  int count = 0;
  int values[6];
  byte RTT = 00000000;//Right turntable
  byte LTT = 00000000;//Left turntable
  byte EFD = 00000000;//Effects dial
  byte CFS = 00000000;//Crossfade slider
  byte SX = 00000000;//Analog stick x
  byte SY = 00000000;//Analog stick y

  Wire.requestFrom (ADDRESS, 6);s
  while(Wire.available()){
    values[count] = Wire.read();
    count++;
  }

  //Print raw binary data
  // Serial.println(values[0], BIN);
  // Serial.println(values[1], BIN);
  // Serial.println(values[2], BIN);
  // Serial.println(values[3], BIN);
  // Serial.println(values[4], BIN);
  // Serial.println(values[5], BIN);
  // Serial.println("-------------------");

  //BitRead section
  if(bitRead(values[5], 2) == 0){//Right Blue Button
    Serial.println("BLUE BUTTON R!!!:)");
  }
  if(bitRead(values[5], 7) == 0){//Left Blue Button
    Serial.println("BLUE BUTTON L!!!:)");
  }
  if(bitRead(values[5], 5) == 0){//Right Green Button
    Serial.println("GREEN BUTTON R!!!:)");
  }
  if(bitRead(values[5], 3) == 0){//Left Green Button
    Serial.println("GREEN BUTTON L!!!:)");
  }
  if(bitRead(values[4], 1) == 0){//Right Red Button
    Serial.println("RED BUTTON R!!!:)");
  }
  if(bitRead(values[4], 5) == 0){//Left Red Button
    Serial.println("RED BUTTON L!!!:)");
  }
  if(bitRead(values[5], 4) == 0){//Euphoria Button
    Serial.println("EUPHORIA BUTTON!!!:)");
  }
  if(bitRead(values[4], 2) == 0){//Plus
    Serial.println("PLUS BUTTON!!!:)");
  }
  if(bitRead(values[4], 4) == 0){//Minus
    Serial.println("MINUS BUTTON!!!:)");
  }

  //Set value of RTT
  //Range: [-128,-97] & [0,31]
  //Because the turntable returns a 6 bit signed number, the sign bit (5) is moved to bit 7
  //Positive numbers stay the same, but negative numbers range from -128 to -97
  //-97 = -1 and -128 = -32, so true value = displayed value + 96, for negative values
  //Conversion could be done by checking if num is negative and applying the equation if
  //it is, but hardcoding the adjusted values for negative numbers will use less processing power
  bitWrite(RTT, 0, bitRead(values[2], 7));
  bitWrite(RTT, 1, bitRead(values[1], 6));
  bitWrite(RTT, 2, bitRead(values[1], 7));
  bitWrite(RTT, 3, bitRead(values[0], 6));
  bitWrite(RTT, 4, bitRead(values[0], 7));
  bitWrite(RTT, 7, bitRead(values[2], 0));

  //Set value of LTT
  //Same disclaimer as RTT
  bitWrite(LTT, 0, bitRead(values[3], 0));
  bitWrite(LTT, 1, bitRead(values[3], 1));
  bitWrite(LTT, 2, bitRead(values[3], 2));
  bitWrite(LTT, 3, bitRead(values[3], 3));
  bitWrite(LTT, 4, bitRead(values[3], 4));
  bitWrite(LTT, 7, bitRead(values[4], 0));

  //Set value of EFD
  //Range: [0,31]
  //Clockwise increases value until wrap around
  bitWrite(EFD, 0, bitRead(values[3], 5));
  bitWrite(EFD, 1, bitRead(values[3], 6));
  bitWrite(EFD, 2, bitRead(values[3], 7));
  bitWrite(EFD, 3, bitRead(values[2], 5));
  bitWrite(EFD, 4, bitRead(values[2], 6));

  //Set value of CFS
  //Range: [0,15]
  //Left is 0, middle 7, right 15
  //Despite clicking into place at 0,7,15, it is still analog and values between
  //the click points exist
  bitWrite(CFS, 0, bitRead(values[2], 1));
  bitWrite(CFS, 1, bitRead(values[2], 2));
  bitWrite(CFS, 2, bitRead(values[2], 3));
  bitWrite(CFS, 3, bitRead(values[2], 4));

  //set value of SX
  //Range: [0,63]
  //Joystick axis are the only inputs that we can use loops to read each bit,
  // all other inputs have bits spread out in different bytes
  for(int idx = 0; idx ++; idx < 6){
    bitWrite(SX, idx, bitRead(values[0], idx));
  }

  //set value of SY
  //Range: [0,63]
  for(int idx = 0; idx ++; idx < 6){
    bitWrite(SY, idx, bitRead(values[1], idx));
  }

  Serial.println(RTT);


  Wire.beginTransmission(ADDRESS);
  Wire.write((uint8_t)0x00);
  Wire.endTransmission();
  delay(100);

}
