#include <Wire.h>
#include <U8g2lib.h>

U8G2_SH1106_128X64_NONAME_F_HW_I2C display(U8G2_R0, U8X8_PIN_NONE);

const int trigpin = 7;
const int echopin = 8;

const int led1 = 2;
const int led2 = 3;
const int led3 = 4;
const int buzzer = 6;

long lastCm = 100;

void setup() {
  Serial.begin(9600);

  pinMode(trigpin, OUTPUT);
  pinMode(echopin, INPUT);

  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);
  pinMode(buzzer, OUTPUT);

  display.begin();
}

void loop() {
  long duration, cm;

  digitalWrite(trigpin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigpin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigpin, LOW);

  duration = pulseIn(echopin, HIGH, 30000);
  cm = microsecondsToCentimeters(duration);

  Serial.print("Distance: ");
  Serial.print(cm);
  Serial.println(" cm");

  display.clearBuffer();
  display.setFont(u8g2_font_ncenB14_tr);

  display.setCursor(0, 20);
  display.print("Distance");

  display.setCursor(0, 50);
  display.print(cm);
  display.print(" cm");

  display.sendBuffer();

  digitalWrite(led1, LOW);
  digitalWrite(led2, LOW);
  digitalWrite(led3, LOW);
  noTone(buzzer);

  if (cm < 5 && lastCm <= 7) {
    digitalWrite(led1, HIGH);
    digitalWrite(led2, HIGH);
    digitalWrite(led3, HIGH);
    tone(buzzer, 2000);
  }
  else if (cm < 10 && lastCm <= 12) {
    digitalWrite(led1, HIGH);
    digitalWrite(led2, HIGH);

    tone(buzzer, 1500);
    delay(100);
    noTone(buzzer);
    delay(100);
  }
  else if (cm < 20 && lastCm <= 22) {
    digitalWrite(led1, HIGH);

    tone(buzzer, 1000);
    delay(300);
    noTone(buzzer);
    delay(300);
  }

  lastCm = cm;
  delay(50);
}

long microsecondsToCentimeters(long microseconds) {
  if (microseconds == 0) return 999;
  return microseconds / 29 / 2;
}