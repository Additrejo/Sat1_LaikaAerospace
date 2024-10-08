// EMISOR NRF24, MPU6050


#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <SPI.h>
#include <RF24.h>
#include <MPU6050.h>

#define nivelMar (1013.25);
RF24 radio(5, 4);    //pines CE y CSN
const byte address[6] = "00001";

struct DataPackage {
  float tem;
  float hum;
  float pre;
  float   alt;
  float   X;
  float   Y;
  float   Z;
};
DataPackage data;

void setup() {
  Wire.begin();

  Serial.begin(9600);
  mpu.initialize();
  
  if (radio.begin()) {
    Serial.println("NRF24L01 iniciado");
    delay(1000);
  } else {
    Serial.println("error en NRF24L01");
    while (1)
      ;
  }
  radio.openWritingPipe(address);  // direccion de receptor
  radio.setPALevel(RF24_PA_HIGH);  // alta potencia
  radio.stopListening();           //solo transmision
  radio.setChannel(70);

  
}

void loop() {
  
  int16_t gx, gy, gz;


  mpu.getRotation(&gx, &gy, &gz);

  data.X = gx /131.0;                             //2 Bytes
  data.Y = gy /131.0;                             //2 Bytes
  data.Z = gz /131.0;                             //2 Bytes
 // data.tem = ;        //2 Bytes
 // data.hum = ;           //2 Bytes
 // data.pre = ;  //Conversion Pa a hPa     //4 Bytes
 // data.alt = ;   //2 Bytes

  radio.write(&data, sizeof(DataPackage));

 // Serial.print("TEMPERATURA:");
 // Serial.println(data.tem);
 // Serial.print("HUMEDAD:");
 // Serial.println(data.hum);
 // Serial.print("PRESION:");
 // Serial.println(data.pre);
 // Serial.print("ALTITUD:");
 // Serial.println(data.alt);

  Serial.print("GX:  ");
  Serial.println(gx);
  Serial.print("GY:  ");
  Serial.println(gy);
  Serial.print("GZ:  ");
  Serial.println(gz);


  delay(100);
}
