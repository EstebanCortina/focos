#include "Focos.h"

Focos f;
void setup()
{
  // put your setup code here, to run once:

  Serial.begin(9600);
  Serial.println("NUEVO");

  pinMode(27, OUTPUT);
  WiFi.begin("E6CA82", "L21503735312143");
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
  }

  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);

  f.luxoSetup();
  f.humoSetup();
  f.flamaSetup();
  // mqttSetup();
}

void loop()
{
  /*
  if (WiFi.status() != WL_CONNECTED) {
    // Reintentar la conexión
    WiFi.begin("E6CA82","L21503735312143");
    while (WiFi.status() != WL_CONNECTED) {
      delay(1000);
    }
  }
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  // put your main code here, to run repeatedly:
  */
  f.luxoListen();
  delay(100);
  f.humoListen();
  delay(100);
  f.flamaListen();
  delay(100);
  mqttListen();
  delay(1000);
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