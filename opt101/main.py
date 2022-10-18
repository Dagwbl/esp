'''
ds18b20与热电偶的精度对比实验，导入了其他模块，确保所有模块存在。
下面模块分别为网络连接network、发送请求urequests、返回数据处理json，
其他的就是micropython 自带的time和machine模块。
'''
import network
import urequests as requests
import json
import time
from machine import Pin,ADC
import ntptime
from machine import RTC
import opt101

# 配置基础参数
SENSOR = "opt101-1"
COORDS = "L435T"
SSID = 'LonelyGirl'
PASSWORD = '19961226'
API = "http://192.168.1.110:80/projects/learn/esp/api/data.php?action=insert"
UNIT = "Aq"
    
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


def upload_data():
    id_num = 0
    while True:
        #time_stamp = get_time() # get time
        # ready to upload data
        id_num += 1
        raw=opt101.probe_data()
        value = raw/65535*100
        verify = COORDS+str(time.time())
        header={"Content-Type":"application/x-www-form-urlencoded"}
        data="value="+str(value)+"&unit="+UNIT+"&sensor="+SENSOR+"&raw="+str(raw)+"&verify="+verify 
        print(data)
        # start upload
        if upload_flag:
            try:
                _response = requests.post(API, headers=header, data=data)
                print(_response.text)
                print(" upload on",end="")
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
    upload_data()
    
    




