#include <Arduino.h>
#include "Button.h"

Button::Button(int pin, unsigned long debounceMs)
    : m_pin(pin), m_debounceMs(debounceMs), m_lastReading(HIGH), m_stableState(HIGH),
      m_lastChangeTime(0), m_pressedEvent(false) {}

void Button::begin()
{
  pinMode(m_pin, INPUT_PULLUP);
}

// Call once per loop(): millis() debounce, no blocking.
void Button::update()
{
  int currentReading = digitalRead(m_pin);

  if (currentReading != m_lastReading)
  {
    m_lastChangeTime = millis();
    m_lastReading = currentReading;
  }

  if ((millis() - m_lastChangeTime) >= m_debounceMs && m_stableState != m_lastReading)
  {
    m_stableState = m_lastReading;

    if (m_stableState == LOW)
    {
      m_pressedEvent = true;
    }
  }
}

// True while the button is held down (debounced). INPUT_PULLUP: pressed = LOW.
bool Button::isPressed() const
{
  return m_stableState == LOW;
}

// True once per press, then resets until the next press.
bool Button::wasPressed()
{
  bool pressed = m_pressedEvent;
  m_pressedEvent = false;
  return pressed;
}
