
import network
import machine

led = machine.Pin(3,machine.Pin.OUT)
led.off()


ssid = b'\xe5\xaf\x82\xe5\xaf\x9e\xe5\xb0\x91\xe5\xa5\xb3\xe5\x9c\xa8\xe7\xba\xbf\xe6\xb1\x82\xe6\x92\xa9'
password = '19961226'

# config network
sta = network.WLAN(network.STA_IF)
if not sta.isconnected():

    print('connecting to network...')
    sta.active(True)
    sta.scan()
    sta.connect(ssid,password)
    sta.ifconfig()
    while not sta.isconnected():
        pass
print('network config:', sta.ifconfig())

# Configure the socket connection over TCP/IP
import socket
# AF_INET - use Internet Protocol v4 addresses
# SOCK_STREAM means that it is a TCP socket.
# SOCK_DGRAM means that it is a UDP socket.
s = socket.socket(socket.AF_INET,socket.SOCK_STREAM)
s.bind(('',80))
s.listen(5)

# **************
# Function for creating the web page to be dispalyed
def web_page():
    if isLedBlinking==True:
        led_state = 'Blinking'
        print('led is Blinking')
    else:
        if led.value()==1:
            led_state = 'ON'
            print('led is ON')
        elif led.value()==0:
            led_state = 'OFF'
            print('led is OFF')

    html_page = """    
    <html>    
    <head>    
     <meta content="width=device-width, initial-scale=1" name="viewport"></meta>    
    </head>    
    <body>    
     <center><h2>ESP32 Web Server in MicroPython </h2></center>    
     <center>    
      <form>    
      <button name="LED" type="submit" value="1"> LED ON </button>    
      <button name="LED" type="submit" value="0"> LED OFF </button>  
      <button name="LED" type="submit" value="2"> LED BLINK </button>   
      </form>    
     </center>    
     <center><p>LED is now <strong>""" + led_state + """</strong>.</p></center>    
    </body>    
    </html>"""  
    return html_page   


tim0 = machine.Timer(0)
def handle_callback(timer):
    led.value( not led.value() )
isLedBlinking = False

while True:
    
    # Socket accept() 
    conn, addr = s.accept()
    print("Got connection from %s" % str(addr))
    
    # Socket receive()
    request=conn.recv(1024)
    print("")
    print("")
    print("Content %s" % str(request))

    # Socket send()
    request = str(request)
    led_on = request.find('/?LED=1')
    led_off = request.find('/?LED=0')
    led_blink = request.find('/?LED=2')
    if led_on == 6:
        print('LED ON')
        print(str(led_on))
        led.value(1)
        if isLedBlinking==True:
            tim0.deinit()
            isLedBlinking = False
        
    elif led_off == 6:
        print('LED OFF')
        print(str(led_off))
        led.value(0)
        if isLedBlinking==True:
            tim0.deinit()
            isLedBlinking = False
        
    elif led_blink == 6:
        print('LED Blinking')
        print(str(led_blink))
        isLedBlinking = True
        tim0.init(period=500, mode=machine.Timer.PERIODIC, callback=handle_callback)
        
    response = web_page()
    conn.send('HTTP/1.1 200 OK\n')
    conn.send('Content-Type: text/html\n')
    conn.send('Connection: close\n\n')
    conn.sendall(response)
    
    # Socket close()
    conn.close()