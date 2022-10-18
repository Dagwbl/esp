import machine

led = machine.Pin(6,machine.Pin.OUT)

tim0 = machine.Timer(0)

def handle_callback(timer):
    led.value(not led.value())
#tim0.init(period=2000,mode=machine.Timer.ONE_SHOT,callback=lambda t:led.value(not led.value()))
tim0.init(period=500,mode=machine.Timer.PERIODIC,callback=handle_callback)
