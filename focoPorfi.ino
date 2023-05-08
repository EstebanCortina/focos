#define pinPorfi 2

void setup(){
    Serial.begin(9600);
    pinMode(pinPorfi, INPUT);
}

void loop(){
    int valor=digitalRead(pinPorfi);
    Serial.println(valor);
    delay(250);
}