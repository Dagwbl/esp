from connect_network import connect_network
import urequests
import json
import time
import machine
import ini

connect_network()


led = machine.Pin(5,machine.Pin.OUT)
# print(type(cmd))
while True:
    cmd = urequests.post("http://192.168.1.110/projects/esp32/api/query.php",data="Hello, I am sensor 4")
    #cmd  = json.loads(cmd.text)
    #print(cmd.text)
    cmd = cmd.json()
    print(cmd[0][2])
    if cmd[0][2]=="0":
        led.off()
        print("Turn off the led!")
    elif cmd[0][2]=="1":
        led.on()
        print("Turn on the led!")
    time.sleep(1)

