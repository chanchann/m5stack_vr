#include <M5Stack.h>
#include "utility/MPU9250.h"

MPU9250 IMU; // new a MPU9250 object

void setup() {
  M5.begin();
  Wire.begin();

  uint8_t id = IMU.readByte(MPU9250_ADDRESS, WHO_AM_I_MPU9250);
  M5.Lcd.print("MPU9250 I AM 0x"); M5.Lcd.print(id, HEX);
}

void loop() {
}
