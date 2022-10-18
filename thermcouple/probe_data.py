'''
热电偶温度采集
'''
from machine import Pin,ADC
import max6675_hw


# while True:
#     val = adc.read_u16()     # read a raw analog value in the range 0-65535
#     # val = optPin.value()
#     # sigma = 1048/1024
#     # val = val*3.3/1024*sigma
#     print(val)
#     print("-----------")
#     time.sleep(1)
def probe_data():
    # config
    
    attempts=0
    success=False
    while attempts<10 and not success:
        try:
            val = adc.read_u16()
            return val
        except OSError:
            print("OSError: [Errno 116] ETIMEDOUT\nAttempt to remeasure.\n")
            attempts += 1
            success=False
    return null

