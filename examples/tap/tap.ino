#include "fabo-adxl345.h"
#include "Wire.h"
  
void setup()
{
  Serial.begin(9600); // シリアルの開始デバック用
  
  Serial.println("Checking I2C device...");
  
  if(fabo3Axis.searchDevice()){
    Serial.println("I am ADXL345");
  }
  Serial.println("Init...");
  fabo3Axis.configuration();
  fabo3Axis.powerOn();
  fabo3Axis.enableTap();  
}

void loop() {

  byte tap = fabo3Axis.readIntStatus();

  if(fabo3Axis.isDoubleTap(tap)){
    Serial.println("Double Tap");
  } else if(fabo3Axis.isSingleTap(tap)){
    Serial.println("Single Tap");
  }
  delay(100);
}
