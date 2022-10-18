import machine
import time

led = machine.Pin(22,machine.Pin.OUT)

counter=0

while counter<5:
    led.off()
    time.sleep(0.5)
    led.on()
    time.sleep(0.5)
    counter +=1

print('Blinking LED is complete')