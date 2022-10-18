import network
import urequests
import json
import time
from machine import Pin
import dht
import ntptime
from machine import RTC


#时间同步函数
def sync_ntp():
     ntptime.NTP_DELTA = 3155644800   # 可选 UTC+8偏移时间（秒），不设置就是UTC0
     ntptime.host = 'ntp1.aliyun.com'  # 可选，ntp服务器，默认是"pool.ntp.org"
     try:
         ntptime.settime()   # 修改设备时间,到这就已经设置好了
     except OSError as e:
         print(e)
# 已经设置好了，随便用

def tuple2str(tup):
    '''时间转换为字符串'''
    s=''
    for item in tup:
        s = s+str(item)
    return s
    

#获取并转换时间
def get_time():
    '''从阿里云授时服务获取准确的网络时间，无法获取时尝试一定次数后若还是失败则返回芯片的默认时间'''
    attempts=0
    while attempts<3:
        try:
            rtc_stamp = tuple2str(rtc.datetime())
            return rtc_stamp
        except OSError as e:
            print(e)
            attempts+=1
            time.sleep(1)
    return time.ticks_ms()

# config network
def connect_network():
    # there may be a problem here.
    sta = network.WLAN(network.STA_IF)
    ssid = b'\xe5\xaf\x82\xe5\xaf\x9e\xe5\xb0\x91\xe5\xa5\xb3\xe5\x9c\xa8\xe7\xba\xbf\xe6\xb1\x82\xe6\x92\xa9'
    password = '19961226'
    if not sta.isconnected():
        print('connecting to network...')
        sta.active(True)
        sta.scan()
        sta.connect(ssid,password)
        sta.ifconfig()
        while not sta.isconnected():
            pass
    print('Successful connection!\nnetwork config:', sta.ifconfig())

def probe_data(pin):
    d = dht.DHT11(Pin(pin))
    print("Probeing...\n")
    attempts=0
    success=False
    while attempts<10 and not success:
        try:
            print("Measuring...\n")
            d.measure()
            success=True
            print("Measuring successful. \n")
            return d.temperature(),d.humidity()
        except OSError:
            print("OSError: [Errno 116] ETIMEDOUT\nAttempt to remeasure.\n")
            time.sleep(2)
            attempts += 1
            success=False
    return null,null

def upload_data():
    id_num = 0
    while True:
        #time_stamp = get_time() # get time
        # ready to upload data
        temperature,humidity=probe_data(1)
        id_num += 1
        # sensor information
        sensor = 'DHT11'
        coords = '2'
        try:
            _response = urequests.post("http://192.168.1.110/esp32/post-esp-data.php/", \
                                       headers={"Content-Type":"application/json"}, \
                                       data=json.dumps({"api_key":"tPmAT5Ab3j7F9", "sensor":sensor, "coords":coords, "temperature":str(temperature), "humidity":str(humidity), "probetime":str(tuple2str(time.localtime())), "id":str(id_num)}))
            print(_response.text)    
        except OSError as e:
            print("Suspected server is not open!\n")
            print(e)
        # Interval time
        time.sleep(3)

if __name__=='__main__':
    # setting time
    #sync_ntp()
    #rtc = RTC()
    # config network
    connect_network()
    # sensor initialization
    upload_data()
    
    


