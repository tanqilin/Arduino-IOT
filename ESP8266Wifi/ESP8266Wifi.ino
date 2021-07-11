#include <ESP8266WiFi.h>

const char *ssid     = "ChinaNet-m39G";      //这里写入网络的ssid
const char *password = "jhg4xjdv";  //wifi密码

WiFiClient client;

void setup() {
  Serial.begin(115200);
  // put your setup code here, to run once:
  WiFi.begin(ssid, password);//启动

  //在这里检测是否成功连接到目标网络，未连接则阻塞。
  while (WiFi.status() != WL_CONNECTED)
  {
      delay(500);
      Serial.print("*");
  }

  Serial.println("");
  Serial.println("Wifi连接成功！");
  Serial.print("IP：");
  Serial.println(WiFi.localIP());
}

void loop() {
  // put your main code here, to run repeatedly:

}
