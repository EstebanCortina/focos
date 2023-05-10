// Librerias para sensor de luz ambiental
#include <Wire.h>
#include <BH1750.h>

// PINES sensor-ESP32

// PINES ESP32-foco
#define focoLuxometro 33

BH1750 lightMeter;
class Focos
{
public:
  // Mqtt
  void callback(char *topic, byte *payload, unsigned int length);
  void mqttSetup();
  void reconnectMqtt();
  // Luxometro
  void luxoSetup();
  void luxoListen();
};
void luxoSetup()
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
void luxoListen()
{
  uint16_t lux = lightMeter.readLightLevel();
  if (lux > 200)
  {
    Serial.print("Luminosidad: ");
    Serial.print(lux);
    Serial.println(" lux");
    digitalWrite(focoLuxometro, LOW);
  }
  else
  {
    Serial.print("Luminosidad: ");
    Serial.print(lux);
    Serial.println(" lux");
    digitalWrite(focoLuxometro, HIGH);
  }

  delay(1000);
}
/*
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

void mqttSetup()
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

void reconnectMqtt()
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
*/