#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_AHTX0.h>
#include <Adafruit_BMP280.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>

// Crear instancias de los sensores
Adafruit_AHTX0 aht;
Adafruit_BMP280 bmp;
Adafruit_MPU6050 mpu;

// Crear instancia de la pantalla OLED
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// Definir los pines de los LEDs y el buzzer
int ledPin = 2;         // LED general
int mpuLedPin = 33;     // LED específico para MPU6050
int buzzerPin = 13;     // Pin del buzzer en ESP32

#define SEALEVELPRESSURE_HPA (1013.25)

void setup() {
  Serial.begin(115200);

  // Configurar los pines de los LEDs y el buzzer como salidas
  pinMode(ledPin, OUTPUT);
  pinMode(mpuLedPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);

  // Hacer sonar el buzzer durante 1 segundo al inicio
  digitalWrite(buzzerPin, HIGH);
  delay(1000);
  digitalWrite(buzzerPin, LOW);

  // Inicializar el sensor AHT20
  if (!aht.begin()) {
    Serial.println("No se encontró el AHT20. Verifique la conexión.");
    while (1);
  }
  Serial.println("AHT20 iniciado correctamente.");

  // Inicializar el sensor BMP280
  if (!bmp.begin(0x77)) { 
    Serial.println("No se encontró el BMP280. Verifique la conexión.");
    while (1);
  }
  Serial.println("BMP280 iniciado correctamente.");

  // Inicializar el sensor MPU6050
  if (!mpu.begin()) {
    Serial.println("No se encontró el MPU6050. Verifique la conexión.");
    mpuLedPin = -1; // Apagar el LED específico si el MPU6050 no está conectado
  } else {
    Serial.println("MPU6050 iniciado correctamente.");
  }

  // Inicializar la pantalla OLED
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("No se encontró la pantalla OLED. Verifique la conexión.");
    while (1);
  }
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);

  // Rotar la pantalla 180 grados
  display.setRotation(2);  

  // Mostrar el texto inicial
  display.setCursor(0, 0);
  display.println("Laika Aerospace");
  display.println("- Laiksat -");
  display.setCursor(0, 20);
  display.println("Addi T.");
  display.println("Julian N.");
  display.println("Juan Hdz.");
  display.display();
  delay(4000); // Esperar 4 segundos para mostrar el texto
}

void loop() {
  // Encender el LED general mientras se envían los datos
  digitalWrite(ledPin, HIGH);

  // Leer datos del sensor AHT20
  sensors_event_t humidity, temp_aht;
  aht.getEvent(&humidity, &temp_aht);

  // Leer altitud del BMP280
  float altitude = bmp.readAltitude(SEALEVELPRESSURE_HPA);

  // Mostrar datos de temperatura y humedad en la pantalla OLED
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 0); 
  display.println("Temp: ");
  display.print(temp_aht.temperature);
  display.println(" C");

  display.print("Hum: ");
  display.print(humidity.relative_humidity);
  display.println(" %");

  display.print("Alt: ");
  display.print(altitude);
  display.println(" m");

  display.display(); // Actualizar la pantalla
  delay(2000); // Esperar 2 segundos

  // Leer datos del sensor MPU6050
  if (mpuLedPin != -1) {
    sensors_event_t a, g, temp;
    mpu.getEvent(&a, &g, &temp);

    // Parpadear el LED específico para MPU6050
    digitalWrite(mpuLedPin, HIGH);
    delay(100); // Encendido del LED por 100 ms
    digitalWrite(mpuLedPin, LOW);
    delay(100); // Apagado del LED por 100 ms

    // Mostrar datos del MPU6050 en la pantalla OLED
    display.clearDisplay();
    display.setTextSize(1);
    display.setCursor(0, 0); 
    display.println("MPU6050");
    display.print("Acel X: ");
    display.print(a.acceleration.x);
    display.println(" m/s^2");
    display.print("Acel Y: ");
    display.print(a.acceleration.y);
    display.println(" m/s^2");
    display.print("Acel Z: ");
    display.print(a.acceleration.z);
    display.println(" m/s^2");

    // Enviar datos al monitor serial
    Serial.println("Datos MPU6050:");
    Serial.print("Acel X: ");
    Serial.print(a.acceleration.x);
    Serial.println(" m/s^2");
    Serial.print("Acel Y: ");
    Serial.print(a.acceleration.y);
    Serial.println(" m/s^2");
    Serial.print("Acel Z: ");
    Serial.print(a.acceleration.z);
    Serial.println(" m/s^2");
  } else {
    // Si el MPU6050 no está conectado, mostrar un mensaje de error en la pantalla OLED
    display.clearDisplay();
    display.setTextSize(1);
    display.setCursor(0, 0); 
    display.println("MPU6050 NO");
    display.println("CONEXION");
  }

  display.display(); // Actualizar la pantalla
  digitalWrite(ledPin, LOW); // Apagar el LED general después de enviar los datos

  delay(2000); // Esperar 2 segundos antes de la siguiente lectura
}
