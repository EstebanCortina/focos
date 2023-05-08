#include <Wire.h>
#include <BH1750.h>

#define RELE 2;
#define LumenSensor 4;

//BH1750 measureLight; //Esto es un objeto

int nightLvl = 50;
//int dayLvl = 100;

void setup() {
  Serial.begin(9600);
  LumenSensor.begin(BH1750_CONTINUOUS_HIGH_RES_MODE);
  //measureLight.setMode();
  pinMode(RELE, OUTPUT);
  pinmode(LumenSensor, INPUT);
}

void loop() {
  uint16_t lux = LumenSensor.readLightLevel();
  Serial.print("El nivel de luz es: ");
  Serial.print(lux);
  Serial.print("lux. ");
  //delay(1000);

  if (lux > nightLvl){
    digitalWrite(RELE, LOW);
    Serial.println("Foco Apagado.");
  } else{
    if (lux < nightLvl){
      digitalWrite(RELE, HIGH);
      Serial.println("Foco Encendido.");
    }
  }
  delay(2000);
}