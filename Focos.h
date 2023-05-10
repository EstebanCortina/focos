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

const char *mqtt_server = "test.mosquitto.org";
String palabra;
const int mqtt_port = 1883;

WiFiClient espClient;
PubSubClient client(espClient);

// Configuracion Mqtt
class Focos
{
public:
  String nombre;
  // Mqtt
  void mqttSetup();
  void mqttListen();
  void reconnectMqtt();
  // bool ledState = false;
  //  Luxometro
  void luxoSetup();
  void luxoListen();
  uint16_t lux;
  // Humo
  void humoSetup();
  void humoListen();
  int valorHumo;
  // Flama
  void flamaSetup();
  void flamaListen();
  int valorFlama;
  int range;
};

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
  if (WiFi.status() != WL_CONNECTED)
  {
    // Reintentar la conexión
    WiFi.begin("E6CA82", "L21503735312143");
    while (WiFi.status() != WL_CONNECTED)
    {
      delay(1000);
    }
  }
  if (!client.connected())
  {
    reconnectMqtt();
  }
  client.loop();
}

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

void mqttSetup()
{
  pinMode(focoMqtt, OUTPUT);
  WiFi.begin("E6CA82", "L215037353121439");
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
  }
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void Focos::flamaListen()
{
  this->valorFlama = analogRead(inputFlama);
  this->range = map(this->valorFlama, sensorMin, sensorMax, 0, 4);
  // Serial.println(sensorReading);
  switch (this->range)
  {
  case 0:
    Serial.println("Peligro fuego");
    client.publish("esliTest/Flama", "Peligro Fuego");
    // Encender el foco
    digitalWrite(focoFlama, HIGH);
    break;
  case 1 || 2:
    Serial.println("Fuego lejano");
    client.publish("esliTest/Flama", "Peligro Fuego");
    break;
  case 3:
    Serial.println("No hay fuego");
    client.publish("esliTest/Flama", "Peligro Fuego");
    digitalWrite(focoFlama, LOW);
    break;
  }
}
void Focos::flamaSetup()
{
  pinMode(focoFlama, OUTPUT);
}
void Focos::humoSetup()
{
  pinMode(inputHumo, INPUT);
  pinMode(focoHumo, OUTPUT);
}
void Focos::humoListen()
{
  this->valorHumo = analogRead(inputHumo);
  if (this->valorHumo < 100)
  {
    digitalWrite(focoHumo, HIGH);
    Serial.println("Humo detectado");
    client.publish("esliTest/Humo", "Humo detectado");
  }
  else
  {
    digitalWrite(focoHumo, LOW);
    Serial.println("Humo no detectado");
    client.publish("esliTest/Humo", "Humo no detectado");
  }
}
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
void Focos::luxoListen()
{
  this->lux = lightMeter.readLightLevel();
  if (this->lux > 200)
  {
    Serial.print("Luminosidad: ");
    Serial.print(this->lux);
    Serial.println(" lux");
    digitalWrite(focoLuxometro, LOW);
    client.publish('esliTest/LuzAmbiental', "Es de dia");
  }
  else
  {
    Serial.print("Luminosidad: ");
    Serial.print(this->lux);
    Serial.println(" lux");
    digitalWrite(focoLuxometro, HIGH);
    client.publish('esliTest/LuzAmbiental', "Es de noche");
  }
}