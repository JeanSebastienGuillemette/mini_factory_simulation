#pragma once

class Button {
public:
  Button(int pin, unsigned long debounceMs);
  void begin();
  void update();
  bool isPressed() const;
  bool wasPressed();

private:
  int m_pin;
  unsigned long m_debounceMs;
  int m_lastReading;
  int m_stableState;
  unsigned long m_lastChangeTime;
  bool m_pressedEvent;
};
