#include <Arduino.h>
#include <Servo.h>
#include <stdio.h>


// peripherals 
const int trigPin_0 = 2;
const int echoPin_0 = 3;
const int trigPin_1 = 4;
const int echoPin_1 = 5;
const int trigPin_2 = 6;
const int echoPin_2 = 7;
const int trigPin_3 = 8;
const int echoPin_3 = 9;
const int servoPin  = 10;
const int armBtn    = 11;
const int armLED    = 12;
const int relayPin  = 13;

// sensor configs
const int pollingInterval = 100; //milliseconds
const int maxDist = 40;
const int triggerDuration = 500;

// calculated values and storage
char dataBuff[100];
long duration;
int distance;
int armState = 0;
int trigger = 0;

Servo myServo;

// sensor data stores
typedef struct {
  int d0,d1,d2,d3;
} Sensors;

Sensors data ={-1}; 


void aimCannon(Sensors data);
void pollSensors(Sensors *data);
void fireCannon();
int checkArmButton(int);
int getDistance(int,int);

void setup() {
  Serial.begin(9600);

  // servo
  myServo.attach(10);

  // transducers
  pinMode(trigPin_0, OUTPUT);
  pinMode(echoPin_0, INPUT);
  pinMode(trigPin_1, OUTPUT);
  pinMode(echoPin_1, INPUT);
  pinMode(trigPin_2, OUTPUT);
  pinMode(echoPin_2, INPUT);
  pinMode(trigPin_3, OUTPUT);
  pinMode(echoPin_3, INPUT);

  pinMode(armBtn, INPUT);
  pinMode(armLED, OUTPUT);
  pinMode(relayPin, OUTPUT);

}

void loop() {

  armState = checkArmButton(armState);

  pollSensors(&data);
  sprintf(dataBuff,"%d, %d,%d,%d", data.d0, data.d1, data.d2, data.d3);
  Serial.println(dataBuff);

  aimCannon(data);

  if (trigger > 0 ){
    fireCannon();
  };
}

void fireCannon(){
  if (armState) {
    delay(500);
    digitalWrite(relayPin, HIGH);
    delay(triggerDuration);
    digitalWrite(relayPin, LOW);
  };

};

int checkArmButton(int armState) {
  if (digitalRead(armBtn)){
    delay(50); //debounce so we dont have multiple triggers

    switch (armState) {
      case 0:
        digitalWrite(armLED,HIGH);
        return 1;
      case 1:
        digitalWrite(armLED, LOW);
        return 0;
    }
  }
  return armState;
};

void pollSensors(Sensors *data) {

  //Sensors data = {0};
  // calculate distances from the sensor array
  data->d0 = getDistance(trigPin_0,echoPin_0);
  delay(pollingInterval);
  data->d1 = getDistance(trigPin_1,echoPin_1);
  delay(pollingInterval);
  data->d2 = getDistance(trigPin_2,echoPin_2);
  delay(pollingInterval);
  data->d3 = getDistance(trigPin_3,echoPin_3);
};

int getDistance(int trigPin, int echoPin ) {

  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);
  distance = duration * 0.034 / 2;

  if (distance > maxDist) {
    distance = -1;
  };

  return distance;
};

void aimCannon(Sensors data) {
  // pos sweeps servo from 0 to 180 degrees
  if (data.d0 > 0){
    myServo.write(30);
    trigger=1;
  } else if (data.d1>0 ){
    myServo.write(60);
    trigger=1;
  } else if (data.d2>0 ){
    myServo.write(120);
    trigger=1;
  } else if (data.d3>0 ){
    myServo.write(150);
    trigger=1;
  } else {
    trigger = 0;
  }
};
