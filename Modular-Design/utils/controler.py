import machine


class Controler:
    def __init__(self, lED_PIN, OPT_PIN):
        self.led = machine.Pin(lED_PIN, machine.Pin.OUT)
        self.led_status = True
        self.optocouple = machine.Pin(OPT_PIN, machine.Pin.OUT)
        self.optocouple_status = False
        self.tips()

    def tips(self):
        '''用于上传数据的时候不断闪烁更新'''
        self.led_status = not self.led_status
        self.led.value(self.led_status)

    def exec(self, cmd):
        '''
        接收到控制信号执行控制
        这里更应该探讨光耦是置于常开还是常闭的状态，这将影响到当电力系统中断的时候系统的有效性
        '''
        if cmd:
            self.optocouple.on()
            print("光耦通道开启")
        else:
            self.optocouple.off()
            print("光耦通道关闭")


  