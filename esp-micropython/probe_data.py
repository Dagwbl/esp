'''
探测采集数据
'''
import time
from machine import Pin
import dht

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

if __name__=='__main__':
    print(probe_data(5))