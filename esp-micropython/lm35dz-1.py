from machine import Pin,ADC

lm35Pin = machine.Pin(0)

adc = ADC(0)   # create an ADC object acting on a pin
val = adc.read_u16()     # read a raw analog value in the range 0-65535
# val = lm35Pin.value()
sigma = 1048/1024
val = val*3.3/1024*sigma
print(val)

print(0)