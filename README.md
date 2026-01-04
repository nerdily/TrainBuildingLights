# Train Building Lights Controller

This is a simple Arduino project to control my model train layout's building lights using 8 relays with randomized on/off sequences.

## Features
- Controls 8 independent building lights via relay modules
- Random on/off sequences with 0-3 second delays between each light
- Single button control to toggle all lights
- Button LED indicator shows current state (on/off)

## Components
- Arduino Uno (or compatible board)
- 8-channel relay module (or individual relays)
- Momentary push button
- LED (for button indicator)
- 220Ω resistor (for button LED)
- 5V buck converter (to power Arduino from layout power supply)
- Jumper wires

## Wiring Diagram

### Power Supply
```
Layout Power Supply → 5V Buck Converter → Arduino 5V & GND
```

### Relay Connections
```
Arduino Pin 2  → Relay 1 (IN1)
Arduino Pin 3  → Relay 2 (IN2)
Arduino Pin 4  → Relay 3 (IN3)
Arduino Pin 5  → Relay 4 (IN4)
Arduino Pin 6  → Relay 5 (IN5)
Arduino Pin 7  → Relay 6 (IN6)
Arduino Pin 8  → Relay 7 (IN7)
Arduino Pin 9  → Relay 8 (IN8)

Relay Module VCC → Arduino 5V
Relay Module GND → Arduino GND
```

### Building Lights (connected to relay outputs)
```
Each Relay COM (Common) → Building Light Power Supply +
Each Relay NO (Normally Open) → Building Light +
Building Light - → Power Supply GND
```

### Button and Indicator LED
```
Arduino Pin 10 → Push Button → GND (using internal pullup)

Arduino Pin 11 → 220Ω Resistor → LED Anode (+)
LED Cathode (-) → GND
```

## Connection Notes
- The relays are configured as active LOW (relay turns ON when pin is LOW)
- The button uses the Arduino's internal pullup resistor (INPUT_PULLUP)
- Make sure to connect all grounds together (common ground)
- Verify your relay module's voltage requirements (most 5V relay modules work with Arduino)

## How It Works
1. Press the button to turn on all building lights in a random sequence
2. Press again to turn them all off in a different random sequence
3. The button LED indicates whether lights are on or off
4. Each light turns on/off with a random delay (0-3 seconds) for a realistic effect 
