# Changelog — Mini Factory Simulation

Step-by-step progress log for the mini industrial embedded learning project.
See [mini_industrial_embedded_learning_project_context.md](docs/mini_industrial_embedded_learning_project_context.md) for the full roadmap.

**Legend:** `[ ]` Not started · `[~]` In progress · `[x]` Complete

**Workflow for every step:**
1. Build the hardware → 2. Smallest possible test program → 3. Understand the code → 4. Add exactly one concept → 5. Test again → 6. Log it here → continue

**Entry template** (copy into the [Log](#log) when you complete a step, oldest first):

```text
### YYYY-MM-DD — Phase X: <step>
- Built/wired:
- Learned (the WHY):
- Issues/fixes:
```

---

## Current status

| Phase | Machine | Topic | Status |
|------:|---------|-------|:------:|
| 1  | M1 — ESP32 #1   | Servo works                    | [x] |
| 2  | M1 — ESP32 #1   | ON button                      | [~] |
| 3  | M1 — ESP32 #1   | OFF button                     | [ ] |
| 4  | M1 — ESP32 #1   | EMERGENCY button + state machine | [ ] |
| 5  | M1 — ESP32 #1   | FreeRTOS                       | [ ] |
| 6  | M1 — ESP32 #1   | Dual-core experiments          | [ ] |
| 7  | M2 — ESP32 #2   | Potentiometer + ADC            | [ ] |
| 8  | M2 → M1         | ESP32-to-ESP32 communication   | [ ] |
| 9  | M3 — Mega 2560  | Mock sensor data               | [ ] |
| 10 | M3 → Pi         | UART to Raspberry Pi           | [ ] |
| 11 | Pi 5            | Status LEDs                    | [ ] |
| 12 | Pi 5            | I2C LCD                        | [ ] |
| 13 | All             | Full integration               | [ ] |

---

## Phase 1 — Machine 1: Make the Servo Work

`ESP32 #1 → Servo` · **No FreeRTOS yet.**

Learn: GPIO output, PWM, servo control, power considerations, basic timing.

- [x] Servo physically connected (signal, 5 V supply, common GND)
- [x] Basic servo test (servo moves)
- [x] Servo moves to known positions (0°, 90°, 180°)
- [x] I can explain how the servo signal works (pulse width → angle)

---

## Phase 2 — Add the ON Button

`ON button → ESP32 → Servo` · **Still no RTOS.**

Behavior: press ON → machine becomes ON → servo starts/moves.

Learn: digital input, pull-up/pull-down, button state, basic state handling.

- [x] ON button wired (with pull-up or pull-down chosen and understood)
- [x] Button state read and printed to Serial
- [ ] Pressing ON starts the servo
- [x] I can explain why a floating input is a problem

---

## Phase 3 — Add the OFF Button

States: `OFF` · `RUNNING`

Learn: multiple inputs, state management, separating input handling from actuator behavior.

- [ ] OFF button wired and read
- [ ] Machine state variable (`OFF` / `RUNNING`)
- [ ] Input handling separated from servo behavior
- [ ] ON/OFF toggles the machine reliably

---

## Phase 4 — Add the EMERGENCY Button

States: `OFF` · `RUNNING` · `EMERGENCY`

Emergency must immediately put the machine in a safe software state.

Learn: state machines, event priority, emergency/safety-state thinking.

- [ ] EMERGENCY button wired and read
- [ ] `EMERGENCY` state added; servo goes to a safe position/stops
- [ ] EMERGENCY overrides ON/OFF (priority of events)
- [ ] Defined how to leave EMERGENCY (reset rule)
- [ ] Basic machine state machine complete and drawn as a diagram

---

## Phase 5 — Introduce FreeRTOS

Only after Phases 1–4 work without RTOS.

```text
ButtonTask ──command──> Queue ──> MotorTask ──> Servo
```

Learn in this order — one at a time:

- [ ] 1. Task creation (`ButtonTask`, `MotorTask`)
- [ ] 2. Task delay (`vTaskDelay`)
- [ ] 3. Task priorities
- [ ] 4. Queue between ButtonTask and MotorTask
- [ ] 5. Task synchronization
- [ ] 6. Mutex/semaphore — only when there's a real reason
- [ ] 7. Interrupt experiment (ISR → task)

---

## Phase 6 — Explore the ESP32's Two Cores

Only after FreeRTOS tasks and scheduling are understood. (Not "one task = one core".)

Learn: scheduling, task affinity, dual-core behavior, why more cores can and can't help.

- [ ] Observed which core each task runs on (unpinned)
- [ ] Pinned motor task to one core, input task to the other
- [ ] Compared behavior pinned vs unpinned
- [ ] I can explain when a second core helps and when it doesn't

---

## Phase 7 — Machine 2: Potentiometer + ADC

Separate from Machine 1. **No communication yet.**

Learn: analog vs digital, ADC, resolution, raw values, mapping (`0–4095 → 0–180°`).

- [ ] Potentiometer connected to ESP32 #2
- [ ] Raw ADC values printed
- [ ] I understand ADC resolution and value range
- [ ] Raw value mapped to a useful range
- [ ] Potentiometer controls a local servo/safe output

---

## Phase 8 — ESP32-to-ESP32 Communication

`ESP32 #2 (pot) ──command──> ESP32 #1 (motor)`

Method to choose when here (UART preferred first; ESP-NOW / Wi-Fi later).

- [ ] Communication method chosen: __________
- [ ] Wiring done (TX↔RX, common GND if UART)
- [ ] Simple message sent and received
- [ ] Machine 2 commands Machine 1's motor

---

## Phase 9 — Machine 3: Arduino Mega 2560

Built independently. Mock data that changes realistically (not pure random).

Learn: serial communication, UART, data formatting, periodic transmission.

- [ ] Arduino Mega basic program
- [ ] Mock temperature
- [ ] Mock pressure
- [ ] Mock RPM
- [ ] Production count
- [ ] Data formatted (e.g. `TEMP=74.3;PRESS=82.1;RPM=1240`)
- [ ] Periodic UART transmission (visible in Serial Monitor)

---

## Phase 10 — Arduino Mega → Raspberry Pi

Learn: UART, serial ports, baud rate, framing, simple protocols, parsing.

⚠️ Pi GPIO is 3.3 V, Mega is 5 V — level-shift or use USB serial.

- [ ] Mega connected to Pi (method: __________)
- [ ] Pi prints raw received data
- [ ] Pi parses Machine 3 data into values

---

## Phase 11 — Raspberry Pi Physical Dashboard (LEDs)

The Pi does **not** invent machine states — machines report, the Pi displays.

- [ ] Machine 1 status LED (green RUNNING / red OFF)
- [ ] Machine 2 status LED
- [ ] Machine 3 status LED
- [ ] Emergency LED blinking

---

## Phase 12 — Raspberry Pi I2C LCD

Learn: I2C, addresses, SDA/SCL, Pi I2C peripherals, LCD control. (Existing LCD at `0x27` may be reused.)

- [ ] I2C enabled and LCD detected (`i2cdetect`)
- [ ] Display temperature
- [ ] Display RPM
- [ ] Display machine states
- [ ] Rotate between screens

---

## Phase 13 — Integrate Everything

- [ ] Machine 1 reports state to Pi
- [ ] Machine 2 reports state to Pi
- [ ] Machine 3 data on LCD
- [ ] LEDs reflect all machine states
- [ ] Full system runs end to end

---

## Log

Add dated entries here as you go (oldest first). Detailed notes: [machine-1-esp32/src/notes.md](machine-1-esp32/src/notes.md).

### 2026-09-23 — Phase 1: Servo works
- Built/wired: project repo set up (README, CHANGELOG, context doc in `docs/`). PlatformIO project `machine-1-esp32`. SG90 signal on GPIO13, power from 5 V (USB), ground shared with the ESP32.
- Code: LEDC channel 0 at 50 Hz, 16-bit resolution. `angleToDuty()` maps 0–180° to duty 1638–7864 and clamps out-of-range angles. `moveServo()` steps through 0°, 90°, 180°.
- Learned (the WHY): the servo reads the absolute pulse width, not the duty ratio. LEDC generates PWM in hardware with no CPU work. Integer division truncates, so multiply first. Clamping is safer than rejecting. C++ scope, pass by value, prototypes.
- Issues/fixes: the datasheet's 1–2 ms didn't reach the full range; calibrated 0.5–2.4 ms on the real servo. Upload failed with "wrong boot mode", so hold BOOT during "Connecting...". Flashing C++ erased MicroPython.

### 2026-09-24 — Phase 2: ON button read and debounced
- Built/wired: START button on GPIO27 (`INPUT_PULLUP`, button to GND). Pins 26 (OFF) and 25 (EMERGENCY) chosen and reserved in `config.h`.
- Code: servo logic refactored into a `ServoMotor` class. millis() debounce written in `main.cpp`, then moved into a reusable `Button` class (`begin()`, `update()`, `wasPressed()`, `isPressed()`). Pins, debounce time and serial baud rate live in `config.h`, and `main.cpp` passes them in.
- Learned (the WHY): floating inputs and pull-ups, safe GPIO choice (strapping/flash/input-only pins), level vs edge, contact bounce, C++ types and `printf` formats.
- Issues/fixes: `delay(100)` hid the bounce; `delay(0)` revealed several PRESSED per press. A 500 ms debounce swallowed normal taps, so 50 ms is used. Tested: hands-off = nothing, long hold = one PRESSED, 10 taps = 10.
