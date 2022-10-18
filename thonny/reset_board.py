import machine

#reset_button = machine.Pin(machin,machine.Pin.OUT)
#reset.value()
#machine.SOFT_RESET()
invalid_pins = []
for pin in range(0,50):
    try:
        p = machine.Pin(pin)
        print(pin,'\t',p.value())
    except ValueError:
        invalid_pins.append(pin)
print('Invalid pins: ',invalid_pins)