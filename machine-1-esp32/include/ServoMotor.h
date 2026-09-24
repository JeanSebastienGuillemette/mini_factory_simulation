#pragma once

class ServoMotor {
public:
  ServoMotor(int pin, int channel, int minDuty, int maxDuty);
  void begin();
  void moveTo(int angle);

private:
  static const int FREQUENCY = 50;
  static const int RESOLUTION = 16;

  int m_pin;
  int m_channel;
  int m_minDuty;
  int m_maxDuty;

  int angleToDuty(int angle);
};