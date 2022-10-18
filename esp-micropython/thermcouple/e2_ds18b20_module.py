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
# max6676 适配热电偶
from max6675_hw import MAX6675
# onewire,ds18x20 适配ds18b20传感器
import onewire, ds18x20

# config network
def connect_network():
    # there may be a problem here.
    sta = network.WLAN(network.STA_IF)
    ssid = 'LonelyGirl'
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

def probe_data():
    # config max6675
    so = Pin(12, Pin.IN)
    sck = Pin(14, Pin.OUT)
    cs = Pin(16, Pin.OUT)
    max6675 = MAX6675(sck, cs , so)
    
    attempts=0
    success=False
    while attempts<10 and not success:
        try:
            #print("Measuring...\n")
            thermcouple = max6675.read()
            ds_sensor.convert_temp()
            ds18b20 = ds_sensor.read_temp(roms[0])
            success=True
            #print("Measuring successful. \n")
            return thermcouple,ds18b20,0
        except OSError:
            print("OSError: [Errno 116] ETIMEDOUT\nAttempt to remeasure.\n")
            attempts += 1
            success=False
    return null,null,null

def upload_data():
    id_num = 0
    while True:
        #time_stamp = get_time() # get time
        # ready to upload data
        id_num += 1
        thermcouple,lm35dz,vin=probe_data()
        
        print('\r{%d} {%.2f} {%.2f} {%d}'%(id_num,thermcouple,lm35dz,vin),end="")
        header={"Content-Type":"application/json"}
        data=json.dumps({"api_key":"tPmAT5Ab3j7F9","vin":vin,"lm35dz":lm35dz, "thermcouple":thermcouple, "probetime":time.time(), "seq":id_num,"flag":flag}).encode()
        #data = json.dumps({"api_key":"tPmAT5Ab3j7F9","lm35dz":123.1, "thermcouple":21.43, "probetime":"23423", "seq":1}).encode()
        # start upload
        if upload_flag:
            try:
                _response = requests.post("http://192.168.1.110/projects/thermcouple/post-esp-data.php", headers=header, data=data)
                print(_response.text)
                print(" upload on",end="")
            except OSError as e:
                print("Suspected server is not open!\n")
                print(e)
            # Interval time
            time.sleep(0.5)
            
        else:
            print(" upload off  ",end="")
            time.sleep(0.5)
        

if __name__=='__main__':
    flag = input("输入实验编号(日期-序号-t): ")
    upload_flag=True
    ds_pin = machine.Pin(4)
    ds_sensor = ds18x20.DS18X20(onewire.OneWire(ds_pin))
    roms = ds_sensor.scan()
    #upload_flag=True
    rtc = RTC()
    # config network
    connect_network()
    # sensor initialization
    upload_data()
    
    



