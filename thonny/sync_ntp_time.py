import ntptime
def sync_ntp():
     ntptime.NTP_DELTA = 3155644800   # 可选 UTC+8偏移时间（秒），不设置就是UTC0
     ntptime.host = 'ntp1.aliyun.com'  # 可选，ntp服务器，默认是"pool.ntp.org"
     ntptime.settime()   # 修改设备时间,到这就已经设置好了

sync_ntp()
# 已经设置好了，随便用
def tuple2str(tup):
    s=''
    for item in tup:
        s = s+str(item)
    return s
    
from machine import RTC
rtc = RTC()
print(rtc.datetime())
print(tuple2str(rtc.datetime()))
