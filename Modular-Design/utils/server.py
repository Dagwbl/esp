import time
import network
import lib.urequests as requests
import json

class Server:
    HEADER = {"Content-Type": "application/json"}
    def __init__(self, MODEL, SSID, PASSWORD, POST_API, CONTROL_API):
        self.ssid = SSID
        self.password = PASSWORD
        self.post_api = POST_API
        self.control_api = CONTROL_API
        self.sta = self.connect()

    def connect(self):
        '''
        链接网络
        '''
        sta = network.WLAN(network.STA_IF)
        sta.active(True)
        print('connecting to network...')
        sta.connect(self.ssid, self.password)
        while not sta.isconnected():
            print("Waiting for connection\n")
            time.sleep(1)
                 
        print('Successful connection!\nnetwork config:', sta.ifconfig())
        return sta

    def upload(self, data):
        '''上传数据'''
#         verify = "L"+self.coords+"T"+str(time.time())
#         data = "value="+str(value)+"&unit="+self.unit+"&sensor=" + self.sensor+"&raw="+str(raw)+"&verify="+verify
        res = requests.post(self.post_api, headers=Server.HEADER, data=json.dumps(data))
        return res
            

    def download(self):
        """下载（获取）服务端的控制状态代码"""
        data = "coords="+self.coords
        # print(data)
        res = requests.post(self.control_api, headers=Server.HEADER, data=data)
        print("下载（获取）服务端的控制状态代码",res.text)
        # 如果状态代码为空，请检查坐标设置是否正确
        cmd = json.loads(res.text)[0]['optocouple']
        return int(cmd)
