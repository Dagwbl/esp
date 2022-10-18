import time
import lib.onewire, lib.ds18x20
from lib.max6675_hw import MAX6675
from machine import Pin, ADC


class Prober:
    """采集数据并将数据返回"""
    def __init__(self, MODEL,COORDS,SENSOR,UNIT):
        self.coords = COORDS
        self.sensor = SENSOR
        self.unit = UNIT
        self.model = MODEL
        self.data = []
        self.value = -1
        self.raw = -1
        if self.model == 'thermocouple':
            self.so = Pin(12, Pin.IN)
            self.sck = Pin(14, Pin.OUT)
            self.cs = Pin(16, Pin.OUT)
            self.max6675 = MAX6675(self.sck, self.cs, self.so)
        elif self.model == 'opt101':
            self.adc = ADC(0)  # create an ADC object acting on a pin
        elif self.model == 'ds18b20':
            self.ds_pin = Pin(4)
            self.ds_sensor = ds18x20.DS18X20(onewire.OneWire(self.ds_pin))
            self.roms = self.ds_sensor.scan()
        elif self.model == 'lm35dz':
            self.adc = ADC(0)
        elif self.model == 'fxfs':
            self.fx_adc = ADC(Pin(35),atten=ADC.ATTN_11DB)
            self.fs_adc = ADC(Pin(39),atten=ADC.ATTN_11DB)
            
    def read(self):
#         清空data
        self.data = []
        if self.model == 'thermocouple':
            self.thermocouple()
        elif self.model == 'opt101':
            self.opt101()
        elif self.model == 'ds18b20':
            self.ds18b20()
        elif self.model == 'lm35dz':
            self.lm35dz()
        elif self.model == "fxfs":
            self.fxfs()
            return self.data # 携带有两个参数，与其他不太一样提前返回
        else:
            print("传感器类型错误")
        verify = "L"+self.coords+"T"+str(time.time())
        self.data.append({"value":self.value, "raw":self.raw,"unit":self.unit,"sensor":self.sensor,"verify":verify})
        return self.data

    def thermocouple(self):
        self.raw = self.max6675.read()
        self.value = self.raw

    def opt101(self):
        print('opt101 start probing')
        attempts = 0
        success = False
        while attempts < 10 and not success:
            try:
                self.raw = self.adc.read_u16()
                self.value = self.raw/65535*100
                success = True
            except OSError:
                print("OSError: [Errno 116] ETIMEDOUT\nAttempt to remeasure.\n")
                attempts += 1
                success = False

    def pms7003(self):
        '''颗粒浓度数据在Arduino平台实现'''
        pass

    def ds18b20(self):
        self.ds_sensor.convert_temp()
        self.raw = self.ds_sensor.read_temp(self.roms[0])
        self.value = self.raw

    def lm35dz(self):
        self.raw = self.adc.read_u16()
        sigma = 1048/1024
        self.value = self.raw*3.3/1024*sigma
    
    def fxfs(self):
        fx_raw = self.fx_adc.read_uv()/1000
        fx = (fx_raw-142)/(2500-142)*360
        fs_raw = self.fs_adc.read_uv()/1000
        fs = (fs_raw-142)/1000*12
        verify = "L"+self.coords+"T"+str(time.time())
        self.data.append({"value":fx,"raw":fx_raw,"unit":"angle","sensor":"vms-fx","verify":verify})
        self.data.append({"value":fs,"raw":fs_raw,"unit":"m/s","sensor":"vms-fs","verify":verify})
        
        
