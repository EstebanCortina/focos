#define luz 12
#define sensorMin 0
#define sensorMax 1024
#define pin A0
void setup(){
    Serial.begin(9600);
    pinMode (luz, OUTPUT); 
}

void loop(){
    int sensorReading = analogRead(pin);
    int range = map(sensorReading, sensorMin, sensorMax, 0, 3);

    switch(range){
        case 0:
            Serial.println("Peligro fuego");
            // Encender el foco
            digitalWrite(luz, LOW);
            break;
        case 1:
            Serial.println("Fuego lejano");
            // Encender el foco con PWM a una intensidad menor
            // analogWrite(luz, 64); // 25% de la intensidad máxima (255)
            break;
        case 2:
            Serial.println("No hay fuego");
            //Foco apagado
            digitalWrite(luz, HIGH);
            break;
    }
    delay(1200);
} 

