/***********************
* Pines de los motores *
************************/
//Dirección del motor derecho
#define rMotorH 4 //Positivo
#define rMotorL 7 //Negativo
//Dirección del motor izquierdo
#define lMotorH 2 //Positivo
#define lMotorL 3 //Negativo
//Velocidad de cada motor
#define rSpeed 5 //motor derecho
#define lSpeed 6 //motor izquierdo

/****************************
* Pines de los ultrasónicos *
*****************************/
//Triggers de los ultrasónicos
#define triggerPin1 8
#define triggerPin2 10
#define triggerPin3 12
//Echos de los ultrasónicos
#define echoPin1 9
#define echoPin2 11
#define echoPin3 13

/****************
* Pin del botón *
*****************/
#define bttnPin A0

/************
* Librerías *
*************/
#include <driver.h>
#include <ultrasonic.h>
#include <din.h>

/*********************
* Variables globales *
**********************/
Driver car(rMotorH, rMotorL, lMotorH, lMotorL, rSpeed, lSpeed); //Controlador de los motores
Ultrasonic s1(triggerPin1, echoPin1); //Cabeza
Ultrasonic s2(triggerPin2, echoPin2); //Izquierda
Ultrasonic s3(triggerPin3, echoPin3); //Derecha
Din bttn(bttnPin); //Boton inicio/reiniciar

const int maxSpeed = 150; //Máxima velocidad autorizada
const int turnSpeed = 120; //Velocidad de giro

const int turnTime = 500;
const int microadvance = 1500;

bool right, left, front; //Variables para verificar si hay espacio con los sensores

/************
* Algoritmo *
*************/
void setup() 
{
  bttn.WaitUntilChange(HIGH);
  delay(500); //tiempo para quitar la mano
  s1.GetRawTime();
 // Serial.begin(9600);
  int d;
  do
  {
    d = s1.Measure();
    car.Forward(180);
    delay(10);
    //Serial.println(d);
  }
  while(d >= 17);
}
void loop()
{
  //Verificar distancias
  verify();
  solve(); //Resolver el laberinto
  //Si hay espacio en toda dirección
}

/**********************
* Funciones y métodos *
**********************/

void solve()
{
  if(right)
  {
    car.Forward(maxSpeed);
    delay(microadvance);
    //Girar 90° a la derecha
    car.Right(turnSpeed);
    delay(turnTime);
    //Avanzar 15 cm
    car.Forward(maxSpeed);
    delay(microadvance);
    delay(microadvance);
  }
  else if(front)
  {
    //Avanzar hasta que front sea false
    car.Forward(maxSpeed);
  }
  /*else if(left)
  {
    //Girar 90° a la izquierda
    car.Left(turnSpeed);
    delay(turnTime);
    //Avanzar 15 cm
    car.Forward(maxSpeed);
    delay(microadvance);
  }*/
  else
  {
    //Girar 180° a la derecha
    car.Left(turnSpeed);
    delay(turnSpeed);
  }
}

void verify()
{
  right = s3.Measure() >= 22; //Hay más de 20 cm a la derecha?
  front = s1.Measure() >= 22; //Hay más de 20 cm al frente?
  //left = s2.Measure() >= 10; //Hay más de 20 cm a la izquierda?
}

void walk()
{
  long distance = s1.Measure();
  while(s1.Measure() > 5)
  {
    car.Forward(turnSpeed);
  }
}
