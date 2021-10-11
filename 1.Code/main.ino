#include <Arduino.h>  
#include <openGLCD.h> //https://github.com/thegeek82000/openGLCD
#include <TimeLib.h>  //https://github.com/PaulStoffregen/Time
#include <DHT.h>  //https://github.com/adafruit/DHT-sensor-library
                  //Also need to install Adafruit Unified Sensor
#include "/Users/fang/Documents/GitHub/ArduinoClock/1.Code/fonts/fixednums15x31.h"  //Also Can Use #include <fixednums15x31.h>
//#include "/Users/fang/Documents/GitHub/ArduinoClock/fonts/font8x8.h"  //Also Can Use #include <font8x8.h>
#include "/Users/fang/Documents/GitHub/ArduinoClock/1.Code/fonts/utf8font10x16.h"  //Also Can Use #include <utf8font10x16.h>
#define SW1 2 //定义开关1引脚
#define SW2 3 //定义开关2引脚
#define DHTPIN 12  //定义温度传感器引脚
#define DHTTYPE DHT11 //定义温度传感器型号
#define TIME_HEADER  "T"   // Header tag for serial time sync message
DHT dht(DHTPIN, DHTTYPE); //初始化温度传感器

//变量定义部分
int mod = 0;
long int t1 = 0;
long int t2 = 0;
float temp = 0;
float hum = 0;
String nowStringHour = "00";
String nowStringMinute = "00";
String nowStringSecond = "00";
String nowStringMonth = "00";
String nowStringDay = "00";
String tempString = "00.00";
String humString = "00.00";
int8_t sw1oldValue = true;
int8_t sw2oldValue = true;
int8_t sw1status = 0;
int8_t sw2status = 0;

void setup()
{
  //Initalize
  Serial.begin(9600);
  dht.begin();

  // switch setup
  pinMode(SW1, INPUT);
  digitalWrite(SW1, HIGH);
  pinMode(SW2, INPUT);
  digitalWrite(SW2, HIGH);

  // Initialize the GLCD 
  GLCD.Init();
  GLCD.ClearScreen();

  // Select the font for the default text area
  GLCD.SelectFont(utf8font10x16);
  // Set time
  setTime(0, 0, 0, 10, 10, 2021);

}

void loop()
{
  readSwitch(); //读取开关
  //插线时间同步部分
  if (Serial.available()) {
    processSyncMessage();
  }
  //显示第一行
  GLCD.CursorTo(0, 0);
  GLCD.SelectFont(utf8font10x16);
  //GLCD.println("                          Now Time");
  if (t1 == 0){
    t1 = millis() + 3000;
  }
  t2 = millis();
  if (t2 > t1){
    temp = dht.readTemperature();
    hum = dht.readHumidity();
    //Serial.println("t1: " + String(t1));
    //Serial.println("t2: " + String(t2));
    //delay(2000);
    t1 = 0;
  }
  if (temp <= 9){
    tempString = "0" + String(temp); 
  }else{
    tempString = String(temp);
  }
  if (hum <= 9){
    humString = "0" + String(hum); 
  }else{
    humString = String(hum);
  }
  GLCD.println("                " + tempString + "*C " + humString + "%");
  //显示第二行
  GLCD.CursorTo(0, 1);
  int8_t nowMonth = month();
  if (nowMonth <= 9){
    nowStringMonth = "0" + String(nowMonth);
  }else{
    nowStringMonth = String(nowMonth);
  }
  int8_t nowDay = day();
  if (nowDay <= 9){
    nowStringDay = "0" + String(nowDay);
  }else{
    nowStringDay = String(nowDay);
  }
  GLCD.println("                     " + String(year()) + " / " + nowStringMonth + " / " + nowStringDay);
  //显示第三行
  GLCD.CursorTo(0, 2);
  GLCD.SelectFont(fixednums15x31);
  int8_t nowHour = hour();
  if (nowHour <= 9){
    nowStringHour = "0" + String(nowHour);
  }else{
    nowStringHour = String(nowHour);
  }
  int8_t nowMinute = minute();
  if (nowMinute <= 9){
    nowStringMinute = "0" + String(nowMinute);
  }else{
    nowStringMinute = String(nowMinute);
  }
  int8_t nowSecond = second();
  if (nowSecond <= 9){
    nowStringSecond = "0" + String(nowSecond);
  }else{
    nowStringSecond = String(nowSecond);
  }
  GLCD.println(nowStringHour + ":" + nowStringMinute + ":" + nowStringSecond);
  /*
  if (second() == 0){
    GLCD.ClearScreen();
  }
  */
}

void processSyncMessage() {
  unsigned long pctime;
  const unsigned long DEFAULT_TIME = 1357041600; // Jan 1 2013

  if(Serial.find(TIME_HEADER)) {
     pctime = Serial.parseInt();
     if( pctime >= DEFAULT_TIME) { // check the integer is a valid time (greater than Jan 1 2013)
       setTime(pctime); // Sync Arduino clock to the time received on the serial port
     }
  }
}

void readSwitch() {
  int8_t sw1value = digitalRead(SW1);
  if (sw1value != sw1oldValue) {
    if (sw1value == LOW) {
      Serial.println("SW1!");
      if (mod <= 7){
        mod++;
        if (mod == 1){
          GLCD.ClearScreen();
          GLCD.SelectFont(utf8font10x16);
          GLCD.println("Clear Second!");
          delay(1000);
          GLCD.ClearScreen();
        }else if (mod == 2){
          GLCD.ClearScreen();
          GLCD.SelectFont(utf8font10x16);
          GLCD.println("Set Hour!");
          delay(1000);
          GLCD.ClearScreen();
        }else if (mod == 3){
          GLCD.ClearScreen();
          GLCD.SelectFont(utf8font10x16);
          GLCD.println("Set Minute!");
          delay(1000);
          GLCD.ClearScreen();
        }else if (mod == 4){
          GLCD.ClearScreen();
          GLCD.SelectFont(utf8font10x16);
          GLCD.println("Set Year!");
          delay(1000);
          GLCD.ClearScreen();
        }else if (mod == 5){
          GLCD.ClearScreen();
          GLCD.SelectFont(utf8font10x16);
          GLCD.println("Set Month!");
          delay(1000);
          GLCD.ClearScreen();
        }else if (mod == 6){
          GLCD.ClearScreen();
          GLCD.SelectFont(utf8font10x16);
          GLCD.println("Set Day!");
          delay(1000);
          GLCD.ClearScreen();
        }else{

        }
      }else{
        mod = 0;
      }
    }
    sw1oldValue = sw1value;
  }
  int8_t sw2value = digitalRead(SW2);
  if (sw2value != sw2oldValue) {
    if (sw2value == LOW) {
      Serial.println("SW2!");
        if (mod == 1){
          if (second() >= 50){
            setTime(hour(), minute() + 1, 0, day(), month(), year());  //H,M,S,D,M,Y
            GLCD.ClearScreen();
          }else{
            setTime(hour(), minute(), 0, day(), month(), year());  //H,M,S,D,M,Y
            GLCD.ClearScreen();
          }
        }else if (mod == 2){
          int8_t setHour = hour();
          setHour++;
          if (setHour == 24){
            setHour == 0;
          }
          setTime(setHour, minute(), second(), day(), month(), year());  //H,M,S,D,M,Y
          GLCD.ClearScreen();
        }else if (mod == 3){
          int8_t setMinute = minute();
          setMinute++;
          if (setMinute == 60){
            setMinute == 0;
          }
          setTime(hour(), setMinute, second(), day(), month(), year());  //H,M,S,D,M,Y
          GLCD.ClearScreen();
        }else if (mod == 4){
          int8_t setYear = year();
          setYear++;
          if (setYear == 2030){
            setYear == 2021;
          }
          setTime(hour(), minute(), second(), day(), month(), setYear);  //H,M,S,D,M,Y
          GLCD.ClearScreen();
        }else if (mod == 5){
          int8_t setMonth = month();
          setMonth++;
          if (setMonth == 13){
            setMonth == 1;
          }
          setTime(hour(), minute(), second(), day(), setMonth, year());  //H,M,S,D,M,Y
          GLCD.ClearScreen();
        }else if (mod == 6){
          int8_t setDay = day();
          setDay++;
          int8_t days[] = {0, 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
          int8_t m = month();
          if (setDay > days[m]) {
            setDay == 1;
          }
          setTime(hour(), minute(), second(), setDay, month(), year());  //H,M,S,D,M,Y
          GLCD.ClearScreen();
        }
      
    }
    sw2oldValue = sw2value;
  }
  delay(5);
}
