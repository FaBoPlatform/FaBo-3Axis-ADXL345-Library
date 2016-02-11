#include "fabo-adxl345.h"
#include "Wire.h"

FaBo3Axis fabo3axis;

void setup()
{
  Serial.begin(9600); // シリアルの開始デバック用
  
  Serial.println("Checking I2C device...");
  
  if(fabo3axis.searchDevice()){
    Serial.println("I am ADXL345");
    fabo3axis.configuration();
    fabo3axis.powerOn();
    fabo3axis.enableTap();  
  }

}

void loop() {

  byte tap = fabo3axis.readIntStatus();

  if(fabo3axis.isDoubleTap(tap)){
    Serial.println("Double Tap");
  } else if(fabo3axis.isSingleTap(tap)){
    Serial.println("Single Tap");
  }
  delay(1000);
}
