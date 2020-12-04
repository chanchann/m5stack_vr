#include "BluetoothSerial.h"
#include <M5Stack.h>
BluetoothSerial SerialBT;

void setup() {
  M5.begin();
  M5.Power.begin();
  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setTextColor(GREEN , BLACK);
  M5.Lcd.setTextSize(2);
  Serial.begin(115200);
  SerialBT.begin("ESP32test"); 
  Serial.println("Sending single char every 3 seconds:");
}

void loop() {
  Serial.write(45);
  SerialBT.write(45);
  SerialBT.write(Serial.read());
  M5.Lcd.setCursor(0, 20);
  M5.Lcd.println("send....");
  delay(3000);
}
