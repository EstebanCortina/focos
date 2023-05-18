#include "Focos.h"
#include <WiFiMulti.h>
#include <ArduinoJson.h>

WiFiMulti wifiMulti;

// Este objeto es donde se asignaran los valores de los sensores.
StaticJsonDocument<200> sensData;
// En este String es donde se guardara el objeto anterior serializado.
String sensDataJson;

// Estas variables contienen los datos de los sensores
int humoValue;
int luxValue;
int flamaValue;

// Estas variables contienen el estado del foco.
int flamaFoco;
int humoFoco;
int gasFoco;
int mqttFoco;

Focos f;
void setup()
{
  Serial.begin(9600);

  pinMode(focoMqtt, OUTPUT);

  // WifiMulti aun no esta probado.
  wifiMulti.addAP("NEPOMUCENO", "sambs0279");
  wifiMulti.addAP("nombre_de_la_red_2", "contraseña_2");

  Serial.print("Conectando a la red WiFi...");

  while (wifiMulti.run() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(500);
  }

  Serial.println();
  Serial.print("Conexión WiFi establecida. Dirección IP: ");
  Serial.println(WiFi.localIP());

  // Single wifi
  WiFi.begin("NEPOMUCENO", "sambs0279");
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
  }

  // Asignar servidor y asignar la funcion de callback para MQTT
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);

  f.luxoSetup();
  f.humoSetup();
  f.flamaSetup();
}

void loop()
{
  luxValue = f.luxoListen();
  delay(100);
  humoValue = f.humoListen();
  delay(100);
  flamaValue = f.flamaListen();
  delay(100);
  mqttListen();
  delay(1000);

  // Cambiar para reconectar MultiWifi
  if (WiFi.status() != WL_CONNECTED)
  {
    // Reintentar la conexión
    WiFi.begin("E6CA82", "L21503735312143");
    while (WiFi.status() != WL_CONNECTED)
    {
      delay(1000);
    }
  }
}

void reconnect()
{
  // Loop hasta que estemos conectados
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