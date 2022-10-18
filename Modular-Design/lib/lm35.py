from machine import ADC,Pin

class LM35:
    def __init__(self, pin):
        # vcc 主板电压
        self.vcc = 3.3
        self.max_Analog = 1024
        self.pin = pin

    def read(self):
        x = self.pin.read_analog()
        return x*self.vcc*100/self.max_Analog

if __name__ == "__main__":
    t = LM35(10)
    while True:
        print(t.read())
        sleep(1000)