import time
from utils.server import Server
from utils.prober import Prober
from utils.controler import Controler
import machine

# time.sleep(3)
# machine.reset() # 每次运行重启以清空变量，特别危险，容易导致无限重启
# 基础信息配置 后续只需更改此处代码即可实现复用
COORDS = "gas" #node coords

MODEL = "opt101"  # thermocouple opt101 ds18b20 lm35dz fxfs
SENSOR = COORDS + "-" + MODEL + "-1"
UNIT = "Aq"  #Aq Celsius

SSID = 'LonelyGirl'
PASSWORD = '19961226'
BASE_URL = "http://192.168.1.110:80/projects/learn/esp/api"
POST_API = BASE_URL + "/data.php?action=insert"
CONTROL_API = BASE_URL + "/node.php?action=query"

# 引脚设置，2号引脚为板载灯，默认高电平 
LED_PIN = 2
# 5号引脚为光耦控制引脚
OPT_PIN = 5

# 实例化Controler
print("实例化Controler")
controler = Controler(LED_PIN,OPT_PIN)
# 实例化Server
print("实例化Server")
server = Server(MODEL,SSID,PASSWORD,POST_API,CONTROL_API)
# 实例化Prober
prober = Prober(MODEL,COORDS,SENSOR,UNIT)
# 连接网络, 在实例化server的时候就已经连接网络，不用再重新设置
#print("连接网络")
#server.connect()

def run():
    while True:
        print(".----------")
        # 采集数据，在循环中调用
        data = prober.read()
        # 上传数据，在循环中调用
        res = server.upload(data)
        print(res.text)
        # 信号指示
        controler.tips()
        # 获取控制状态, 只有携带热电偶的node才执行下面的代码
        if MODEL == 'thermocouple':
            opt_status = server.download()
            controler.exec(opt_status)
        # print(time.time())
        time.sleep(0.5)
        print("----------.")
        
        
# 启动入口
if __name__=='__main__':
    run()