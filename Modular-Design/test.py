import lib.onewire, lib.ds18x20
from lib.max6675_hw import MAX6675
from machine import Pin, ADC
import time

fx_adc = ADC(Pin(35),atten=ADC.ATTN_11DB)
fs_adc = ADC(Pin(39),atten=ADC.ATTN_11DB)
while True:
    time.sleep(1)
    fx_raw = fx_adc.read_uv()/1000
    fx = (fx_raw-142)/(2500-142)*360
    
    fs_raw = fs_adc.read_uv()/1000
    fs = (fs_raw-142)/1000*12
    print(fx,"\t ",fs)