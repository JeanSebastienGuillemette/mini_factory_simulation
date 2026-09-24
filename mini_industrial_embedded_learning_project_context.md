# Mini Industrial Embedded Systems Learning Project

## Purpose

This project is a hands-on embedded-systems learning project built around:

- 2 × IoTCrazy ESP32-WROOM-32U Dev Board with 3dBi Antenna, Micro USB, Dual-Core WiFi + Bluetooth Module, Long Range ESP32 IoT Development Board
- 1 × Elegoo MEGA 2560 R3 Board ATmega2560 ATMEGA16U2 + USB Cable for Arduino | USB Cable Compatible with Arduino IDE Projects RoHS Compliant
- 1 × IRasptek Starter Kit for Raspberry Pi 5 RAM 8GB - Edition of OS-Bookworm Pre-Installed
- Servomotor (TowerPro SG90 servo)
- 3 physical buttons for Machine 1
- Potentiometer for Machine 2
- LEDs for the Raspberry Pi dashboard
- I2C LCD for the Raspberry Pi dashboard
- Optional sensors later

The goal is **not** to build a production-ready industrial system. The goal is to progressively learn embedded programming concepts by building a small, visual "factory" where every new concept has a practical purpose.

The project should be built **iteratively**. Do not jump ahead and build everything at once.

---

# Overall Architecture

```text
                         RASPBERRY PI 5
                    ┌─────────────────────┐
                    │   FACTORY DASHBOARD │
                    │                     │
                    │ M1 🟢 RUNNING       │
                    │ M2 🔴 OFF           │
                    │ M3 🟢 RUNNING       │
                    │                     │
                    │ TEMP: 74.3 °C       │
                    │ RPM: 1240           │
                    └───────┬─────────────┘
                            │
                    Communication
                            │
       ┌────────────────────┼────────────────────┐
       │                    │                    │
       ↓                    ↓                    ↓
 ┌──────────┐         ┌──────────┐         ┌─────────────┐
 │ ESP32 #1 │         │ ESP32 #2 │         │ Arduino Mega│
 │ Machine 1│         │ Machine 2│         │ Machine 3   │
 └────┬─────┘         └────┬─────┘         └──────┬──────┘
      │                     │                      │
    Servo              Potentiometer           "Sensors"
      │                     │                  Temperature
   Buttons                 ADC                  Pressure
                                                RPM etc.
```

## Machine roles

### Machine 1 — ESP32 #1

Main FreeRTOS learning machine.

Hardware:
- Servomotor
- ON button
- OFF button
- EMERGENCY button

Concepts:
- GPIO
- PWM / servo control
- Digital inputs
- Machine states
- FreeRTOS tasks
- Queues
- Task synchronization
- Task priorities
- Interrupts later
- ESP32 dual-core concepts later

Conceptual architecture:

```text
ESP32 #1
│
├── MotorTask ────────> Servo
│
├── ButtonTask
│      ├── ON
│      ├── OFF
│      └── EMERGENCY
│
└── Queue / synchronization
```

### Machine 2 — ESP32 #2

Operator/control machine.

Hardware:
- Potentiometer
- Potentially a servo or other actuator later

Concepts:
- ADC
- Mapping analog values
- Control values
- ESP32-to-ESP32 communication later

Initial concept:

```text
Potentiometer
      │
      ↓
     ADC
      │
      ↓
   ESP32 #2
      │
      ↓
Motor command
```

Eventually Machine 2 can send a command to Machine 1.

### Machine 3 — Arduino Mega 2560

Data/sensor machine.

Hardware:
- Arduino Mega 2560 R3
- Initially no real sensor required
- Generate mock machine data in software

Example data:

```text
Temperature: 74.3 °C
Pressure:    82.1 PSI
RPM:         1240
Production:  1847
```

Concepts:
- Arduino programming
- UART / serial communication
- Sending structured data
- Multiple hardware UARTs
- Eventually real sensors if desired

Conceptual architecture:

```text
Arduino Mega
│
├── Temperature → 73.4 °C
├── Pressure    → 82.1 PSI
├── RPM         → 1240
└── Production  → 1847
        │
        ↓
       UART
        │
        ↓
 Raspberry Pi
```

### Raspberry Pi 5 — Factory Dashboard

The Raspberry Pi is the central dashboard/controller.

Hardware:
- Raspberry Pi 5
- LEDs representing machine states
- I2C LCD
- Potentially buttons or other controls later

Dashboard example:

```text
       FACTORY DASHBOARD

┌─────────────────────────────────┐
│ MACHINE 1                       │
│ 🟢 RUNNING                      │
│                                 │
│ MACHINE 2                       │
│ 🔴 OFF                          │
│                                 │
│ MACHINE 3                       │
│ 🟢 RUNNING                      │
│                                 │
│ Temperature: 74.3 °C            │
│ Pressure:    82.1 PSI           │
│ RPM:         1240               │
└─────────────────────────────────┘
```

Physical LEDs:

```text
Machine 1   🟢 / 🔴
Machine 2   🟢 / 🔴
Machine 3   🟢 / 🔴

Emergency   🔴 blinking
```

LCD example:

```text
┌────────────────────┐
│ M3 TEMP: 74.3 C    │
│ RPM: 1240          │
└────────────────────┘
```

---

# VERY IMPORTANT: Learning Method

The project must be built **one small step at a time**.

Do NOT provide a giant finished implementation.

The preferred workflow is:

1. Build the physical hardware.
2. Test that the hardware works with the smallest possible program.
3. Understand what the code is doing.
4. Add exactly one new concept.
5. Test again.
6. Only then continue.

The user prefers:
- Step-by-step development
- One thing at a time
- Minimal code changes
- Understanding WHY something works
- Direct and honest explanations
- Practical examples
- Visible progress

Avoid dumping large amounts of code unless it is necessary.

The user should type/build the code themselves whenever practical rather than blindly copying a complete project.

Use progress tracking such as:

```text
[ ] Not started
[~] In progress
[x] Complete
```

---

# Development Roadmap

## Phase 1 — Machine 1: Make the Servo Work

Goal:

```text
ESP32 #1 → Servo
```

Start with the simplest possible servo test.

Learn:
- GPIO output
- PWM
- Servo control
- Power considerations
- Basic embedded timing

Do NOT introduce FreeRTOS yet.

Success criteria:

```text
[x] Servo connected correctly
[x] Servo moves
[x] Servo can move to known positions
[x] User understands how the servo signal works
```

---

# Phase 2 — Add the ON Button

Hardware:

```text
ON button
   │
   ↓
ESP32
   │
   ↓
Servo
```

Behavior:

```text
Press ON
   ↓
Machine becomes ON
   ↓
Servo starts/moves
```

Learn:
- Digital input
- Pull-up/pull-down concepts
- Button state
- Basic state handling

Still avoid RTOS at this stage.

---

# Phase 3 — Add the OFF Button

Add:

```text
ON  button ─┐
            │
OFF button ─┼──> ESP32
            │
            ↓
          Servo
```

Machine should now have at least:

```text
OFF
RUNNING
```

Learn:
- Multiple inputs
- State management
- Separating input handling from actuator behavior

---

# Phase 4 — Add the EMERGENCY Button

Add:

```text
ON ──────────┐
OFF ─────────┤
EMERGENCY ───┤
             ↓
         Controller
             ↓
           Servo
```

Machine states should eventually include:

```text
OFF
RUNNING
EMERGENCY
```

Emergency behavior should immediately put the machine into a safe software state.

The exact physical behavior should be simple and safe for the hardware being used.

Learn:
- State machines
- Priority of events
- Emergency/safety-state thinking

---

# Phase 5 — Introduce FreeRTOS

Only after the basic machine works without RTOS.

Start with a very simple task structure:

```text
FreeRTOS
│
├── ButtonTask
│
└── MotorTask
```

Then introduce communication:

```text
ButtonTask
    │
    │ command
    ↓
  Queue
    │
    ↓
MotorTask
    │
    ↓
 Servo
```

Learn in this order:

1. Task creation
2. Task delay
3. Task priorities
4. Queues
5. Task synchronization
6. Mutexes/semaphores when there is an actual reason to use them
7. Interrupts later

Do not introduce every FreeRTOS feature at once.

---

# Phase 6 — Explore the ESP32's Two Cores

Only after the user understands FreeRTOS tasks and scheduling.

Experiment with:

```text
Core 0 → one group of work
Core 1 → another group of work
```

For example:

```text
Core 0
└── Motor-related task

Core 1
└── Input/control-related task
```

Important:

Do NOT teach "one task = one core" as a general rule.

First understand that FreeRTOS schedules tasks. Then explore what changes when tasks are pinned to ESP32 cores.

The purpose is to understand:
- Scheduling
- Task affinity
- Dual-core MCU behavior
- Why multiple cores can and cannot help

---

# Phase 7 — Machine 2: Potentiometer + ADC

Start separately from Machine 1.

Hardware:

```text
Potentiometer
      │
      ↓
ESP32 #2
      │
     ADC
```

Read the potentiometer.

Learn:
- Analog vs digital
- ADC
- ADC resolution
- Raw values
- Mapping a raw ADC value to a useful range

Example:

```text
ADC
0 ─────────────── 4095
│                    │
0°                   180°
```

Then use the potentiometer to control a servo or another safe output.

Do not introduce communication with Machine 1 immediately.

---

# Phase 8 — ESP32-to-ESP32 Communication

Once Machine 2 works independently:

```text
ESP32 #2                         ESP32 #1

Potentiometer
     │
     ↓
    ADC
     │
     └──── command ───────────────> Motor
```

The exact communication method can be selected when reaching this phase.

Possible concepts:
- UART
- Wi-Fi
- ESP-NOW
- Other appropriate method

Prefer UART initially if the goal is to understand serial communication at a low level.

---

# Phase 9 — Machine 3: Arduino Mega 2560

Build Machine 3 independently.

The Arduino initially generates mock sensor data.

Example:

```text
Temperature: 70.0 °C
Temperature: 70.4 °C
Temperature: 71.1 °C
Temperature: 71.8 °C
...
```

Also generate:
- Pressure
- RPM
- Production count

The values should change realistically rather than being completely random if practical.

Learn:
- Serial communication
- UART
- Data formatting
- Periodic data transmission

Example conceptual message:

```text
TEMP=74.3;PRESS=82.1;RPM=1240
```

Do not over-engineer the protocol initially.

---

# Phase 10 — Arduino Mega → Raspberry Pi

Connect Machine 3 to the Raspberry Pi.

Concept:

```text
Arduino Mega
     │
    UART
     │
     ↓
Raspberry Pi
```

The Pi receives the data and parses it.

Learn:
- UART
- Serial ports
- Baud rate
- Framing
- Simple message protocols
- Parsing incoming data

The Raspberry Pi should first simply print received data.

Only after that should it update the LCD.

---

# Phase 11 — Raspberry Pi Physical Dashboard

Add physical status LEDs.

Suggested representation:

```text
Machine 1:
Green = RUNNING
Red   = OFF

Machine 2:
Green = RUNNING
Red   = OFF

Machine 3:
Green = RUNNING
Red   = OFF

Emergency:
Red LED blinking = EMERGENCY
```

The Pi becomes responsible for displaying machine states.

Important design principle:

The Pi should not invent machine states.

The machines should report their states, and the Pi should represent those states.

---

# Phase 12 — Raspberry Pi I2C LCD

Use the LCD as a simple information display.

Example:

```text
┌────────────────────┐
│ M3 TEMP: 74.3 C    │
│ RPM: 1240          │
└────────────────────┘
```

Later it can rotate between:

```text
Machine 1 status
Machine 2 status
Machine 3 status

Temperature
Pressure
RPM
Production count
```

Learn:
- I2C
- I2C addresses
- SDA
- SCL
- Raspberry Pi I2C peripherals
- LCD control

The user's existing LCD experience at I2C address 0x27 can be reused if the hardware is the same.

---

# Phase 13 — Integrate Everything

Final conceptual system:

```text
                         RASPBERRY PI 5
                    ┌─────────────────────┐
                    │   FACTORY DASHBOARD │
                    │                     │
                    │ M1 🟢 RUNNING       │
                    │ M2 🔴 OFF           │
                    │ M3 🟢 RUNNING       │
                    │                     │
                    │ TEMP: 74.3 °C       │
                    │ PRESS: 82.1 PSI     │
                    │ RPM: 1240           │
                    └───────┬─────────────┘
                            │
                    Communication
                            │
       ┌────────────────────┼────────────────────┐
       │                    │                    │
       ↓                    ↓                    ↓
 ┌──────────┐         ┌──────────┐         ┌─────────────┐
 │ ESP32 #1 │         │ ESP32 #2 │         │ Arduino Mega│
 │ Machine 1│         │ Machine 2│         │ Machine 3   │
 └────┬─────┘         └────┬─────┘         └──────┬──────┘
      │                     │                      │
    Servo              Potentiometer           Mock data
      │                     │                      │
   Buttons                 ADC                    UART
```

---

# Concepts We Should Eventually Cover

The project should naturally expose the user to:

## MCU / hardware

- GPIO
- Digital input/output
- Pull-up / pull-down
- PWM
- Servo control
- ADC
- Timers
- Interrupts
- DMA later if appropriate

## Communication

- UART
- I2C
- SPI
- CAN conceptually/later if hardware permits
- ESP32 networking later

## FreeRTOS

- Tasks
- Scheduler
- Task priorities
- `vTaskDelay`
- Queues
- Semaphores
- Mutexes
- Task notifications
- Interrupt-to-task communication
- Core affinity
- Dual-core ESP32 behavior

## Embedded architecture

- State machines
- Event-driven design
- Separation of responsibilities
- Machine states
- Fault/emergency states
- Producer/consumer patterns
- Communication protocols
- Data serialization/parsing

---

# Important Teaching Rule

Whenever introducing a new technology, explain:

1. **What problem does it solve?**
2. **Why are we using it here?**
3. **What happens electrically / conceptually?**
4. **What part is hardware-specific?**
5. **What part is generic embedded programming?**
6. **How does it relate to things already learned?**

For example, before introducing I2C:

> We need to get data from a sensor using only a few wires. I2C lets multiple devices share SDA and SCL, with each device having an address.

Before introducing FreeRTOS:

> Our machine now has multiple independent responsibilities. Instead of putting everything into one large loop, FreeRTOS lets us organize the program into tasks that can be scheduled independently.

Before introducing UART:

> The Arduino needs to send machine data to the Raspberry Pi. UART gives us a simple serial communication channel between the two devices.

---

# Do Not Over-Engineer the Project

This is primarily a learning project.

Avoid adding:
- Databases
- Web applications
- Docker
- MQTT
- Cloud services
- Authentication
- Complex protocols
- Large frameworks

until the underlying embedded concepts are understood.

Those technologies can be added later as optional extensions.

The first goal is:

```text
Hardware works
      ↓
Understand the MCU
      ↓
Understand communication
      ↓
Understand RTOS
      ↓
Connect machines
      ↓
Build physical dashboard
```

---

# Desired Development Style

When helping with this project:

- Work on **one milestone at a time**.
- Do not give the entire project implementation unless explicitly requested.
- Prefer minimal code.
- Explain why each important line exists.
- Explain wiring before code when hardware is involved.
- Encourage testing after each small change.
- When something fails, troubleshoot the current layer before adding complexity.
- Clearly distinguish ESP32-specific APIs from generic FreeRTOS concepts.
- Clearly distinguish Arduino APIs from generic UART concepts.
- When a course uses STM32, explain how the same FreeRTOS concept maps to ESP32 rather than pretending the hardware APIs are identical.
- Use diagrams whenever they make the architecture clearer.
- Keep the project practical and visual.

---

# Progress Tracker

## Machine 1 — ESP32 FreeRTOS Machine

[ ] Servo physically connected  
[ ] Basic servo test  
[ ] Servo position control  
[ ] ON button  
[ ] OFF button  
[ ] Emergency button  
[ ] Basic machine state machine  
[ ] FreeRTOS introduced  
[ ] Motor task  
[ ] Button task  
[ ] Queue  
[ ] Task priorities  
[ ] Synchronization  
[ ] Interrupt experiment  
[ ] ESP32 dual-core experiment  

## Machine 2 — ESP32 Controller

[ ] Potentiometer connected  
[ ] ADC reading  
[ ] Understand ADC values  
[ ] Map ADC to useful range  
[ ] Control local actuator  
[ ] Communicate with Machine 1  

## Machine 3 — Arduino Mega

[ ] Arduino Mega basic program  
[ ] Generate mock temperature  
[ ] Generate mock pressure  
[ ] Generate mock RPM  
[ ] Generate production count  
[ ] Format data  
[ ] UART transmission  

## Raspberry Pi Dashboard

[ ] Receive UART data  
[ ] Parse Machine 3 data  
[ ] Machine 1 status LED  
[ ] Machine 2 status LED  
[ ] Machine 3 status LED  
[ ] Emergency blinking LED  
[ ] I2C LCD  
[ ] Display temperature  
[ ] Display RPM  
[ ] Display machine states  
[ ] Integrate entire system  

---

# Final Learning Goal

By the end, the user should be able to look at an embedded system and understand the role of concepts such as:

```text
GPIO
PWM
ADC
UART
I2C
SPI
CAN
Interrupts
DMA
RTOS
Tasks
Queues
Semaphores
Mutexes
State machines
```

rather than merely recognizing the acronyms.

The project should make these concepts memorable because each one was introduced to solve a concrete problem in the miniature factory.
