#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <PubSubClient.h>
#include "src/ESP8266MyHome.h"
#include <FS.h>

const char *CONFIG_IP= "192.168.4.1";

const char *ssid     = "ESP8266-WEB";       //这里写入网络的ssid
const char *password = "00000000";            //wifi密码
const String subCtrTopic = "/"+ WiFi.macAddress() +"/center/control";   // 节点系统控制命令
const String subMsgTopic = "/brd/center/N-Meessage";                    // 接收系统广播消息

const char *pubCtrTopic = "/center/result/C-Control";                  // 回复版本控制中心控制结果
const char *pubRegTopic = "/center/node/C-Register";                   // 向控制中心注册
const char *pubVerTopic = "/center/node/C-Version";                    // 回复控制中心版本信息

ESP8266WebServer web_server(80);
WiFiClient client;
PubSubClient mqttClient(client);

void setup() {
  Serial.begin(115200);
  pinMode(LED_BUILTIN,OUTPUT);
  if(SPIFFS.begin()){
    Serial.println("SPIFFS Started.");
  }else {
    Serial.println("SPIFFS Failed to Start.");
  }

  // 开启配网模式
  bool status = autoConfig();
  if(!status)
    status = webConfig();

  // 配网成功后尝试连接MQTT服务
  if(status){
    mqttConnect();
  }
  
  Serial.print("Start listening on port 80 IP:");
  Serial.println(WiFi.localIP());
}

void loop() {
   web_server.handleClient();
   if (mqttClient.connected()) { // 如果开发板成功连接服务器    
    mqttClient.loop();          // 保持客户端心跳
  } else {                  // 如果开发板未能成功连接服务器
    mqttConnect();    // 则尝试连接服务器
  }
}

// 自动配网模式
bool autoConfig(){
  int i = 0;

  // 建立DynamicJsonDocument对象
  const size_t capacity = JSON_OBJECT_SIZE(1) + JSON_OBJECT_SIZE(2) + 60;
  DynamicJsonDocument doc(capacity);
 
  // 从闪存文件系统中读取即将解析的json文件,进行配网
  File file = SPIFFS.open("/config.json", "r"); 
  deserializeJson(doc, file);
  file.close();
  
  // 获取解析后的数据信息
  const char* wifi_ssid = doc["wifi"]["ssid"];
  const char* wifi_password = doc["wifi"]["password"];
  WiFi.begin(wifi_ssid,wifi_password);

  //初始化WebServer
  web_server.on("/", homepage);
  web_server.on("/conf", getConfigInfo);
  web_server.onNotFound(handleNotFound);
  web_server.begin();
  
  digitalWrite(LED_BUILTIN,LOW);
  while (WiFi.status() != WL_CONNECTED)
  {
      i++;
      delay(1000);
      Serial.print(i);
      if(i == 20){
        Serial.println("");
        Serial.println("自动配网失败！进入一键配网模式...");
        return false;
      }
  }


  Serial.println("");
  Serial.println("自动配网成功");
  digitalWrite(LED_BUILTIN,HIGH);
  return true;
}

// 进入Web配网模式
bool webConfig(){
  Serial.println("启动Web配网服务...");
  WiFi.mode(WIFI_AP_STA);
  WiFi.softAP(ssid, password);
  Serial.print("AP设置完成,IP:");
  Serial.println(WiFi.softAPIP());
  if (MDNS.begin("esp8266")) {
    Serial.println("MDNS responder started");
  }
  
  while(1){
    MDNS.update();
    web_server.handleClient();
    if (WiFi.status() == WL_CONNECTED)
    {
      
        Serial.println("Web配网成功！");
        digitalWrite(LED_BUILTIN,HIGH);
        return true;
    }

    digitalWrite(LED_BUILTIN,LOW);
    delay(500);
    digitalWrite(LED_BUILTIN,HIGH);
    delay(500);
  }
  return false;
}

// 连接MQTT服务
void mqttConnect(){
  // 建立DynamicJsonDocument对象
  DynamicJsonDocument doc(1024);
 
  // 从闪存文件系统中读取即将解析的json文件,进行配网
  File file = SPIFFS.open("/config.json", "r"); 
  deserializeJson(doc, file.readString());
  file.close();

  JsonObject root = doc.as<JsonObject>();
  const char * mqtt_ssid = root["mqtt"]["mqserver"];
  const char * mqtt_name = root["mqtt"]["mqname"];
  const char * mqtt_mqpswd = root["mqtt"]["mqpswd"];
  String ip = mqtt_ssid;
  String str_name = mqtt_name;
  Serial.println("连接MQTT服务器,IP:" + ip + ",MQTT服务器,Name:" + str_name);

  // 设置MQTT服务器和端口号
  mqttClient.setServer(mqtt_ssid, 1883);
  mqttClient.setCallback(receiveCallback);

  // 根据ESP8266的MAC地址生成客户端ID（避免与其它ESP8266的客户端ID重名）
  String clientId = WiFi.macAddress();

   // 连接MQTT服务器
  if (mqttClient.connect(clientId.c_str(),"ESP8266","")) { 
    Serial.println("MQTT Server Connected.");
    Serial.print("Server Address: ");
    Serial.println(mqtt_ssid);
    Serial.print("ClientId:");
    Serial.println(clientId);

    // 连接成功后订阅主题
    subscribeTopic();
  } else {
    Serial.print("MQTT Server Connect Failed. Client State:");
    Serial.println(mqttClient.state());
    delay(3000);
  }   
}

//自定义主页访问处理函数
void homepage() {
  //读取文件内容并且通过串口监视器输出文件信息
  String fileName = "/index.html";
  if(WiFi.status() != WL_CONNECTED)
    fileName = "/config.html";
    
  File file = SPIFFS.open(fileName, "r");
  if(file){
    web_server.streamFile(file,"text/html");
    file.close();
  }else{
    web_server.send(404, "text/plain", "404: Not found Index.html"); 
  }
}

// 获取配置信息
void getConfigInfo(){
  // 从闪存文件系统中读取即将解析的json文件,进行配网
  String jsonStr;
  File file = SPIFFS.open("/config.json", "r"); 
  //deserializeJson(doc, file);
  jsonStr = file.readString();
  file.close();
  
  Serial.println("Web查询配置信息："+jsonStr);
  web_server.send(200, "application/json", jsonStr); 
}

// 设置处理404情况的函数'handleNotFound'
void handleNotFound() {
  // 获取用户请求网址信息
  String webAddress = web_server.uri();

  // GET 
  bool fileReadOK = false;
  if(web_server.method() == 1){
    // 通过handleFileRead函数处处理用户访问
    fileReadOK = handleFileRead(webAddress);
  }else if(web_server.method() == 3){
    fileReadOK = handlePostRead(webAddress);
  }

  // 如果在SPIFFS无法找到用户访问的资源，则回复404 (Not Found)
  if (!fileReadOK){                                                 
    web_server.send(404, "text/plain", "404 Not Found"); 
  }
}

bool handleFileRead(String path) {            //处理浏览器HTTP访问

  if (path.endsWith("/")) {                   // 如果访问地址以"/"为结尾
    path = "/index.html";                     // 则将访问地址修改为/index.html便于SPIFFS访问
  } 
  
  String contentType = getContentType(path);  // 获取文件类型
  
  if (SPIFFS.exists(path)) {                     // 如果访问的文件可以在SPIFFS中找到
    File file = SPIFFS.open(path, "r");          // 则尝试打开该文件
    web_server.streamFile(file, contentType);    // 并且将该文件返回给浏览器
    file.close();                                // 并且关闭文件
    return true;                                 // 返回true
  }
  return false;                                  // 如果文件未找到，则返回false
}

// 处理POST请求
bool handlePostRead(String path){
  String mqip,mqname,mqpswd;
  if(path.endsWith("config")){
    String wifis = web_server.arg("ssid");
    String wifip = web_server.arg("password");
    mqip = web_server.arg("mqserver");
    mqname = web_server.arg("mqname");
    mqpswd = web_server.arg("mqpswd");
    WiFi.begin(wifis,wifip);
  }

  if(path.endsWith("reset")){
    Serial.println("重置网络状态成功，Wifi：清空,密码：清空");
    // 把Wifi信息写入配置文件中
    File dataFile = SPIFFS.open("/config.json", "w");
    dataFile.println("");
    dataFile.close(); 
    delay(100);
    ESP.restart();
  }
  
  if(WiFi.status() != WL_CONNECTED){
     String configJson = "{";
            configJson  += "\"wifi\": {";
            configJson  +=      "\"ssid\": \"" + WiFi.SSID() + "\",";
            configJson  +=      "\"password\": \""+ WiFi.psk() +"\"";
            configJson  +=   "},";
            configJson  +=   "\"mqtt\": {";
            configJson  +=      "\"mqserver\": \"" + mqip + "\",";
            configJson  +=      "\"mqname\": \""+ mqname +"\",";
            configJson  +=      "\"mqpswd\": \""+ mqpswd +"\"";
            configJson  +=   "}";
            configJson  += "}";

    Serial.println("收到配网信息："+configJson);
    // 把Wifi信息写入配置文件中
    File dataFile = SPIFFS.open("/config.json", "w");
    dataFile.println(configJson);
    dataFile.close(); 
  }
  web_server.send(200, "text/plain", "{\"result\":\"ok\"}");
  return true;
}

// 获取文件类型
String getContentType(String filename){
  if(filename.endsWith(".htm")) return "text/html";
  else if(filename.endsWith(".html")) return "text/html";
  else if(filename.endsWith(".css")) return "text/css";
  else if(filename.endsWith(".js")) return "application/javascript";
  else if(filename.endsWith(".png")) return "image/png";
  else if(filename.endsWith(".gif")) return "image/gif";
  else if(filename.endsWith(".jpg")) return "image/jpeg";
  else if(filename.endsWith(".ico")) return "image/x-icon";
  else if(filename.endsWith(".xml")) return "text/xml";
  else if(filename.endsWith(".pdf")) return "application/x-pdf";
  else if(filename.endsWith(".zip")) return "application/x-zip";
  else if(filename.endsWith(".gz")) return "application/x-gzip";
  return "text/plain";
}

// 订阅主题
void subscribeTopic(){
  // 订阅系统控制主题
  mqttClient.subscribe(subCtrTopic.c_str(), 0);
  if(mqttClient.subscribe(subMsgTopic.c_str(), 0)){
    String msg = subMsgTopic;
    Serial.println("已成功订阅系统广播主题: " + msg);
    // msg为json类型字符串，如需处理需要调用ArduinoJson库进行处理

    // 订阅成功后向系统注册
    Serial.println("发送注册信息: " + getRegInfo());
    mqttClient.publish(pubRegTopic,getRegInfo().c_str());
  } else {
    Serial.print("Subscribe Fail...");
  }  
}

// mqtt消息回调函数
void receiveCallback(char* topic, byte* payload, unsigned int length){
  String str_topic = topic;
  Serial.print("Message Received [");
  Serial.print(str_topic);
  Serial.println("] ");

  // 把byte转成string打印出来
  char* msg = new char[length];
  memcpy(msg,payload,length);
  msg[length] = 0;
  
  // 解析接收到的json数据
  DynamicJsonDocument doc(1024);
  deserializeJson(doc, msg);
  JsonObject root = doc.as<JsonObject>();

  String str_method = root["method"];
  Serial.print("MQTT Message：");
  Serial.println(str_method);
  
  // 广播消息
  if(str_topic == subMsgTopic){
    Serial.print("MQTT Message：广播消息");
    if(str_method == "C-Version")
    {
      mqttClient.publish(pubRegTopic,getRegInfo().c_str());
    }
  }else{
    Serial.print("MQTT Message：控制消息");
    if(str_method == "C-Reset"){
      ESP.restart();
    }
    if(str_method == "C-Close")
    {
      digitalWrite(LED_BUILTIN,HIGH);
      Serial.println("MQTT Commad：C-Close");
    }
    if(str_method == "C-Open")
    {
      digitalWrite(LED_BUILTIN,LOW);
      Serial.println("MQTT Commad：C-open");
    }
  }
}

String getRegInfo(){
  String reg = "{";
    reg+=   "\"method\":\"C-Register\",";
    reg+=   "\"code\":\"\",";
    reg+=   "\"data\":{";
    reg+=     "\"Type\":\"switch\",";
    reg+=     "\"RegTime\":\"2022-10-30 12:20:35\",";
    reg+=     "\"ClientId\":\""+ WiFi.macAddress() +"\",";
    reg+=     "\"Commands\":[\"open\",\"close\",\"\"]";
    reg+=   "}";
    reg+= "}";
    return reg;
}
