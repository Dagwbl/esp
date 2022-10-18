import network
import urequests
import json
import time
from machine import Pin
import dht
import ntptime
from machine import RTC


def connect_network():
    # there may be a problem here.
    sta = network.WLAN(network.STA_IF)
    ssid = 'LonelyGirl'
    password = '19961226'
    sta.active(True)
    if not sta.isconnected():
        print('connecting to network...')
        sta.connect(ssid,password)
        while not sta.isconnected():
            print("\nERROR: Connection failed!\n")
            sta.connect(ssid,password)
            pass
    print('Successful connection!\nnetwork config:', sta.ifconfig())
    
if __name__=='__main__':
    connect_network()