#define pinsensor A0
#define pinrelat 8
int valor;
void setup(){
  Serial.begin(9600);
  pinMode(pinsensor,INPUT);
  pinMode(pinrelat,OUTPUT);
}
void loop(){
  valor = analogRead(pinsensor);
  Serial.println(valor);
  if(valor < 100){
    digitalWrite(pinrelat,HIGH);
    Serial.println("Humo detectado");
    delay(1000);
  } else{
    Serial.println("Humo no detectado");
    digitalWrite(pinrelat,LOW);  
  }
  delay(100);
}
