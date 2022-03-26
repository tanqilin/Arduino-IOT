/*
 * PIR 微型人体红外感应模块
 * @ 日期：2022-03-26
 */
 
const int signPin = 4;    // 信号输入引脚
int signState = 0;

void setup() {
  // 初始化ESP8266模块
  pinMode(signPin, INPUT);      // 设置信号接收引脚4为输入模式
  pinMode(LED_BUILTIN,OUTPUT);
  Serial.begin(115200);
  Serial.print("Serial start...");
}

void loop() {
  // 循环检测状态
  delay(500);
  signState = digitalRead(signPin);
  Serial.print("Sign Status:");
  Serial.println(signState);
  if (signState == HIGH) {
    // 点亮LED
    digitalWrite(LED_BUILTIN,LOW);
  }else{
    digitalWrite(LED_BUILTIN,HIGH);
  }
}
