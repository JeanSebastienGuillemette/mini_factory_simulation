# Learning notes — Phase 1

## Servo (SG90) basics
- A servo is controlled by PWM at 50 Hz (a 20 ms period).
- Unlike an LED, the servo doesn't care about the duty-cycle *ratio*:
  it reads the *absolute pulse width* in ms. The width encodes the angle.
- Datasheet says 1–2 ms for the full range, but when calibrating on the
  real SG90, 0.5–2.4 ms gave the full 180° with no buzzing at the ends.
  → Lesson: datasheet values are a starting point; calibrate on hardware.
- Servo power: 5V pin (comes from USB), not 3V3. The servo's ground must
  be shared with the ESP32's ground, otherwise the signal has no reference.

## LEDC (ESP32 hardware PWM)
- A PWM channel (0–15) is an internal hardware generator.
- A GPIO pin is the physical output. ledcAttachPin() connects a pin to a channel.
- The CPU only configures it (ledcSetup / ledcWrite). After that, the hardware
  generates the pulses on its own, with no CPU work.
- Channels share timers in pairs (0-1, 2-3...). Two channels on the same timer
  must have the same frequency → the buzzer (Phase 5) must not use channel 1.
- ledcSetup and ledcWrite take the CHANNEL, not the pin. Passing the pin
  compiles fine (both are int) but silently does nothing. Named constants help.
- The LEDC API changed between Arduino-ESP32 2.x and 3.x. Always check the
  core version first (mine: 2.0.17).

## Duty-cycle math
- 16-bit resolution = 65536 steps for the full 20 ms → about 3277 per ms.
- 0.5 ms → 1638, 2.4 ms → 7864.
- duty = MIN + (angle × (MAX − MIN)) / 180
- Integer division truncates (drops decimals, never rounds).
  → Multiply first, divide last, so the decimals are only lost once.
  (6226 / 180) * 180 = 6120, but (180 * 6226) / 180 = 6226.
- Mega warning: int is 16-bit there (max 32767). 180 * 6226 would overflow.

## Input validation: clamp vs reject
- Clamping: if angle < 0 → 0, if angle > 180 → 180. The function always
  returns a safe value, so no caller can misuse it.
- Rejecting would need an "error" return value, e.g. -1. But ledcWrite takes
  an unsigned value, so -1 becomes a huge number → garbage pulse.
  Every caller would have to check, and one forgetting is enough.

## C++ basics learned
- Every statement ends with ;
- Scope/lifetime: a variable lives inside the { } where it's declared.
  Declared in loop() → recreated and reset every pass. Global → lives forever.
- Pass by value: a function gets a COPY of the argument. Changing it inside
  doesn't change the caller's variable. (By reference: int& — later.)
- Declare a function at the top (prototype), define it below. The compiler
  reads top to bottom and must know a name before it's used.
- "text" + number compiles but prints garbage (pointer arithmetic).
  Use several Serial.print() calls, or Serial.printf("%d\n", x).
  The Mega's Serial has no printf.
- Design: one function, one job. angleToDuty converts, moveServo moves,
  loop() decides the timing.

## Toolchain
- PlatformIO on Windows, not WSL (WSL can't see USB easily).
- monitor_speed = 115200 must match Serial.begin().
- "Wrong boot mode" on upload: hold BOOT during "Connecting...".
  The ESP32 reads a strapping pin at reset to choose run vs download mode.
- Flashing C++ erases MicroPython.