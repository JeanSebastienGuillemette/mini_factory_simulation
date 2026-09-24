#pragma once

// Serial
const unsigned long SERIAL_BAUD_RATE = 115200;  // must match monitor_speed in platformio.ini

// Servo
const int SERVO_PIN = 13;
const int SERVO_CHANNEL = 0;
const int SERVO_MIN_DUTY = 1638;  // 0.5 ms pulse = 0°
const int SERVO_MAX_DUTY = 7864;  // 2.4 ms pulse = 180°

// Button
const int START_BUTTON_PIN = 27;
const int OFF_BUTTON_PIN = 26;
const int EMERGENCY_BUTTON_PIN = 25;
const unsigned long DEBOUNCE_TIME_MS = 50;

// Debug
const bool SERVO_ENABLED = false;
