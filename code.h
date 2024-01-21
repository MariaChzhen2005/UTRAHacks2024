#include <Wire.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>

Adafruit_MPU6050 mpu;

const int trigPin = 2; 
const int echoPin = 3; 
const int photoSensor1 = A0; 
const int photoSensor2 = A1;
const int greenLED = 4;
const int yellowLED = 5;
const int redLED = 6;

void setup() {
  Serial.begin(9600);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(greenLED, OUTPUT);
  pinMode(yellowLED, OUTPUT);
  pinMode(redLED, OUTPUT);

  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    while (1);
  }
}

void loop() {
  float distance = measureDistance();
  float posture = measurePosture();
  int ambientLight = measureAmbientLight();

  // Problem 1: User too close to the screen
  if (distance < 30) {
    digitalWrite(greenLED, HIGH);  // Turn ON green LED
  } else {
    digitalWrite(greenLED, LOW);   // Turn OFF green LED
  }

  // Problem 2: Slouching detected
  if (posture > 45) {
    digitalWrite(yellowLED, HIGH);  // Turn ON yellow LED
  } else {
    digitalWrite(yellowLED, LOW);   // Turn OFF yellow LED
  }
// Problem 3: Dark environment
  if (ambientLight < 510) {
    digitalWrite(redLED, HIGH);  // Turn ON red LED
  } else {
    digitalWrite(redLED, LOW);   // Turn OFF red LED
  }

  delay(500); // Adjust delay based on your requirements
}

float measureDistance() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  float duration = pulseIn(echoPin, HIGH);
  float distance = duration * 0.034 / 2;
  return distance;
}

float measurePosture() {
  sensors_event_t accelEvent, gyroEvent, tempEvent;
  mpu.getEvent(&accelEvent, &gyroEvent, &tempEvent);
  float inclination = atan2(-accelEvent.acceleration.x, accelEvent.acceleration.y) * 180.0 / PI;
  return abs(inclination);
}

int measureAmbientLight() {
  int light1 = analogRead(photoSensor1);
  int light2 = analogRead(photoSensor2);
  return (light1 + light2) / 2;
}
