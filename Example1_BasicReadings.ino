#include <Wire.h>
#include "SparkFunBME280.h"

#define LED_PIN 6

BME280 mySensor;

void setup()
{
  pinMode(LED_PIN, OUTPUT);

  Serial.begin(115200);
  Serial.println("Reading basic values from BME/BMP280");

  Wire.begin();

  mySensor.setI2CAddress(0x76);

  if (mySensor.beginI2C() == false)
  {
    Serial.println("Sensor not detected at 0x77!");

    while (1) {
      digitalWrite(LED_PIN, HIGH);
      delay(100);
      digitalWrite(LED_PIN, LOW);
      delay(100);
    }
  }
}

void loop()
{
  float tempF = mySensor.readTempF();

  if (tempF > 80) {
    digitalWrite(LED_PIN, HIGH);
  } else {
    digitalWrite(LED_PIN, LOW);
  }

  Serial.print("Pressure: ");
  Serial.print(mySensor.readFloatPressure(), 0);

  Serial.print(" Alt: ");
  Serial.print(mySensor.readFloatAltitudeFeet(), 1);

  Serial.print(" Temp: ");
  Serial.print(tempF, 2);

  Serial.print(" Humidity: ");
  Serial.print(mySensor.readFloatHumidity(), 0);

  Serial.println();

  delay(500);
}