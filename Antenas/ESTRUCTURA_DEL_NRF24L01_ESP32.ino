//codigo para NRF24L01 + PA/LNA
// VCC  = 3.3    INDICANDO LAS SALIDAS DE COMUNICACION SPI PARA ESP32
// GND  = GND
// CE   = 5
// CSN  = 4
// SCK  = 18
// MISO = 19
// MOSI = 23


// GND  : VCC        pin out de NRF24L01
// CE   : CSN
// SCK  : MOSI
// MISO :

#include <SPI.h>
#include <RF24.h>


RF24 radio(5,4); //CE Y CSN
const byte address[6] = "00001"; //direccion

struct PAQUETE{
float tem;
float hum;
float pre;
float alt;
float X;
float Y;
float Z;
};
PAQUETE data;

int TEM, HUM, PRE, ALT;
void setup() {
  Serial.begin(115200);
  if (radio.begin()) {
    Serial.println("NRF24L01 iniciado");
    delay(1000);
  } else {
    Serial.println("error en NRF24L01");
    while (1)
      ;
  }
radio.openWritingPipe(address); //direccion
//radio.openReadingPipe(1, address);  //direccion del transmisor como receptor
radio.setPALevel(RF24_PA_HIGH); //potencia
radio.stopListening(); // modo transmision
//radio.startListening();    // modo receptor
radio.setChannel(70); // canal
//abajo de este comentario sensores en setup





}

void loop() {
 //codigo para sensores 
int16_t x,y,z; //variables de 16 bites cada uno para giroscopio 

























//fin de codigo para sensores
//a continuacion empaquedado
data.tem = TEM; // variable de temperatura
data.hum = HUM; // variable de humedad
data.pre = PRE; // variable de presion 
data.alt = ALT; // variable de altitud
data.X   = x;
data.Y   = y;
data.Z   = z;
//a continuacion envio de datos
radio.write(&data, sizeof(PAQUETE));


delay(100);
}
