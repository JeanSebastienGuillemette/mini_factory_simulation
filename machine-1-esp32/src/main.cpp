#include <Arduino.h>

// put function declarations here:
const int SERVO_PIN = 13;
const int FREQUENCY = 50;
const int RESOLUTION = 16;
const int MIN_ENDPOINT = 1638;
const int MAX_ENDPOINT = 7864;
const int SERVO_CHANNEL = 0;
const bool SERVO_ENABLED = false;

void moveServo(int angle);
int angleToDuty(int angle);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
 
  ledcSetup(SERVO_CHANNEL, FREQUENCY, RESOLUTION);
  ledcAttachPin(SERVO_PIN, SERVO_CHANNEL);
}

void loop() {
  // put your main code here, to run repeatedly:
  if (SERVO_ENABLED) {
    moveServo(0);
    delay(1000);
    moveServo(90);
    delay(1000);
    moveServo(180);
    delay(1000);
  }
}

// put function definitions here:
int angleToDuty(int angle) {
  if (angle < 0) {
    angle = 0;
  } else if (angle > 180) {
    angle = 180;
  }
  int output = (MIN_ENDPOINT + (angle * (MAX_ENDPOINT - MIN_ENDPOINT)) / 180);
  return output;
}

void moveServo(int angle) {
  int duty = angleToDuty(angle);
  ledcWrite(SERVO_CHANNEL, duty);
  Serial.printf("angle: %d duty: %d\n", angle, duty);
}
