'''
手动控制测试
'''
import network
import urequests as requests
import json
import time
from machine import Pin,ADC
import ntptime
from machine import RTC

# 配置基础参数
NODE = 7
SENSOR = "thermcouple-1"
COORDS = "435"
SSID = 'LonelyGirl'
PASSWORD = '19961226'
API = "http://192.168.1.110:80/projects/learn/esp/api/node.php?action=query"
UNIT = "Aq"
OPT_STATUS = False

# 远程控制板载小灯实验
LED0 = Pin(5,Pin.OUT)

# config network
def connect_network():
    # there may be a problem here.
    sta = network.WLAN(network.STA_IF)
    if not sta.isconnected():
        print('connecting to network...')
        sta.active(True)
        sta.scan()
        sta.connect(SSID,PASSWORD)
        sta.ifconfig()
        while not sta.isconnected():
            pass
    print('Successful connection!\nnetwork config:', sta.ifconfig())

def control(flag):
    if int(flag):
        print(type(flag))
        print("打开小灯")
        LED0.on()
    else:
        print("关闭小灯")
        LED0.off()
        
        
def fetch_data():
    id_num = 0
    while True:
        header={"Content-Type":"application/x-www-form-urlencoded"}
        data="coords="+COORDS 
        print(data)
        # start upload
        if upload_flag:
            try:
                _response = requests.post(API, headers=header, data=data)
                print(_response.text)
                #print(" upload on",end="")
                json_str = json.loads(_response.text)
                flag = json_str[0]['optocouple']
                control(flag)
            except OSError as e:
                print("Suspected server is not open!\n")
                print(e)
            # Interval time
            time.sleep(1)
        else:
            print(" upload off  ",end="")
            time.sleep(0.5)
        

if __name__=='__main__':
    # flag = input("输入实验编号(日期-序号-t): ")
    upload_flag=True
    adc = ADC(0)
    #upload_flag=True
    rtc = RTC()
    # config network
    connect_network()
    # sensor initialization
    fetch_data()
    
    





