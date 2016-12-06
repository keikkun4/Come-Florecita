/*Ambos ventiladores trabajen simultaneamente caliente y que LED se encienda cuando esta frio
 *Variable del sensor de fotoresistencia como contramedida de conteo de horas. 
 *2 tiras de Led una para justificar calentador. 
 */
//Necesario para el control de la LCD
#include <LiquidCrystal.h>             //Libreria para LCD
LiquidCrystal LCD(7, 6, 5, 4, 3, 2);   //Pines al arduino

//Necesario para sensor de temperatura
#include "DHT.h"                       //Libreria del sensor DTH11/DTH22
#define DHTPIN 8                       //Pin al arduino   
#define DHTTYPE DHT22                  //Tipo del sensor 
DHT dht(DHTPIN, DHTTYPE);              //Se inicia una variable que será usada por Arduino para comunicarse con el sensor

float frio = 25.00;          //Señal del sensor sobre que el ambiente es demasiado frio
float caliente = 35.00;      //Señal del sensor sobre que el ambiente es demasiado caliente

//Necesario para que trabaje el modulo LDR
int LEDES  = 11;                       //Tira de LEDES
int ledPin = 12;                       //Pin al arduino

//Necesario para sensor de humedad en tierra
int GNDSensor = A1;                   //Pin al arduino

//Necesario para el motorDC de 12V que sera la bomba de agua
int bombaDC = 14;                     //Pin al arduino

//Necesario para Ventiladores del sistema
int Vex = 15;                         //Extractor de aire caliente
int Vin = 16;                         //Inyector de aire caliente

void setup() {
  LCD.begin(16, 2);                   //Dimensión de la LCD
  LCD.clear();                        //Limpieza de la LCD

  pinMode(LEDES, OUTPUT);
  pinMode(ledPin, OUTPUT);            //Inicialización LED indicador de LDR
  pinMode(bombaDC, OUTPUT);           //Inicialización de la bomba de agua
  pinMode(Vex, OUTPUT);               //Inicializacion de extractor
  pinMode(Vin, OUTPUT);               //Inicializacion de inyector

  Serial.begin(9600);                 //Inicialización del monitor serial a 9600 baudios

  dht.begin();                        //Inicialización del sensor DTH22
}

void loop() {

  ctrlVentiladores();
  ctrlBombaDC();
  ctrlLEDes();

}

void ctrlVentiladores() {
  float tmp = dht.readTemperature(); //Se lee la temperatura del ambiente

  Serial.print("Temperatura: ");     //Se imprimen la temperatura del ambiente
  Serial.println(tmp);               //Se captura dato de temperatura del sensor
  delay(3000);                       //Latencia de 2 segundos (2000 milisengundos)

  if (tmp <= frio)                   //Si el ambiente esta frio
  {
    digitalWrite(Vin, HIGH);         //Encender el ventilador inyector de aire caliente
  }
  else if (tmp >= caliente)          //Si el ambiente esta caliente
  {
    digitalWrite(Vex, HIGH);         //Encender el ventilador que es extractor de aire caliente
  }
  else                               //Si la temperatura esta tibia
  {
    digitalWrite(Vin, LOW);          //Apagar inyector de aire caliente
    digitalWrite(Vex, LOW);          //Apagar extractor de aire caliente
  }
} /*Fin del ctrlVentiladores*/

void ctrlBombaDC() {
  int valorSensor;
  valorSensor = analogRead(GNDSensor);  //Captura de la señal del sensor de humedad en tierra

  int StdHumedo = 700;                  //Umbral donde la tierra esta humeda
  int StdSeco = 500;                    //Umbral donde la tierra esta seca

  int espera = 2000;                    //Cada segundo refrescar data

  if (valorSensor >= StdHumedo ) {
    digitalWrite(bombaDC, HIGH);

    Serial.print("Tierra Humeda: ");
    Serial.println (valorSensor);
    Serial.print("Bomba: APAGADA");
    Serial.println();
    delay(espera);
  }
  else if (valorSensor <= StdSeco) {
    digitalWrite(bombaDC, LOW);

    Serial.print("Tierra Seca: ");
    Serial.println(valorSensor);
    Serial.print("Bomba: ENCENDIDA");
    Serial.println();
    delay(espera);
  }
}

void ctrlLEDes() {
  int espera = 500;                      //Refrescar cada 500 milisengundos
  int valorSensor = analogRead(0);       //Captura del nivel de luz
  float Rsensor;
  Rsensor = ((float)(1023 - valorSensor) * 10 / valorSensor); //Resistencia del LDR

  const int valorLimite = 10;            //Umbral de la fotoresistencia

  Serial.print("Estado LDR: ");
  Serial.print(valorSensor);
  Serial.print(" Resistencia: ");
  Serial.println(Rsensor, DEC);
  delay(espera);

  if (Rsensor >= valorLimite)            //Si no hay luz en en el invernadero
  {
    digitalWrite(LEDES,  HIGH);          //Encender tira de LEDES
    digitalWrite(ledPin, HIGH);          //Encender LED indicador

    Serial.print("Estado de luces: Encendidas");
    Serial.println();
    delay(espera);
  }
  else
  {
    digitalWrite(LEDES,  LOW);          //Apagar tira de LEDES
    digitalWrite(ledPin, LOW);          //Apagar LED indicador

    Serial.print("Estado de luces: Apagadas");
    Serial.println();
    delay(espera);
  }
  
Serial.println();
delay(1000);
}


