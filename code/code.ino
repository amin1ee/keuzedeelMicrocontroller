#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// Pins
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

  // Start OLED
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("OLED not found");
    while (true);
  }

  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
}

void loop() {
  long duration, cm;

  // ---- Trigger ultrasonic ----
  digitalWrite(trigpin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigpin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigpin, LOW);

  duration = pulseIn(echopin, HIGH);
  cm = microsecondsToCentimeters(duration);

  // ---- SERIAL MONITOR ----
  Serial.print("Distance: ");
  Serial.print(cm);
  Serial.println(" cm");

  // ---- OLED DISPLAY ----
  display.clearDisplay();
  display.setCursor(0, 0);
  display.print("Distance");
  display.setCursor(0, 30);
  display.print(cm);
  display.print(" cm");
  display.display();

  // Turn everything OFF first
  digitalWrite(led1, LOW);
  digitalWrite(led2, LOW);
  digitalWrite(led3, LOW);
  noTone(buzzer);

  // ---- LEDs + BUZZER WITH BUFFER ----
  if (cm < 5 && lastCm <= 7) {
    // Very close
    digitalWrite(led1, HIGH);
    digitalWrite(led2, HIGH);
    digitalWrite(led3, HIGH);
    tone(buzzer, 2000);   // continuous sound
  }
  else if (cm < 10 && lastCm <= 12) {
    // Medium close
    digitalWrite(led1, HIGH);
    digitalWrite(led2, HIGH);

    tone(buzzer, 1500);
    delay(100);
    noTone(buzzer);
    delay(100);
  }
  else if (cm < 20 && lastCm <= 22) {
    // Far close
    digitalWrite(led1, HIGH);

    tone(buzzer, 1000);
    delay(300);
    noTone(buzzer);
    delay(300);
  }

  lastCm = cm;
  delay(50);
}

// ---- Distance conversion ----
long microsecondsToCentimeters(long microseconds) {
  return microseconds / 29 / 2;
}
