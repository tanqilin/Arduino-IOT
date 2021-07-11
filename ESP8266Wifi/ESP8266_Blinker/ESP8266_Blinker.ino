// 使用点灯科技控制esp8266
#define BLINKER_WIFI
#define BLINKER_MIOT_LIGHT

#include <Blinker.h>


char auth[] = "66753bf50c19";
char ssid[] = "ChinaNet-m39G";
char pswd[] = "jhg4xjdv";

// 控件
BlinkerButton Button("btn-abc");

void setup() {
  Serial.begin(115200);
  pinMode(LED_BUILTIN,OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
  
  // put your setup code here, to run once:
  Blinker.begin(auth,ssid,pswd); 
  Button.attach(button_callback);
  BlinkerMIOT.attachPowerState(miotPowerState);
  Serial.println("初始化成功！");
}

void loop() {
  Blinker.run();
}

void button_callback(const String & state){
  Serial.println(state);
  digitalWrite(LED_BUILTIN,!digitalRead(LED_BUILTIN));

  if(state == BLINKER_CMD_ON){
    Button.icon("fad fa-lightbulb-on");
    Button.text("开灯");
    Button.print("on");
  }  else if(state ==  BLINKER_CMD_OFF){
    Button.icon("fas fa-lightbulb-exclamation");
    Button.text("关灯");
    Button.print("off");
  }
}

void miotPowerState(const String & state){
  if(state == BLINKER_CMD_ON){
    digitalWrite(LED_BUILTIN,HIGH);
    BlinkerMIOT.powerState("on");
    BlinkerMIOT.print();
  }  else if(state ==  BLINKER_CMD_OFF){
    digitalWrite(LED_BUILTIN,LOW);
    BlinkerMIOT.powerState("off");
    BlinkerMIOT.print();
  }
}
