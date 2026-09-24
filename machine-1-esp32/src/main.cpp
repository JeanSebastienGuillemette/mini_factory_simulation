#include <Arduino.h>
#include "config.h"
#include "ServoMotor.h"

ServoMotor servo(SERVO_PIN, SERVO_CHANNEL, SERVO_MIN_DUTY, SERVO_MAX_DUTY);

void setup() {
  Serial.begin(115200);
  servo.begin();
}

void loop() {
  if (SERVO_ENABLED) {
    servo.moveTo(0);
    delay(1000);
    servo.moveTo(90);
    delay(1000);
    servo.moveTo(180);
    delay(1000);
  }
}
