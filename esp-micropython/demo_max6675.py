from max6675_hw import MAX6675
from machine import Pin,ADC
import time

so = Pin(12, Pin.IN)
sck = Pin(14, Pin.OUT)
cs = Pin(16, Pin.OUT)

max = MAX6675(sck, cs , so)

adc = ADC(0)   # create an ADC object acting on a pin


while True:

    val = adc.read_u16()     # read a raw analog value in the range 0-65535
    sigma = 1048/1024
    val = val*3.3/1024*sigma
    
    print(max.read(),val)
    time.sleep(0.5)
    