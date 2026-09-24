#include <Arduino.h>
#include "config.h"
#include "ServoMotor.h"
#include "Button.h"

ServoMotor servo(SERVO_PIN, SERVO_CHANNEL, SERVO_MIN_DUTY, SERVO_MAX_DUTY);
Button startButton(START_BUTTON_PIN, DEBOUNCE_TIME_MS);

void setup()
{
  Serial.begin(SERIAL_BAUD_RATE);
  servo.begin();
  startButton.begin();
}

void loop()
{
  startButton.update();

  if (startButton.wasPressed())
  {
    Serial.println("PRESSED");
  }
}
