#ifndef ESP8266MYHOME_H
#define ESP8266MYHOME_H

#include "arduino.h"

#if defined(ESP8266) || defined(ESP32)
#include <functional>
#define MQTT_CALLBACK_COMMAND std::function<void(char*, uint8_t*, unsigned int)> callback
#else
#define MQTT_CALLBACK_COMMAND void (*callback)(char*, uint8_t*, unsigned int)
#endif

#define SUB_CTRL_TOPIC "/"+ WiFi.macAddress() +"/center/control";

class ESP8266MyHome
{
public:
    ESP8266MyHome();
    ~ESP8266MyHome();
    
    ESP8266MyHome& setCmdCallback(MQTT_CALLBACK_COMMAND);

private:
    const char * mq_ip;
    MQTT_CALLBACK_COMMAND;
};

#endif