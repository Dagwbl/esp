import machine
import time

yx = machine.Pin(5,machine.Pin.OUT)
yx.on()
time.sleep(2)
yx.off()
