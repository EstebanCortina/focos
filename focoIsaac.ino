#define sensorMin 0
#define sensorMax 1024
#define pin A0
void setup(){
    Serial.begin(9600);
}
void loop(){
    int sensorReading =analogRead(pin);
    int range=map(sensorReading, sensorMin, sensorMax, 0, 3);

    switch(range){
        case 0:
            Serial.println("Peligro fuego");
            break;
        case 1:
            Serial.println("Fuego lejano");
            break;
        case 2:
            Serial.println("No hay fuego");
            break;
    }
    dealy(12000);
}