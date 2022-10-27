#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <FS.h>

const char *CONFIG_IP= "192.168.4.1";

const char *ssid     = "ESP8266-WEB";       //这里写入网络的ssid
const char *password = "00000000";            //wifi密码

ESP8266WebServer web_server(80);
WiFiClient client;

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
  
  Serial.print("Start listening on port 80 IP:");
  Serial.println(WiFi.localIP());
}

void loop() {
   web_server.handleClient();
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
    web_server.streamFile(file, contentType);// 并且将该文件返回给浏览器
    file.close();                                // 并且关闭文件
    return true;                                 // 返回true
  }
  return false;                                  // 如果文件未找到，则返回false
}

// 处理POST请求
bool handlePostRead(String path){
  if(path.endsWith("config")){
    String wifis = web_server.arg("ssid");
    String wifip = web_server.arg("password");
    Serial.println("收到配网信息，Wifi：" + wifis + ",密码：" + wifip);
    WiFi.begin(wifis,wifip);
  }

  if(path.endsWith("reset")){
    Serial.println("重置网络状态成功，Wifi：清空,密码：清空");
    // 把Wifi信息写入配置文件中
    File dataFile = SPIFFS.open("/config.json", "w");
    dataFile.println("");
    dataFile.close(); 
  }
  
  while(WiFi.status() != WL_CONNECTED){
     String configJson = "{";
            configJson  += "\"wifi\": {";
            configJson  +=      "\"ssid\": \"" + WiFi.SSID() + "\",";
            configJson  +=      "\"password\": \""+ WiFi.psk() +"\"";
            configJson  +=   "}";
            configJson  += "}";

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
