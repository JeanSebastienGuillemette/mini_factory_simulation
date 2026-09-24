#include <Arduino.h>
#include "ServoMotor.h"

ServoMotor::ServoMotor(int pin, int channel, int minDuty, int maxDuty)
    : m_pin(pin), m_channel(channel), m_minDuty(minDuty), m_maxDuty(maxDuty) {}

void ServoMotor::begin()
{
  ledcSetup(m_channel, FREQUENCY, RESOLUTION);
  ledcAttachPin(m_pin, m_channel);
}

void ServoMotor::moveTo(int angle)
{
  int duty = angleToDuty(angle);
  ledcWrite(m_channel, duty);
  Serial.printf("angle: %d duty: %d\n", angle, duty);
}

int ServoMotor::angleToDuty(int angle)
{
  if (angle < 0)
  {
    angle = 0;
  }
  else if (angle > 180)
  {
    angle = 180;
  }
  return m_minDuty + (angle * (m_maxDuty - m_minDuty)) / 180;
}
