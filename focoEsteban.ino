#include <WiFi.h>
#include <PubSubClient.h>
#define pinLED 22
const char *mqtt_server = "test.mosquitto.org";
String palabra;
const int mqtt_port = 1883;

bool ledState = false;

WiFiClient espClient;
PubSubClient client(espClient);

void callback(char *topic, byte *payload, unsigned int length)
{
  Serial.print("Mensaje recibido [");
  Serial.print(topic);
  Serial.print("] ");

  for (int i = 0; i < length; i++)
  {
    Serial.print((char)payload[i]);
    palabra += (char)payload[i];
  }

  Serial.println(palabra);
  palabra = "";
  ledState = !ledState;
  digitalWrite(pinLED, ledState);
}

void setup()
{
  pinMode(pinLED, OUTPUT);
  // put your setup code here, to run once:
  // Inicializar la conexión Wi-Fi
  WiFi.begin("E6CA82", "L21503735312143");
  Serial.begin(9600);

  // Esperar a que se establezca la conexión
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
  }

  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void loop()
{
  // put your main code here, to run repeatedly:

  if (WiFi.status() != WL_CONNECTED)
  {
    // Reintentar la conexión
    WiFi.begin("E6CA82", "L21503735312143");
    while (WiFi.status() != WL_CONNECTED)
    {
      delay(1000);
    }
  }

  /*
    Serial.print("SSID: ");
    Serial.println(WiFi.SSID());
    Serial.print("RSSI: ");
    Serial.println(WiFi.RSSI());
    delay(1000);
  */

  if (!client.connected())
  {
    reconnect();
  }
  client.loop();
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