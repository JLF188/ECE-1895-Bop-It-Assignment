#include <Wire.h>

#define MPU_ADDR_1 0x68  // First MPU-6050  (AD0 LOW)
#define MPU_ADDR_2 0x69  // Second MPU-6050 (AD0 HIGH)

#define THRESHOLD 300
#define SENSOR_PIN A3

void setup() {
  // COUNTER SETUP
  pinMode(8, OUTPUT);  // increment counter
  pinMode(7, OUTPUT);  // reset counter

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
  bool success;

  while (true) {
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

    if (ax_g > 1.5 || ay_g > 1.5 || az_g > 1.5) {
      return success = true;
      break;
    }

    delay(50);  // Poll at ~20Hz
  }

  return success;
}

bool wash() {
  bool success;

  while (true) {
    Wire.beginTransmission(MPU_ADDR_2);
    Wire.write(0x3B);
    Wire.endTransmission(false);
    Wire.requestFrom(MPU_ADDR_2, 6, true);

    int16_t ax = (Wire.read() << 8) | Wire.read();
    int16_t ay = (Wire.read() << 8) | Wire.read();
    int16_t az = (Wire.read() << 8) | Wire.read();

    float ax_g = abs(ax / 16384.0);
    float ay_g = abs(ay / 16384.0);
    float az_g = abs(az / 16384.0);

    if (ax_g > 1.5 || ay_g > 1.5 || az_g > 1.5) {
      return success = true;
      break;
    }

    delay(50);  // Poll at ~20Hz
  }

  return success;
}

bool chop() {
  bool success;

  while (true) {
    int rawValue = analogRead(SENSOR_PIN);

    if (rawValue >= THRESHOLD) {
      success = true;
      break;
    }

    delay(50);  // Poll at ~20 Hz
  }

  return success;
}

void loop() {
  if (digitalRead(9) == HIGH) {

    // Reset the counter
    digitalWrite(7, HIGH);
    delay(50);
    digitalWrite(7, LOW);

    for (int i = 0; i < 99; i++) {
      bool success = runRound();

      if (success) {
        digitalWrite(8, HIGH);
        delay(50);
        digitalWrite(8, LOW);
        delay(50);
      } else {
        break;
      }
      delay(1000);
    }
  }
}