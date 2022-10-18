import machine

led_g = machine.Pin(4,machine.Pin.OUT)
sw = machine.Pin(9,machine.Pin.IN)

def handler_interrupt(pin):
    led_g.value(not led_g.value())
sw.irq(trigger=machine.Pin.IRQ_FALLING,handler=handler_interrupt)