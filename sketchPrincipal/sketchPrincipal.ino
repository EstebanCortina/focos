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
  wifiMulti.addAP("E6CA82", "L21503735312143");
  wifiMulti.addAP("nombre_de_la_red_2", "contraseña_2");

  Serial.print("Conectando a la red WiFi...");

  connectToWiFi();

  // Asignar servidor y asignar la funcion de callback para MQTT.
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);

  // Setup de los sensores.
  f.luxoSetup();
  // f.humoSetup();
  // f.flamaSetup();
}

void loop()
{

  mqttListen();

  luxValue = f.luxoListen();
  delay(100);

  // TODO ESTO SERA UN NUEVO METODO DE LA LIBRERIA Focos.h
  sensData["lux"] = luxValue;

  serializeJson(sensData, sensDataJson);
  client.publish("esliPrueba", sensDataJson.c_str());

  // ESTA VARIABLE DEBE SER LOCAL DEL NUEVO METODO
  sensDataJson = "";
  // TERMINA BLOQUE DE NUEVO METODO

  /*
  humoValue = f.humoListen();
  delay(100);
  flamaValue = f.flamaListen();
  delay(100);
  */

  delay(2000);

  if (WiFi.status() != WL_CONNECTED)
  {
    Serial.println("Conexión WiFi perdida. Intentando reconectar...");
    connectToWiFi();
  }
}

void connectToWiFi()
{
  while (wifiMulti.run() != WL_CONNECTED)
  {
    Serial.print("Conectando a ");
    Serial.println(WiFi.SSID());

    delay(1000);
  }

  Serial.println("Conectado a WiFi");
  Serial.print("Dirección IP: ");
  Serial.println(WiFi.localIP());
}
