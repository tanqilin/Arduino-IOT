#!/usr/bin/python
# -*- coding: utf-8 -*-
import time
from rpi_ws281x import PixelStrip, Color
import argparse

class displayLed:    
    
    def __init__(self,ledNum =60,pin =18):
        self.LED_COUNT = ledNum
        self.LED_PIN = pin
        
        # 以下可以不用改
        self.LED_FREQ_HZ = 800000  # LED signal frequency in hertz (usually 800khz)
        self.LED_DMA = 10          # DMA channel to use for generating signal (try 10)
        self.LED_BRIGHTNESS = 255  # Set to 0 for darkest and 255 for brightest
        self.LED_INVERT = False    # True to invert the signal (when using NPN transistor level shift)
        self.LED_CHANNEL = 0       # set to '1' for GPIOs 13, 19, 41, 45 or 53
        
        self.strip = PixelStrip(self.LED_COUNT, self.LED_PIN, self.LED_FREQ_HZ, self.LED_DMA, self.LED_INVERT, self.LED_BRIGHTNESS, self.LED_CHANNEL)
        # Intialize the library (must be called once before other functions).
        self.strip.begin()
    
    def wheel(self,pos):
        """生成横跨0-255个位置的彩虹颜色."""
        if pos < 85:
            return Color(pos * 3, 255 - pos * 3, 0)
        elif pos < 170:
            pos -= 85
            return Color(255 - pos * 3, 0, pos * 3)
        else:
            pos -= 170
            return Color(0, pos * 3, 255 - pos * 3)
        
    def theaterChase(self,r,g,b, wait_ms=50, iterations=10):
        """电影影院灯光风格的追逐动画."""
        strip = self.strip
        color = Color(r,g,b)
        for j in range(iterations):
            for q in range(3):
                for i in range(0, strip.numPixels(), 3):
                    strip.setPixelColor(i + q, color)
                strip.show()
                time.sleep(wait_ms / 1000.0)
                for i in range(0, strip.numPixels(), 3):
                    strip.setPixelColor(i + q, 0)
            
    def rainbow(self,wait_ms=20, iterations=1):
        """绘制彩虹，褪色的所有像素一次."""
        strip = self.strip
        for j in range(256 * iterations):
            for i in range(strip.numPixels()):
                strip.setPixelColor(i, self.wheel((i + j) & 255))
            strip.show()
            time.sleep(wait_ms / 1000.0)
    
    def rainbowCycle(self, wait_ms=10, iterations=5):
        """画出均匀分布在所有像素上的彩虹."""
        strip = self.strip
        for j in range(256 * iterations):
            for i in range(strip.numPixels()):
                strip.setPixelColor(i, self.wheel(
                    (int(i * 256 / strip.numPixels()) + j) & 255))
            strip.show()
            time.sleep(wait_ms / 1000.0)
        
    def openLed(self,wait_ms=20):
        strip = self.strip
        for j in range(256):
            for i in range(strip.numPixels()):
                strip.setPixelColor(i, self.wheel((i + j) & 255))
            strip.show()
            time.sleep(wait_ms / 1000.0)
    
    def clearLed(self,wait_ms=20):
        strip = self.strip
        color = Color(0,0,0)
        for i in range(strip.numPixels()):
            strip.setPixelColor(i, color)
            strip.show()
            time.sleep(wait_ms / 1000.0)
    
    def setColor(self,r,g,b, wait_ms=20):
        """一次擦除显示像素的颜色."""
        strip = self.strip
        color = Color(r, g, b)
        for i in range(strip.numPixels()):
            strip.setPixelColor(i, color)
            strip.show()
            time.sleep(wait_ms / 1000.0)
            
            
            