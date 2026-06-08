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

    lcd.clear();

    lcd.print("L:");
    lcd.print(dL);

    lcd.setCursor(0,1);
    lcd.print("R:");
    lcd.print(dR);

    delay(300);
}