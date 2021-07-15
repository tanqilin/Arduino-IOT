import sys
import json
import paho.mqtt.client as mqtt
sys.path.append("..")
from WS2812B.ws2812b import displayLed

class MqttClient(mqtt.Client):
    username = None
    host = '0.0.0.0'
    port = 1883
    led = displayLed(60,18) # 18,21,12

    def set_client_data(self,username = None,host = '0.0.0.0',port=1883):
        self.username = username
        self.host = host
        self.port = port 
        
    def on_connect(self, mqttc, obj, flags, rc):
        print("链接成功: "+str(rc))
        self.led.openLed()
    
    def on_disconnect(self, mqttc, obj,rc):
        self.led.clearLed()
    
    def on_message(self, mqttc, obj, msg):
        print(msg.topic+" "+str(msg.qos)+" "+str(msg.payload))
        if msg.topic == "tanqilin/out@center":
            text = json.loads(msg.payload)
            print(text)
            if text['order'] == 0:
                self.led.openLed()
            elif text['order'] == 1:
                self.led.clearLed()
            elif text['order'] == 2:
                self.led.setColor(text['r'],text['g'],text['b'])
        self.publish('tanqilin/in@center',payload='order run successfuly!',qos=0)
        
    def on_publish(self, mqttc, obj, mid):
        print("mid: "+str(mid))

    def on_subscribe(self, mqttc, obj, mid, granted_qos):
        print("Subscribed: "+str(mid)+" "+str(granted_qos))

    def on_log(self, mqttc, obj, level, string):
        print(string)

    def run(self):
        self.username_pw_set(self.username)
        self.connect(self.host,self.port, 60)
        self.subscribe("tanqilin/out@center", 0)

        rc = 0
        while rc == 0:
            rc = self.loop()
        return rc


if __name__ == '__main__':
    mqttc = MqttClient("iot-raspberry-4b")
    mqttc.set_client_data('raspberry 4b')
    rc = mqttc.run()
