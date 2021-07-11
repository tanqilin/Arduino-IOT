#!/usr/bin/env python
# -*- coding: utf-8 -*-
import os
from Blinker import Blinker, BlinkerButton, BlinkerNumber, BlinkerRGB,BlinkerSlider
from Blinker.BlinkerDebug import *
from WS2812B.ws2812b import displayLed
from EMQX.client import MqttClient

mqttc = MqttClient()

auth = '83f4d2e9da48'

BLINKER_DEBUG.debugAll()

Blinker.mode("BLINKER_WIFI")
Blinker.begin(auth)

button1 = BlinkerButton("btn-abc")
button2 = BlinkerButton("btn-frpc")
rgbColor = BlinkerRGB("col-fzk")
ledSlider = BlinkerSlider("ran-vkg")

led = displayLed(60,18)

def button1_callback(state):
    """ """
    print(state)
    BLINKER_LOG('get button state: ', state)
    if state == "on":
        led.openLed()
        button1.icon('fad fa-camera-home')
        button1.color('#FFFFFF')
        button1.text('已打开')
        button1.print("on")
    else:
        button1.icon('fad fa-camera-home')
        button1.color('#FFFFFF')
        button1.text('已关闭')
        button1.print("off")
        led.clearLed()

def button2_callback(state):
    os.popen("sudo service frpc restart")
    button2.text('服务已重启')
    button2.print(state)

# 颜色
def rgbColor_callback(r_value, g_value, b_value, bright_value):
    led.setColor(r_value,g_value,b_value)
    led.theaterChase(r_value,g_value,b_value)
    
#滑块
def slider_callback(value):
    """ 无效 """
    led = displayLed(value,18)

# 设备收到APP发来数据时的回调
def data_callback(data):
    global counter
    
    BLINKER_LOG("Blinker readString: ", data)

button1.attach(button1_callback)
button2.attach(button2_callback)
rgbColor.attach(rgbColor_callback)
ledSlider.attach(slider_callback)

Blinker.attachData(data_callback)

if __name__ == '__main__':
    while True:
        Blinker.run()

