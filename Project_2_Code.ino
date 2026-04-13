#include <Wire.h>

#define MPU_ADDR_1 0x68  // First MPU-6050  (AD0 LOW)
#define MPU_ADDR_2 0x69  // Second MPU-6050 (AD0 HIGH)

#define THRESHOLD 300
#define SENSOR_PIN A3

unsigned long round_timer = 1000UL;  //time to complete action

void setup() {
  // COUNTER SETUP
  pinMode(8, OUTPUT);  // increment counter
  pinMode(7, OUTPUT);  // reset counter

  // SPEAKERS SETUP
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(10, OUTPUT);

  digitalWrite(7, HIGH);
  delay(50);
  digitalWrite(7, LOW);

  // START BUTTON SETUP
  pinMode(9, INPUT);

  // MPU-6050 SETUPS
  Wire.begin();

  Wire.beginTransmission(MPU_ADDR_1);
  Wire.write(0x6B);
  Wire.write(0x00);  // Wake up MPU-6050 #1
  Wire.endTransmission(true);

  Wire.beginTransmission(MPU_ADDR_2);
  Wire.write(0x6B);
  Wire.write(0x00);  // Wake up MPU-6050 #2
  Wire.endTransmission(true);

  randomSeed(analogRead(0));
}

bool runRound() {
  bool success;
  int rand = random(1, 4);  // randomly picks 1, 2, 3

  if (rand == 1)
    success = flip();
  else if (rand == 2)
    success = wash();
  else
    success = chop();

  return success;
}

bool flip() {
  digitalWrite(5, HIGH);
  delay(200);
  digitalWrite(5, LOW);

  unsigned long start_time = millis();

  while (true) {
    if (millis() - start_time >= 2000UL) 
      return false;

    Wire.beginTransmission(MPU_ADDR_1);
    Wire.write(0x3B);
    Wire.endTransmission(false);
    Wire.requestFrom(MPU_ADDR_1, 6, true);

    int16_t ax = (Wire.read() << 8) | Wire.read();
    int16_t ay = (Wire.read() << 8) | Wire.read();
    int16_t az = (Wire.read() << 8) | Wire.read();

    float ax_g = abs(ax / 16384.0);
    float ay_g = abs(ay / 16384.0);
    float az_g = abs(az / 16384.0);

    if (az_g > 1.5)
      return true;

    // Check wrong sensor (MPU2)
    Wire.beginTransmission(MPU_ADDR_2);
    Wire.write(0x3B);
    Wire.endTransmission(false);
    Wire.requestFrom(MPU_ADDR_2, 6, true);
    Wire.read(); Wire.read();  // skip ax
    Wire.read(); Wire.read();  // skip ay
    int16_t az2 = (Wire.read() << 8) | Wire.read();
    if (abs(az2 / 16384.0) > 1.5) 
      return false;

    // Check wrong sensor (chop)
    if (analogRead(SENSOR_PIN) >= THRESHOLD) 
      return false;

    delay(50);  // Poll at ~20Hz
  }
}

bool wash() {
  digitalWrite(10, HIGH);
  delay(200);
  digitalWrite(10, LOW);

  unsigned long start_time = millis();

  while (true) {
    if (millis() - start_time >= 1000UL)
      return false;

    // Check correct sensor (MPU2)
    Wire.beginTransmission(MPU_ADDR_2);
    Wire.write(0x3B);
    Wire.endTransmission(false);
    Wire.requestFrom(MPU_ADDR_2, 6, true);
    Wire.read(); Wire.read();  // skip ax
    Wire.read(); Wire.read();  // skip ay
    int16_t az = (Wire.read() << 8) | Wire.read();
    if (abs(az / 16384.0) > 1.5) 
      return true;

    // Check wrong sensor (MPU1)
    Wire.beginTransmission(MPU_ADDR_1);
    Wire.write(0x3B);
    Wire.endTransmission(false);
    Wire.requestFrom(MPU_ADDR_1, 6, true);
    Wire.read(); Wire.read();  // skip ax
    Wire.read(); Wire.read();  // skip ay
    int16_t az2 = (Wire.read() << 8) | Wire.read();
    if (abs(az2 / 16384.0) > 1.5) 
      return false;

    // Check wrong sensor (chop)
    if (analogRead(SENSOR_PIN) >= THRESHOLD) 
      return false;

    delay(50);
  }
}

bool chop() {
  digitalWrite(6, HIGH);
  delay(200);
  digitalWrite(6, LOW);

  unsigned long start_time = millis();

  while (true) {
    if (millis() - start_time >= 1000UL)
      return false;

    // Check correct sensor (chop)
    if (analogRead(SENSOR_PIN) >= THRESHOLD) 
      return true;

    // Check wrong sensor (MPU1)
    Wire.beginTransmission(MPU_ADDR_1);
    Wire.write(0x3B);
    Wire.endTransmission(false);
    Wire.requestFrom(MPU_ADDR_1, 6, true);
    Wire.read(); Wire.read();  // skip ax
    Wire.read(); Wire.read();  // skip ay
    int16_t az = (Wire.read() << 8) | Wire.read();
    if (abs(az / 16384.0) > 1.5) 
      return false;

    // Check wrong sensor (MPU2)
    Wire.beginTransmission(MPU_ADDR_2);
    Wire.write(0x3B);
    Wire.endTransmission(false);
    Wire.requestFrom(MPU_ADDR_2, 6, true);
    Wire.read(); Wire.read();  // skip ax
    Wire.read(); Wire.read();  // skip ay
    int16_t az2 = (Wire.read() << 8) | Wire.read();
    if (abs(az2 / 16384.0) > 1.5) 
      return false;

    delay(50);
  }
}

void loop() {
  if (digitalRead(9) == HIGH) {
    // Reset the counter
    digitalWrite(7, HIGH);
    delay(50);
    digitalWrite(7, LOW);

    round_timer = 1000UL;  // reset the round timer

    delay(1000);  // small pause on start

    for (int i = 0; i < 100; i++) {
      bool success = runRound();

      if (success) {
        digitalWrite(8, HIGH);
        delay(50);
        digitalWrite(8, LOW);
        delay(50);

        if (i == 99) {
          for (int j = 0; j < 2; j++) {
            digitalWrite(5, HIGH);
            digitalWrite(6, HIGH);
            digitalWrite(10, HIGH);
            delay(500);
            digitalWrite(5, LOW);
            digitalWrite(6, LOW);
            digitalWrite(10, LOW);
            delay(500);
          }
        }
      } 
      else {
        for (int j = 0; j < 3; j++) {
          digitalWrite(5, HIGH);
          digitalWrite(6, HIGH);
          digitalWrite(10, HIGH);
          delay(200);
          digitalWrite(5, LOW);
          digitalWrite(6, LOW);
          digitalWrite(10, LOW);
          delay(100);
        }
        break;
      }
      if (i % 10 == 1 && i > 1)
        round_timer -= 50UL;
      delay(round_timer);
    }
  }
}
