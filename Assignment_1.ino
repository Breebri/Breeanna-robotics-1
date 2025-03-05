#include <Servo.h>
Servo myservo;

const int sound = A0;
const int Lg = 8;
const int Ly = 5;
const int Lr = 9;

const int button = 2;
const int buzz = 13;

const int trigpin = 3;
const int echopin = 7;
const int servopin = 6;

const int In1 = 12;
const int In2 = 4;
const int EnA = 11;

const int pot = A5;

void setup() 
{
  Serial.begin(9600);
  pinMode(sound, INPUT);
  pinMode(Lg, OUTPUT);
  pinMode(Ly, OUTPUT);
  pinMode(Lr, OUTPUT);

  pinMode(button, INPUT);
  pinMode(buzz, OUTPUT);

  pinMode(trigpin, OUTPUT);
  pinMode(echopin, INPUT);

  myservo.attach(10);

  pinMode(EnA, OUTPUT);
  pinMode(In1, OUTPUT);
  pinMode(In2, OUTPUT);

  pinMode(pot, INPUT);
}

void loop()
{
  LEDs();
  Buzzer();
  Servo();
  Motor();
}

void LEDs()
{ int val = analogRead(sound);
  val = map(val, 0, 1023, 0, 255);
  Serial.println(val);
  delay(10);

  if(val<20)
  { digitalWrite(Lg, HIGH);
    digitalWrite(Ly, LOW);
    digitalWrite(Lr, LOW);
  }

  else if(20<val<50)
  { digitalWrite(Ly, HIGH);
    digitalWrite(Lg, LOW);
    digitalWrite(Lr, LOW);
  }
  else
  { digitalWrite(Lr, HIGH);
    digitalWrite(Lg, LOW);
    digitalWrite(Ly, LOW);
  }

}

void Buzzer()
{
  if(digitalRead(button)==HIGH)
  {
    tone(buzz, 500);
  }
  else
  { 
    noTone(buzz);
  }
}

void Servo()
{
  long duration, cm;
  digitalWrite(trigpin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigpin, HIGH);
  delayMicroseconds(5);
  digitalWrite(trigpin, LOW);

  duration  = pulseIn(echopin, HIGH);

  cm = microsecondsToCentimeters(duration);

  Serial.print(cm);
  Serial.print("cm");
  Serial.println();
  delay(100);

  if(cm < 30){
    myservo.write(20);
    delay(100);
  } else {
    myservo.write(0);
    delay(100);
  }
}

long microsecondsToCentimeters(long microseconds)
{
   return microseconds/29/2;
}

void Motor()
{
  int input = analogRead(pot);
  Serial.println(input);
  if(0<input<257)
  {
    digitalWrite(In1, HIGH);
    digitalWrite(In2, LOW);
    analogWrite(EnA, 64);
  } else if(257<input<509)
  {
    digitalWrite(In1, HIGH);
    digitalWrite(In2, LOW);
    analogWrite(EnA, 127);
  } else if(509<input<768)
  {
    digitalWrite(In1, HIGH);
    digitalWrite(In2, LOW);
    analogWrite(EnA, 191);
  } else if(input>768)
  {
    digitalWrite(In1, HIGH);
    digitalWrite(In2, LOW);
    analogWrite(EnA, 255);
  } else 
  {
    digitalWrite(In1, LOW);
    digitalWrite(In2, LOW);
  }
  
}
