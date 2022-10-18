import machine
import time

led = machine.Pin(22,machine.Pin.OUT)
def blink_led_ntimes(num,t_on,t_off,msg):
    counter=0

    while counter<num:
        led.off()
        time.sleep(t_on)
        led.on()
        time.sleep(t_off)
        counter +=1

    print(msg)
