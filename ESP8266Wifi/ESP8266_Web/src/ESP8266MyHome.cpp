#include "ESP8266MyHome.h"

ESP8266MyHome::ESP8266MyHome(){

}

ESP8266MyHome::~ESP8266MyHome(){
    
}

ESP8266MyHome& ESP8266MyHome::setCmdCallback(MQTT_CALLBACK_COMMAND) {
    this->callback = callback;
    return *this;
}