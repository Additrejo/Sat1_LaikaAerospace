//RECEPTOR NRF24 CON ALTIMETRO
//ESP32 Dev Module
//CE    5
//CSN   4
//SCK  18
//MISO 19
//MOSI 23


//
#include <SPI.h>
#include <RF24.h>

RF24 radio(5, 4);  //pines CE y CSN
const byte address[6] = "00001";

struct DataPackage {
  float tem;
  float hum;
  float pre;
  float alt;
  float X;
  float Y;
  float Z;
};

DataPackage data;  // estructura
int LED = 21;
void setup() {
pinMode(LED, OUTPUT);  
  Serial.begin(9600);
  delay(1000);
  if (radio.begin()) {
    Serial.println("iniciado NRF24L01");
    digitalWrite(LED, HIGH);
  } else {
    Serial.println("error en NRF24L01");
    while (1)
      ;
  }
  radio.openReadingPipe(1, address);  //direccion del transmisor
  radio.setPALevel(RF24_PA_HIGH);     // alta potencia
  radio.startListening();             //modo recepcion
  radio.setChannel(70);
}

void loop() {
  if (radio.available()) {
digitalWrite(LED, HIGH);
    radio.read(&data, sizeof(DataPackage));

    Serial.print("TEMPERATURA:   ");
    Serial.print(data.tem);
    Serial.println(" C");
    Serial.print("HUMEDAD:       ");
    Serial.print(data.hum);
    Serial.println(" %");
    Serial.print("PRESION:       ");
    Serial.print(data.pre);
    Serial.println(" hPa");
    Serial.print("ALTITUD:       ");
    Serial.print(data.alt);
    Serial.println(" m");
    Serial.print("X:             ");
    Serial.println(data.X);
    Serial.print("Y:             ");
    Serial.println(data.Y);
    Serial.print("Z:             ");
    Serial.println(data.Z);
  }
  else{
    digitalWrite(LED, LOW);
  }
}
