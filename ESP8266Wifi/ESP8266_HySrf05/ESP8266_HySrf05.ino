/*
 * ESP8266 驱动 hy-srf05 超声波传感器
 * @ 日期：2022-03-26
 */
#include <ESP8266WiFi.h>

const int trigPin = D1;
const int echoPin = D2;

long duration;
int distance;

void setup() {
  // 初始化ESP8266模块
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the trigPin as an Input
  
  Serial.begin(115200);
  Serial.print("Serial start...");
}

void loop() {
  digitalWrite(echoPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);  
  // 读取 echoPin 为高电平的持续时间
  int distance = pulseIn(echoPin, HIGH);

  // 根据时间计算距离
  distance= distance/58;
  Serial.println(distance);           
  delay(500);  
}
