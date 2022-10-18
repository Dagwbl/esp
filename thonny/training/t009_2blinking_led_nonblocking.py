import machine
import time

led = machine.Pin(3,machine.Pin.OUT)

start_time = time.ticks_ms()
interval = 500 # 500ms
led_status = 0
while True:
    if time.ticks_ms()-start_time>=interval:
        start_time = time.ticks_ms()
        if led_status ==1:
            led_status=0
        else:
            led_status=1
        led.value(led_status)
        