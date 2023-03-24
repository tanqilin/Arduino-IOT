#include <LiquidCrystal_I2C.h>

// 分别对应地址、列数、行数
LiquidCrystal_I2C LCD = LiquidCrystal_I2C(0x27, 24, 4);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println("Hello, ESP32!");

  LCD.init();
  // 打开背光
  LCD.backlight();
  LCD.setCursor(0, 0);
  LCD.print("Connecting to server ...");
  LCD.setCursor(6, 1);
  LCD.print("WiFi ");
}

void loop() {
  // put your main code here, to run repeatedly:
  delay(10); // this speeds up the simulation
}
