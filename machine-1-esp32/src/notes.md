# Learning notes — Machine 1 (ESP32 #1)

Phase numbers follow docs/mini_industrial_embedded_learning_project_context.md.

## Phase 1 — Servo

### Servo (SG90) basics
- A servo is controlled by PWM at 50 Hz (a 20 ms period).
- Unlike an LED, the servo doesn't care about the duty-cycle *ratio*:
  it reads the *absolute pulse width* in ms. The width encodes the angle.
- Datasheet says 1–2 ms for the full range, but when calibrating on the
  real SG90, 0.5–2.4 ms gave the full 180° with no buzzing at the ends.
  → Lesson: datasheet values are a starting point; calibrate on hardware.
- Servo power: 5V pin (comes from USB), not 3V3. The servo's ground must
  be shared with the ESP32's ground, otherwise the signal has no reference.

### LEDC (ESP32 hardware PWM)
- A PWM channel (0–15) is an internal hardware generator.
- A GPIO pin is the physical output. ledcAttachPin() connects a pin to a channel.
- The CPU only configures it (ledcSetup / ledcWrite). After that, the hardware
  generates the pulses on its own, with no CPU work.
- Channels share timers in pairs (0-1, 2-3...). Two channels on the same timer
  must have the same frequency → a later non-50 Hz output (e.g. a buzzer,
  not in the roadmap yet) must not use channel 1.
- ledcSetup and ledcWrite take the CHANNEL, not the pin. Passing the pin
  compiles fine (both are int) but silently does nothing. Named constants help.
- The LEDC API changed between Arduino-ESP32 2.x and 3.x. Always check the
  core version first (mine: 2.0.17).

### Duty-cycle math
- 16-bit resolution = 65536 steps for the full 20 ms → about 3277 per ms.
- 0.5 ms → 1638, 2.4 ms → 7864.
- duty = MIN + (angle × (MAX − MIN)) / 180
- Integer division truncates (drops decimals, never rounds).
  → Multiply first, divide last, so the decimals are only lost once.
  (6226 / 180) * 180 = 6120, but (180 * 6226) / 180 = 6226.
- Mega warning: int is 16-bit there (max 32767). 180 * 6226 would overflow.

### Input validation: clamp vs reject
- Clamping: if angle < 0 → 0, if angle > 180 → 180. The function always
  returns a safe value, so no caller can misuse it.
- Rejecting would need an "error" return value, e.g. -1. But ledcWrite takes
  an unsigned value, so -1 becomes a huge number → garbage pulse.
  Every caller would have to check, and one forgetting is enough.

### C++ basics learned
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

### Toolchain
- PlatformIO on Windows, not WSL (WSL can't see USB easily).
- monitor_speed = 115200 must match Serial.begin().
- "Wrong boot mode" on upload: hold BOOT during "Connecting...".
  The ESP32 reads a strapping pin at reset to choose run vs download mode.
- Flashing C++ erases MicroPython.

## Phase 2 — ON button

### Choosing a GPIO for a button
- Three checks:
  1. Not input-only (GPIO 34–39). Those pins also have NO internal pull-up.
  2. Not a strapping pin (0, 2, 5, 12, 15). The chip reads these ONCE at
     reset to choose its boot mode. A button pressed during reset could
     boot the board into the wrong mode.
  3. Not a flash pin (6–11). They're wired to the SPI flash; touching them
     crashes the chip or prevents booting.
- Sources disagree about GPIO4 being strapping. Espressif's datasheet says
  no → the manufacturer's datasheet wins over tutorials.
- GPIO 25/26 are also DAC outputs. Irrelevant for a digital input.
- Mine: START = 27, OFF = 26, EMERGENCY = 25 (chosen for placement once
  they passed the checks).

### Floating inputs and pull-ups
- A floating input isn't connected to anything that defines its voltage
  (high impedance), so tiny noise flips it → random HIGH/LOW readings.
- INPUT_PULLUP enables an internal resistor (~45 kΩ) between the pin and
  3.3V. THROUGH a resistor, not a wire: a wire would short 3.3V to GND
  when the button is pressed.
- Wiring: GPIO → button → GND. The button never touches 3.3V.
  Pressed current: 3.3 V / 45 kΩ ≈ 0.07 mA, harmless.
- Inverted logic: released = HIGH (1), pressed = LOW (0).
  HIGH and LOW are just 1 and 0.
- 4-leg tactile button: legs are paired internally → use diagonal legs.
  2-leg button: no ambiguity.
- All grounds are shared (buttons, servo, ESP32): the ESP32 measures
  voltages relative to its own GND. Watch for false presses when the servo
  moves (current spikes on the shared ground).
- BOOT button = GPIO0 + pull-up on the board. Held LOW at reset
  → download mode. Same circuit as my buttons: pressed = LOW.
- Wire colors: black = GND, red = power, other colors = signals.

### Busy waiting
- loop() with nothing to wait on runs hundreds of thousands of times per
  second: 100% CPU doing nothing useful.
- On the ESP32, loop() already runs inside a FreeRTOS task. A task that
  never blocks can starve other tasks on its core (Phase 5, FreeRTOS).
- delay() on the ESP32 calls vTaskDelay(): it BLOCKS (gives the CPU away)
  instead of spinning.

### Level vs edge
- Level = the current state ("is it pressed?").
  Edge = the change ("did it just become pressed?").
- Toggling on the level fires every loop while the button is held.
- Falling edge = previous HIGH and current LOW. Needs a variable that
  survives between loop() calls (global), while the current reading is local.

### Bounce and debouncing
- Mechanical contacts bounce for about 1–10 ms: one press = several
  HIGH/LOW flickers. Each flicker is a real edge to the code.
- delay(100) hid the bounce because it read too rarely to see it.
  delay(0) revealed it: sometimes several PRESSED per press.
  But delay-based filtering adds latency and blocks the loop.
- millis() debounce: record the time of the last raw change; accept the
  reading as the stable state only once it has been unchanged for
  DEBOUNCE_TIME_MS (50 ms: well above bounce, well below a ~100–200 ms tap).
  500 ms swallowed normal taps.
- The initial stableState must match the real state at boot (HIGH),
  otherwise boot looks like an event.
- Detecting a stable CHANGE fires on press AND release → filter on
  stableState == LOW to react only to the press.
- Tested: hands-off = nothing, long hold = one PRESSED, 10 taps = 10.

### C++: types and naming
- C++ is statically typed like Java: digitalRead() returns int, so use int.
- `string` doesn't exist on its own: it's std::string (the std namespace,
  similar to a Java package).
- printf trusts the format string: %d with a non-int reads garbage.
  This is undefined behavior, with no exception like in Java.
- The size of int/long depends on the platform. ESP32: int and long are
  both 32 bits. Mega: int is 16 bits. Java's long is always 64 bits.
  uint32_t = exactly 32 bits when the size matters.
- unsigned = no negatives, double the positive range.
- millis() returns unsigned long: wraps to 0 after about 49.7 days.
  `millis() - last >= DELAY` stays correct across the wrap (the unsigned
  subtraction wraps too). A 16-bit int would break after 32.7 s.
- Naming: ALL_CAPS = constants, camelCase = variables. Put the unit in the
  name (DEBOUNCE_TIME_MS).
- config.h holds constants only. A non-const variable defined in a header
  included by two .cpp files → linker error (multiple definition).

### Button class
- Built by MOVING the working main.cpp debounce code into a class, not
  rewriting it: the globals became private members (m_lastReading,
  m_stableState, m_lastChangeTime). One object per button → three buttons
  = three objects, each with its own state. With globals, three buttons
  would need three copies of every variable.
- Constructor takes the pin AND the debounce time. The class never
  includes config.h: main.cpp passes the values in (same as ServoMotor).
- Initializer list for all members, m_ prefix: same conventions across
  the project.
- API mirrors level vs edge:
  - isPressed(): true while held (debounced level).
  - wasPressed(): true once per press, then resets (edge event).
- update() samples the pin and runs the debounce. It must run EVERY loop,
  because the debounce measures time. wasPressed() is called whenever the
  logic cares. The m_pressedEvent flag decouples sampling from consuming.
  → It's a one-slot queue. In Phase 5 (FreeRTOS), ButtonTask produces events,
    MachineTask consumes them, and a FreeRTOS queue replaces the flag.

### const methods
- `bool isPressed() const;` promises the method doesn't modify any member.
  The compiler rejects any mutation inside it.
- wasPressed() can't be const: it clears m_pressedEvent.
- Only const methods can be called through a `const Button&` (later).

### Flag vs count
- A bool flag says "something happened", not "how many times".
  Two presses between two wasPressed() calls → true, true → seen as ONE.
- Fine for START (one press or two, same result). Wrong for a production
  counter: it silently UNDERCOUNTS.
- Fix: an int counter, or a queue that keeps every event in order.

### Header comments = the contract
- Users read the .h, not the .cpp. Usage rules go in the header:
  "Must be called every loop(). Presses are missed otherwise."
- Header comments say WHAT a method does and what the caller must do.
  HOW it works (millis, no blocking) belongs in the .cpp.
- Don't comment what the signature already shows (e.g. what const means).

### delay() vs non-blocking timing (next step)
- The sweep uses delay(1000) × 3. During those 3 s, update() never runs:
  - a short press is NEVER seen;
  - a held press is detected up to 3 s LATE.
  → Unacceptable for START, dangerous for EMERGENCY.
- Fix: the same pattern as the debounce. Every loop: "has 1000 ms passed
  since the last move?" No → return immediately. Yes → move to the next
  step, note the time.
- State to keep: the step index in the sequence + the time of the last move.
- The sweep lives in main.cpp for now. It's machine BEHAVIOR, not a servo
  property: ServoMotor stays generic (moveTo an angle). Later it moves to a
  machine class (Phase 4 state machine) / MachineTask (Phase 5).
- This is what FreeRTOS generalizes in Phase 5.

### Small habits
- SERIAL_BAUD_RATE in config.h. It must match monitor_speed in
  platformio.ini (two places, same value → comment the link).
- Using code I didn't write is fine IF I can explain every line.

### Workflow habits
- BUILD before pasting code. The compiler catches mistakes in seconds.
- "It doesn't work" isn't a bug report: give the output, what I did, and
  what I expected.
- IntelliSense: open the folder that contains platformio.ini.
- Predict the output BEFORE running a test.

### Open questions
- Why is a real industrial e-stop wired normally closed? What happens if a
  wire breaks in each design? (Phase 4, EMERGENCY button)
