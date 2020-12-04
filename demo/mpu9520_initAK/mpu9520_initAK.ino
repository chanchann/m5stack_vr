#include <M5Stack.h>
#include "utility/MPU9250.h"

MPU9250 IMU; // new a MPU9250 object

void setup() {
  M5.begin();
  Wire.begin();

  IMU.initAK8963(IMU.magCalibration);
  M5.Lcd.println("AK8963 initialized for active data mode....");
  if (Serial)
  {
    M5.Lcd.println("Calibration values: ");
    M5.Lcd.print("X-Axis sensitivity adjustment value ");
    M5.Lcd.println(IMU.magCalibration[0], 2);
    M5.Lcd.print("Y-Axis sensitivity adjustment value ");
    M5.Lcd.println(IMU.magCalibration[1], 2);
    M5.Lcd.print("Z-Axis sensitivity adjustment value ");
    M5.Lcd.println(IMU.magCalibration[2], 2);
  }
}

void loop() {
}
