import machine
import time

led = machine.Pin(3,machine.Pin.OUT)

while True:
    led.off()
    time.sleep(0.5)
    led.on()
    time.sleep(0.5)