#include "SSD1306Wire.h" // legacy include: `#include "SSD1306.h"`
#include <ESP8266WiFi.h>

SSD1306Wire  display(0x3c, D2, D3);

u8 pon[]={0x42,0x4d,0xe4,0x0,0x1,0x1,0x74};
u8 poff[]={0x42,0x4d,0xe4,0x0,0x0,0x1,0x73};

void poweron()
{
//  while(Serial.read() >= 0){}
  Serial.write(pon,7);
  Serial.flush();
}
void standby()
{
//  while(Serial.read() >= 0){}
  Serial.write(poff,7);
  Serial.flush();
}

uint32_t pm25;
uint32_t pm1;
uint32_t pm10;
uint32_t errors=0;
uint32_t corrects=0;

void setup() {
  display.init();
//  display.flipScreenVertically();   //反转显示
  display.setBrightness(200);
  display.setFont(ArialMT_Plain_16);
  display.drawString(0,0,"AirIndex:Start");
  display.display();
  Serial.begin(9600);
  delay(100);
}

void loop() {
    byte buffer[32];
    int numdata=0;
    int i=0;
    display.clear();
//    numdata=Serial.readBytesUntil('\n',buffer,32);
    while(Serial.read() >= 0){}
    while(Serial.available()<32){
      delay(1);
    }
    numdata=Serial.readBytes(buffer,32);
    if(numdata==32 && buffer[0]==0x42 && buffer[1]==0x4d){
      display.setLogBuffer(5, 30);//设置显示几行文字
      corrects++;
      pm1=buffer[8]*256+buffer[9];
      pm25=buffer[10]*256+buffer[11];
      pm10=buffer[12]*256+buffer[13];

      display.println("PM1.0="+(String)pm1);
      display.println("PM2.5="+(String)pm25);
      display.println("PM 10="+(String)pm10);
      display.drawLogBuffer(0, 0);
      display.display();
      delay(1000);
    }
    else
    {
      errors++;
      String aaa="";
      for(i=0;i<32;i++){
        aaa=aaa+(String)buffer[i]+"/";
      }
      aaa=(String)numdata+"/"+aaa;
      Serial.println(aaa);
      display.drawString(0,0,"Error:"+(String)errors);
      display.display();
      while(Serial.read() >= 0){}
    }
    delay(110);
 }
