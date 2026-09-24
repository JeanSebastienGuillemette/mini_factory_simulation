#pragma once

class Button {
public:
  Button(int pin, unsigned long debounceMs);
  void begin();
  void update(); // Must be called every loop(). Presses are missed otherwise.
  bool isPressed() const; // True when the button is held down (debounced). INPUT_PULLUP: pressed = LOW.
  bool wasPressed(); // True once per press, then resets until next press

private:
  int m_pin;
  unsigned long m_debounceMs;
  int m_lastReading;
  int m_stableState;
  unsigned long m_lastChangeTime;
  bool m_pressedEvent;
};
