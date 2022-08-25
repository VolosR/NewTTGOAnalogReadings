#include "Arduino.h"
#include "TFT_eSPI.h"/* Please use the TFT library provided in the library. */
#include <WiFi.h>
#include "time.h"


TFT_eSPI lcd = TFT_eSPI();
TFT_eSprite sprite = TFT_eSprite(&lcd);



const char* ssid     = "IGK20";
const char* password = "18072019";

const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec =3600;
const int   daylightOffset_sec = 3600;

int values[24]={0};
int values2[24]={0};
char timeHour[3]="00";
char timeMin[3]="00";
char timeSec[3];

char m[12];
char y[5];
char d[3];
char dw[12];

int gw=204;
int gh=102;
int gx=110;
int gy=144;
int curent=0;
#define gray 0x6B6D
#define blue 0x0967
#define orange 0xC260
#define purple 0x604D
#define green 0x1AE9




int deb=0;

int Mode=0;

#define RIGHT 14





void setup(void)
{
 

 

    pinMode(RIGHT,INPUT_PULLUP);

    Serial.begin(115200);
    lcd.init();
    lcd.fillScreen(TFT_BLACK);
    lcd.setRotation(1);
    
   WiFi.begin(ssid, password);
   while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected.");
   configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);

    sprite.createSprite(320, 170);
    sprite.setTextDatum(3);
    sprite.setSwapBytes(true);
    values[23]=gh/2;
    analogReadResolution(10);
}

int counter=0;
int Min=gh/2;
int Max=gh/2;
int average=0;
String minT="";
String maxT="";

long lastMillis=0;
int fps=0;

void loop()
{
  if(digitalRead(RIGHT)==0){
    
  if(deb==0)
  {Mode++;
  
  if(Mode==2)
  Mode=0;
  deb=1;
   
  }}else{deb=0;}
  
  fps=1000/(millis()-lastMillis);
  lastMillis=millis();
  
  average=0;
  if(counter==0)
  printLocalTime();

  counter++;
  if(counter==50)
  counter=0;

if(Mode==0){
 if(values[23]>12)
 curent=random(values[23]-12,values[23]+12);
 else
 curent=random(1,values[23]+14);

 if(curent>gh)
 curent=random(gh-10,gh);
}
 if(Mode==1){
 int num=analogRead(44);
 curent=map(num,0,1024,0,gh);
 }

 for(int i=0;i<24;i++)
 values2[i]=values[i];

 for(int i=23;i>0;i--)
 values[i-1]=values2[i];
 
 values[23]=curent;
 if(values[23]>Max){
 Max=values[23];
 maxT=String(timeHour)+":"+String(timeMin)+":"+String(timeSec);}
  if(values[23]<Min){
  Min=values[23];
  minT=String(timeHour)+":"+String(timeMin)+":"+String(timeSec);}
 
  for(int i=0;i<24;i++)
  average=average+values[i];
  average=average/24;

  sprite.fillSprite(TFT_BLACK);
 // sprite.setTextDatum(4);
  sprite.setTextColor(TFT_WHITE,blue);
  sprite.fillRoundRect(6,5,38,32,4,blue);
  sprite.fillRoundRect(52,5,38,32,4,blue);
  sprite.fillRoundRect(6,42,80,12,4,blue);
  sprite.fillRoundRect(6,82,78,76,4,purple);
  sprite.fillRoundRect(6,58,80,18,4,green);
  sprite.drawString(String(timeHour),10,24,4);
  sprite.drawString(String(timeMin),56,24,4);
   sprite.drawString(String(m)+" "+String(d),10,48);
   //sprite.drawString(String(dw)+", "+String(y),10,58);

    
  //sprite.drawString(String(analogRead(18)),gx+160,26);
  
  sprite.drawString(String(timeSec),gx-14,14,2);
  sprite.setTextColor(TFT_WHITE,purple);
  sprite.drawString("CURR: "+String(average),10,92,2);
  
  sprite.drawString("MIN: "+String(Min),10,108,2);
  
  sprite.drawString("MAX: "+String(Max),10,138,2);
  
  
  
 
  
  
  sprite.setTextColor(TFT_SILVER,purple);
  sprite.drawString(String(maxT),10,152);
  sprite.drawString(String(minT),10,122);
  sprite.setTextColor(TFT_WHITE,green);
  sprite.drawString("SPEED:"+String(fps)+" fps",10,68);
  sprite.setTextColor(TFT_YELLOW,TFT_BLACK);
   sprite.drawString("ANALOG READINGS",gx+10,16,2);
   sprite.drawString("ON PIN 12",gx+10,30);
  sprite.setFreeFont();


  for(int i=1;i<12;i++){
  sprite.drawLine(gx+(i*17),gy,gx+(i*17),gy-gh,gray);
  if(i*17%34==0)
  if(i*2<10)
  sprite.drawString("0"+String(i*2),gx+(i*17)-3,gy+8);
  else
  sprite.drawString(String(i*2),gx+(i*17)-4,gy+8);
  }
  for(int i=1;i<6;i++){
  sprite.drawLine(gx,gy-(i*17),gx+gw,gy-(i*17),gray);
  sprite.drawString(String(i*17),gx-16,gy-(i*17));
  }

    sprite.drawLine(gx,gy,gx+gw,gy,TFT_WHITE);
    sprite.drawLine(gx,gy,gx,gy-gh,TFT_WHITE);

   for(int i=0;i<23;i++){
 sprite.drawLine(gx+(i*17),gy-values[i],gx+((i+1)*17),gy-values[i+1],TFT_RED);
 sprite.drawLine(gx+(i*17),gy-values[i]-1,gx+((i+1)*17),gy-values[i+1]-1,TFT_RED);}

  sprite.setTextColor(TFT_WHITE,TFT_BLACK);
  sprite.drawString("BAT:"+String(analogRead(4)),gx+160,16); 
  sprite.drawString("MOD:"+String(Mode),gx+160,26); 
 
  sprite.pushSprite(0,0);
 // delay(5);
  } 



  void printLocalTime(){
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
    Serial.println("Failed to obtain time");
    return;
  }
  
  strftime(timeHour,3, "%H", &timeinfo);
  strftime(timeMin,3, "%M", &timeinfo);
  strftime(timeSec,3, "%S", &timeinfo);
  strftime(y,5, "%Y", &timeinfo);
  strftime(m,12, "%B", &timeinfo);

  strftime(dw,10, "%A", &timeinfo);
   

   
   strftime(d,3, "%d", &timeinfo);

}
