#include <Wire.h>
#include <BH1750.h>

BH1750 lightSensor;
const int RELE = 13;

void setup() {
  Serial.begin(9600);
  lightSensor.begin();
  pinMode(RELE, OUTPUT);
} 

void loop() {
  uint16_t lux = lightSensor.readLightLevel();
  Serial.print("Luminosidad: ");
  Serial.print(lux);
  Serial.println(" lux");

  if (lux > 200) {
    digitalWrite(RELE, HIGH);
  } else {
    digitalWrite(RELE, LOW);
  }

  delay(100);
}