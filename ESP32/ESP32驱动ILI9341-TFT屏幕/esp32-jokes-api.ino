#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>

#define BTN_PIN 5   
#define TFT_DC 2      // 接模块 D/C SPI数据命令选择
#define TFT_RSET 4
#define TFT_CS 15     // 片选接口引脚

Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);

void setup() {
  pinMode(BTN_PIN, INPUT_PULLUP);
  pinMode(TFT_RSET, OUTPUT);

  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println("Hello, ESP32!");
  tft.begin();

  // 旋转tft屏幕显示
  tft.setRotation(1);

  tft.setCursor(5, 5);
  tft.setTextSize(2);
  tft.setTextColor(ILI9341_RED);
  tft.println("TFT display start...");
}

void loop() {
  // 按下复位键清空TFT屏幕显示
  if (digitalRead(BTN_PIN) == LOW) {
    tft.setCursor(5, 5);
    tft.fillScreen(ILI9341_WHITE);
    tft.println("TFT clear content success...");
  }
  delay(10); // this speeds up the simulation
}
