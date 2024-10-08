//ESP32 Dev Module
//CE    5
//CSN   4
//SCK  18
//Miso 19
//MOSI 23



#include <SPI.h>
#include <RF24.h>

RF24 radio(5, 4); //pines CE y CSN
const byte address[6] = "00001";

DataPackage data; // estructura

void setup() {
 Serial.begin(9600);
 delay(1000);
 if (radio.begin()){
  Serial.println("iniciado NRF24L01");
 }else{
  Serial.println("error en NRF24L01");
  while(1);
 }
 radio.openReadingPipe(1, address); //direccion del transmisor
 radio.setPALevel (RF24_PA_HIGH);// alta potencia
 radio.startListening(); //modo recepcion
 radio.setChannel(70);
}

void loop() {}