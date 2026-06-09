#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27,16,2);

const int trigL = 2;
const int echoL = 3;

const int trigR = 4;
const int echoR = 5;

const int trigB = 6;
const int echoB = 7;

float prevDist = 0;
float speed = 0;
unsigned long prevTime = 0;

long getDistance(int trig, int echo)
{
    long sum = 0;

    for(int i=0;i<5;i++)
    {
        digitalWrite(trig,LOW);
        delayMicroseconds(2);

        digitalWrite(trig,HIGH);
        delayMicroseconds(10);

        digitalWrite(trig,LOW);

        long duration = pulseIn(echo,HIGH);
        long dist = duration * 0.034 / 2;

        sum += dist;
        delay(5);
    }

    return sum/5;
}

int getRisk(long d, float s)
{
    if(d < 10) return 3;

    else if(d < 20 && s > 10)
        return 3;

    else if(d < 25)
        return 2;

    else if(d < 30 && s > 5)
        return 2;

    else if(d < 40)
        return 1;

    return 0;
}

void setup()
{
    lcd.init();
    lcd.backlight();

    pinMode(trigL,OUTPUT);
    pinMode(echoL,INPUT);

    pinMode(trigR,OUTPUT);
    pinMode(echoR,INPUT);

    pinMode(trigB,OUTPUT);
    pinMode(echoB,INPUT);
}

void loop()
{
    long dL = getDistance(trigL,echoL);
    long dR = getDistance(trigR,echoR);
    long dB = getDistance(trigB,echoB);

    long currDist = min(dL,min(dR,dB));

    unsigned long now = millis();
    float dt = (now-prevTime)/1000.0;

    if(dt>0)
  {
    speed = (prevDist-currDist)/dt;

    if(speed<0)
        speed = 0;
  }

     prevDist = currDist;
     prevTime = now;

     int rL = getRisk(dL,speed);
     int rR = getRisk(dR,speed);
     int rB = getRisk(dB,speed);

    bool hasCritical = (rL == 3 || rR == 3 || rB == 3);
    bool hasHigh     = (rL == 2 || rR == 2 || rB == 2);
    bool hasWarning  = (rL == 1 || rR == 1 || rB == 1);

    lcd.clear();

    if (hasCritical)
        lcd.print("CRITICAL");
    else if (hasHigh)
         lcd.print("HIGH RISK");
    else if (hasWarning)
          lcd.print("WARNING");
    else
        lcd.print("ALL SAFE");

    lcd.setCursor(0,1);
    
    delay(300);
}