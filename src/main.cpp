#include <Arduino.h>
#include <Servo.h>
#include <stdio.h>

Servo myServo;

const int pollingInterval = 250; //milliseconds

const int trigPin_0 = 2;
const int echoPin_0 = 3;

const int trigPin_1 = 4;
const int echoPin_1 = 5;

const int trigPin_2 = 6;
const int echoPin_2 = 7;

const int trigPin_3 = 8;
const int echoPin_3 = 9;

long duration;
int distance;
int d0;
int d1;
int d2;
int d3;

char sensor_buffer[100];




void aimCannon(int);
int getDistance(int,int);

void setup() {
  Serial.begin(9600);

  // servo
  myServo.attach(9);

  // transducers
  pinMode(trigPin_0, OUTPUT);
  pinMode(echoPin_0, INPUT);
  pinMode(trigPin_1, OUTPUT);
  pinMode(echoPin_1, INPUT);
  pinMode(trigPin_2, OUTPUT);
  pinMode(echoPin_2, INPUT);
  pinMode(trigPin_3, OUTPUT);
  pinMode(echoPin_3, INPUT);

}

void loop() {

  // calculate distances from the sensor array
  d0 = getDistance(trigPin_0,echoPin_0);
  delay(pollingInterval);
  d1 = getDistance(trigPin_1,echoPin_1);
  delay(pollingInterval);
  d2 = getDistance(trigPin_2,echoPin_2);
  delay(pollingInterval);
  d3 = getDistance(trigPin_3,echoPin_3);
  delay(pollingInterval);

  sprintf(sensor_buffer,"d0:%d, d1:%d, d2:%d, d3:%d", d0, d1, d2, d3);

  Serial.println(sensor_buffer);
}

int getDistance(int trigPin, int echoPin ) {

  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);
  distance = duration * 0.034 / 2;

  return distance;
}

void aimCannon(int pos) {
  // pos sweeps servo from 0 to 180 degrees
  myServo.write(pos);

  /** 
  // Sweep from 0 to 180 degrees
  for (int pos = 0; pos <= 180; pos += 1) {
    myServo.write(pos);    // Move servo to 'pos'
    delay(15);             // Wait 15ms for the servo to reach the position
  }

  // Sweep from 180 to 0 degrees
  for (int pos = 180; pos >= 0; pos -= 1) {
    myServo.write(pos);
    delay(15);
  }
    **/
}

// put function definitions here:
int myFunction(int x, int y) {
  return x + y;
}