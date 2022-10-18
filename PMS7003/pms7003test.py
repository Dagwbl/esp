#import micropython
from pms7003 import PassivePms7003


def do_work(__):
    print('wakeup\n')
    pms.wakeup()
    print('start\n')
    pms_data = pms.read()
    print(pms_data)
    print('Done\n')
    
    pms.sleep()

# usually performing readings in interrupt handler (e.g. Timer's)
# so use schedule to avoid heap lock limitations:
# https://docs.micropython.org/en/latest/reference/isr_rules.html
#callback = lambda __: micropython.schedule(do_work, 0)

while True:
    print('begin')
    pms = PassivePms7003(uart=0)

    do_work()
    time.sleep(3000)
    