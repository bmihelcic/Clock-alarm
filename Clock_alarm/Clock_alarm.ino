#include <DS3231.h> // http://www.rinkydinkelectronics.com/library.php?id=73
#include "TM1637.h" // Grove 4-digit display (Seeed studio)

#define CLK 4
#define DIO 3
#define enter 5
#define scroll 6
#define buzzer 8

DS3231 rtc(SDA,SCL);
TM1637 displ(CLK,DIO);
Time t;

int8_t TimeDisp[] = {0x00,0x00,0x00,0x00};
uint8_t Digits[10] = {0,1,2,3,4,5,6,7,8,9};
uint8_t h0,h1,m2,m3;
bool ClockPoint = 0;
unsigned long t1;
int br = 0;
int br2 = 0;
volatile bool flag = false;

void setup(){
  pinMode(enter, INPUT_PULLUP);
  pinMode(scroll, INPUT_PULLUP);
  pinMode(buzzer, OUTPUT);
  pinMode(2, INPUT_PULLUP);
  rtc.begin();
  displ.init();
  displ.set(BRIGHT_TYPICAL);
  t1 = millis();
  attachInterrupt(digitalPinToInterrupt(2), exitISR, CHANGE);
}

void loop(){
  if(millis() - t1 > 500){
    displ.point(ClockPoint);
    TimeUpdate();
    displ.display(TimeDisp);
    ClockPoint = !ClockPoint;
    t1 = millis();
  }
  if(digitalRead(enter) == LOW){
    br++; 
    if(br >= 3000) setAlarm();
  }
  if(digitalRead(scroll) == LOW){
    br2++; 
    if(br2 >= 3000) timeSetup();
  }
  delayMicroseconds(500);
  if(checkAlarm() == 1){
    if(t.sec<10 && flag == true){
      buzz();
    }
  }
  if(t.sec>=10){
    flag = true;
  }
}

void TimeUpdate(){
  t = rtc.getTime();
  TimeDisp[0] = t.hour/10;
  TimeDisp[1] = t.hour % 10;
  TimeDisp[2] = t.min/10;
  TimeDisp[3] = t.min % 10;
}

void timeSetup(){
  int count,i;
  int setHour;
  int setMinute;
  int setTimeDigits[4];
  int8_t digitSelect;

  displ.point(0);
  displ.clearDisplay();
  delay(500);
  
  i=0;
  count=0;
  digitSelect = Digits[i];
  do{
    if(digitalRead(scroll) == LOW){
      if(count == 0 && i>2) i=0;
      if(count == 1 && TimeDisp[0] == 2 && i>3) i=0;
      if(count == 1 && TimeDisp[0] < 2 && i>9) i=0;
      if(count == 2 && i>5) i=0;
      if(count == 3 && i>9) i=0;
      digitSelect = Digits[i];
      displ.display(count,digitSelect);
      i++;
    }
    if(digitalRead(enter) == LOW){
      setTimeDigits[count] = digitSelect;
      if(count == 0)  h0 = digitSelect;
      else if(count == 1) h1 = digitSelect;
      else if(count == 2) m2 = digitSelect;
      else m3 = digitSelect;
      count++;
      i=0;
    }
    delay(200);
  }while(count<4);
  setHour = setTimeDigits[0]*10 + setTimeDigits[1];
  setMinute = setTimeDigits[2]*10 + setTimeDigits[3];
  rtc.setTime(setHour, setMinute, 0);
  br2 = 0;
}


void setAlarm(){
  int count,i;
  int8_t digitSelect;

  displ.point(0);
  displ.clearDisplay();
  delay(500);
  
  i=0;
  count=0;
  digitSelect = Digits[i];
  do{
    if(digitalRead(scroll) == LOW){
      if(count == 0 && i>2) i=0;
      if(count == 1 && TimeDisp[0] == 2 && i>3) i=0;
      if(count == 1 && TimeDisp[0] < 2 && i>9) i=0;
      if(count == 2 && i>5) i=0;
      if(count == 3 && i>9) i=0;
      digitSelect = Digits[i];
      displ.display(count,digitSelect);
      i++;
    }
    if(digitalRead(enter) == LOW){
      TimeDisp[count] = digitSelect;
      if(count == 0)  h0 = digitSelect;
      else if(count == 1) h1 = digitSelect;
      else if(count == 2) m2 = digitSelect;
      else m3 = digitSelect;
      count++;
      i=0;
    }
    delay(200);
  }while(count<4);
  br = 0;
}

void buzz(){
  digitalWrite(buzzer, HIGH);
  delay(200);
  digitalWrite(buzzer, LOW);
  delay(200); 
}

int checkAlarm(){
  if(TimeDisp[0] == h0 && TimeDisp[1] == h1 && TimeDisp[2] == m2 && TimeDisp[3] == m3){
    return 1;
  }
  else  return 0;
}

void exitISR(){
  flag = false;
}
