//Necesario para el control de la LCD
#include <LiquidCrystal.h>             //Libreria para LCD
LiquidCrystal LCD(7, 6, 5, 4, 3, 2);   //Pines al arduino

//Necesario para sensor de temperatura
#include "DHT.h"                       //Libreria del sensor DTH11/DTH22
#define DHTPIN 8                       //Pin al arduino   
#define DHTTYPE DHT22                  //Tipo del sensor 
DHT dht(DHTPIN, DHTTYPE);              //Se inicia una variable que será usada por Arduino para comunicarse con el sensor

//Necesario para que trabaje el modulo LDR
int ledPin = 12;                       //Pin al arduino
const int valorLimite = 10;            //Umbral de la fotoresistencia
float Rsensor;                         //Captura del nivel de luz

//Necesario para sensor de humedad en tierra
int GNDSensor = A1;                   //Pin al arduino
int StdHumedo = 700;                  //Umbral donde la tierra esta humeda
int StdSeco = 500;                    //Umbral donde la tierra esta seca

//Necesario para el motorDC de 12V que sera la bomba de agua
int bombaDC = 14;                     //Pin al arduino

//Necesario para Ventiladores del sistema
int Vex = 15;                         //Extractor de aire caliente
int Vin = 16;                         //Inyector de aire caliente

void setup() {
  LCD.begin(16, 2);                   //Dimensión de la LCD
  LCD.clear();                        //Limpieza de la LCD
  
  pinMode(ledPin, OUTPUT);            //Inicialización LED indicador de LDR
   
  pinMode(bombaDC, OUTPUT);           //Inicialización de la bomba de agua 
  
  Serial.begin(9600);                 //Inicialización del monitor serial a 9600 baudios
  
  dht.begin();                        //Inicialización del sensor DTH22
}

void loop() {
  //Estos son codigos de prueba individual de cada sensor en el sistema
  //pruebaLCD();
  //pruebaTemp();
  //pruebaGND();
  //pruebaLDR();
  pruebaMDC();

}
void pruebaMDC() {
 digitalWrite(bombaDC, LOW);
}
void pruebaLDR() {
  int sensorValue = analogRead(0);

  Rsensor = (float)(1023 - sensorValue) * 10 / sensorValue;

  if (Rsensor > valorLimite)
  {
    digitalWrite(ledPin, HIGH);
  }
  else
  {
    digitalWrite(ledPin, LOW);
  }
  Serial.print("LDR: ");
  Serial.print(sensorValue);
  Serial.print("         Resistencia LDR: ");
  Serial.println(Rsensor, DEC);
  delay(1000);
}

void pruebaGND() {
  int valorSensor;
  valorSensor = analogRead(GNDSensor);

  if (valorSensor >= StdHumedo ) {
    Serial.print("Tierra Humeda: ");
    Serial.println (valorSensor);
    Serial.println();
    delay(1000);
  }
  else if (valorSensor <= StdSeco) {
    Serial.print("Tierra Seca: ");
    Serial.println(valorSensor);
    Serial.println();
    delay(1000);
  }
}

void pruebaTemp() {
  float h = dht.readHumidity(); //Se lee la humedad
  float t = dht.readTemperature(); //Se lee la temperatura
  //Se imprimen las variables
  Serial.print("Humedad: ");
  Serial.print(h);
  Serial.print("   Temperatura: ");
  Serial.println(t);
  delay(2000); //Se espera 2 segundos para seguir leyendo //datos
}

void pruebaLCD() {
  LCD.setCursor(0, 0);
  LCD.print("probando LCD");

  LCD.setCursor(0, 1);
  LCD.print("Te gusta? Ale Angel");
  LCD.scrollDisplayLeft();
  delay (500);
}

