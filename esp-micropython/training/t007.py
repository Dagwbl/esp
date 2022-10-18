import machine
import time

p = machine.Pin(4,machine.Pin.OUT)
buzzer = machine.PWM(p)

buzzer.freq(1047)

buzzer.duty(50)

time.sleep(1)

buzzer.duty(0)

buzzer.deinit()
