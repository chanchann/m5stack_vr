#include <M5Stack.h>
#include "utility/MPU9250.h"

MPU9250 IMU; // new a MPU9250 object

void setup(){
    M5.begin();
    Serial.begin(115200);

    IMU.initMPU9250();
    byte c = IMU.readByte(MPU9250_ADDRESS, WHO_AM_I_MPU9250);
    Serial.print("MPU9250 "); Serial.print("I AM "); Serial.print(c, HEX);
    IMU.calibrateMPU9250(IMU.gyroBias, IMU.accelBias);
}

void loop(){
    // If intPin goes high, all data registers have new data
    // On interrupt, check if data ready interrupt
    if (IMU.readByte(MPU9250_ADDRESS, INT_STATUS) & 0x01)
    {
        // "gyroCount[3] = {0};" was defined at file "MPU9250.h"
        IMU.readGyroData(IMU.gyroCount);  // Read the x/y/z adc values
        IMU.getGres(); // get Gyro scales saved to "gRes"
        IMU.gx = (float)IMU.gyroCount[0]*IMU.gRes;
        IMU.gy = (float)IMU.gyroCount[1]*IMU.gRes;
        IMU.gz = (float)IMU.gyroCount[2]*IMU.gRes;
        Serial.print("X-gyro rate: "); Serial.print(IMU.gx, 3);
        Serial.print(" degrees/sec ");
        Serial.print("Y-gyro rate: "); Serial.print(IMU.gy, 3);
        Serial.print(" degrees/sec ");
        Serial.print("Z-gyro rate: "); Serial.print(IMU.gz, 3);
        Serial.println(" degrees/sec");
    }
}
