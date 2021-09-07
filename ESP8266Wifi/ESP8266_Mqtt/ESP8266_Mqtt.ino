// ESP8266使用MQTT连接私有MQTT服务器(树莓派)
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

const char *ssid     = "ChinaNet-m39G";      //这里写入网络的ssid
const char *password = "jhg4xjdv";  //wifi密码

const char *mqttServer = "192.168.1.4";
const char *userName = "ESP8266";
const char *userPswd = "";
const char *subTopic = "tanqilin/out@center";
const char *pubTopic = "tanqilin/in@center";

WiFiClient wifiClient;
PubSubClient mqttClient(wifiClient);

void setup() {
  Serial.begin(115200);
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

  // 设置MQTT服务器和端口号
  mqttClient.setServer(mqttServer, 1883);
  mqttClient.setCallback(receiveCallback);
}

void loop() {
  if (mqttClient.connected()) { // 如果开发板成功连接服务器    
    mqttClient.loop();          // 保持客户端心跳
  } else {                  // 如果开发板未能成功连接服务器
    connectMQTTServer();    // 则尝试连接服务器
  }
}

void connectMQTTServer(){
  // 根据ESP8266的MAC地址生成客户端ID（避免与其它ESP8266的客户端ID重名）
  String clientId = "esp8266-" + WiFi.macAddress();

   // 连接MQTT服务器
  if (mqttClient.connect(clientId.c_str(),userName,userPswd)) { 
    Serial.println("MQTT Server Connected.");
    Serial.println("Server Address: ");
    Serial.println(mqttServer);
    Serial.println("ClientId:");
    Serial.println(clientId);

    // 连接成功后订阅主题
    subscribeTopic();
  } else {
    Serial.print("MQTT Server Connect Failed. Client State:");
    Serial.println(mqttClient.state());
    delay(3000);
  }   
}

// mqtt消息回调函数
void receiveCallback(char* topic, byte* payload, unsigned int length){
  Serial.print("Message Received [");
  Serial.print(topic);
  Serial.print("] ");

  // 把byte转成string打印出来
  char* msg = new char[length];
  memcpy(msg,payload,length);
  msg[length] = 0;
  Serial.println("");
  Serial.print("Message：");
  Serial.println(msg);
}

// 订阅主题
void subscribeTopic(){
  // 请注意subscribe函数第二个参数数字为QoS级别。这里为QoS = 1
  if(mqttClient.subscribe(subTopic, 0)){
    Serial.print("已成功订阅主题: ");
    Serial.println(subTopic);
    // msg为json类型字符串，如需处理需要调用ArduinoJson库进行处理
    String pub = "{\"result\":\"ok\"}";
    mqttClient.publish(pubTopic,pub.c_str());
  } else {
    Serial.print("Subscribe Fail...");
  }  
}
