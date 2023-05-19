#include <Wire.h>
#include <BH1750.h>
#include <WiFi.h>
#include <PubSubClient.h>

// Rangos para sensor de Flama
#define sensorMin 0
#define sensorMax 4095

// PINES sensor-ESP32
#define inputHumo 34
#define inputFlama 35

// PINES ESP32-foco
#define focoLuxometro 33
#define focoHumo 25
#define focoFlama 26
#define focoMqtt 27

BH1750 lightMeter;
bool ledState = false;

// Configuracion del servidor MQTT (Mosquitto Broker publico)
const char *mqtt_server = "test.mosquitto.org";
String palabra;
const int mqtt_port = 1883;

// Este cliente es de la libreria MultiWifi.h
WiFiClient espClient;
// Este cliente es para el MQTT
PubSubClient client(espClient);

// Configuracion Mqtt
class Focos
{
public:
  // Mqtt
  void mqttSetup();
  void mqttListen();
  void reconnectMqtt();

  //  Luxometro
  void luxoSetup();
  int luxoListen();
  uint16_t lux;
  // Humo
  void humoSetup();
  int humoListen();
  int valorHumo;
  // Flama
  void flamaSetup();
  int flamaListen();
  int valorFlama;
  int range;
};
void callback(char *topic, byte *payload, unsigned int length)
{
  Serial.print("Mensaje recibido [");
  Serial.print(topic);
  Serial.print("] ");

  for (int i = 0; i < length; i++)
  {
    Serial.print((char)payload[i]);
  }
  ledState = !ledState;
  digitalWrite(focoMqtt, ledState);
}
void reconnectMqtt()
{
  while (!client.connected())
  {
    Serial.print("Conectando al servidor MQTT...");

    // Intenta conectarse
    if (client.connect(mqtt_server))
    {
      Serial.println("conectado");

      // Suscribirse a un tema
      client.subscribe("focoEsli");
    }
    else
    {
      Serial.print("fallo, rc=");
      Serial.print(client.state());
      Serial.println(" esperando 5 segundos antes de volver a intentar");
      // Espera 5 segundos antes de intentar nuevamente
      delay(5000);
    }
  }
}

void mqttListen()
{
  if (!client.connected())
  {
    reconnectMqtt();
  }
  client.loop();
}

// FLAMA
int Focos::flamaListen()
{
  this->valorFlama = analogRead(inputFlama);
  this->range = map(this->valorFlama, sensorMin, sensorMax, 0, 4);
  // Serial.println(sensorReading);
  switch (this->range)
  {
  case 0:
    Serial.println("Peligro fuego");
    // Encender el foco
    digitalWrite(focoFlama, HIGH);
    break;
  case 1 || 2:
    Serial.println("Fuego lejano");
    break;
  case 3:
    Serial.println("No hay fuego");
    digitalWrite(focoFlama, LOW);
    break;
  }
  return this->range;
}
void Focos::flamaSetup()
{
  pinMode(focoFlama, OUTPUT);
}

// Humo
void Focos::humoSetup()
{
  pinMode(inputHumo, INPUT);
  pinMode(focoHumo, OUTPUT);
}
int Focos::humoListen()
{
  this->valorHumo = analogRead(inputHumo);
  if (this->valorHumo < 100)
  {
    digitalWrite(focoHumo, HIGH);
    Serial.println("Humo detectado");
  }
  else
  {
    digitalWrite(focoHumo, LOW);
    Serial.println("Humo no detectado");
  }
  return this->valorHumo;
}

// Luxometro
void Focos::luxoSetup()
{
  Wire.begin();
  if (lightMeter.begin())
  {
    Serial.println("Conectado");
  }
  else
  {
    Serial.println("No conectado");
  }
  pinMode(focoLuxometro, OUTPUT);
}
int Focos::luxoListen()
{
  this->lux = lightMeter.readLightLevel();
  if (this->lux > 200)
  {
    Serial.print("Luminosidad: ");
    Serial.print(this->lux);
    Serial.println(" lux");
    digitalWrite(focoLuxometro, LOW);
  }
  else
  {
    Serial.print("Luminosidad: ");
    Serial.print(this->lux);
    Serial.println(" lux");
    digitalWrite(focoLuxometro, HIGH);
  }
  return this->lux;
}