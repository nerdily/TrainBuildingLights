#include <Arduino.h>

const int RELAY_PINS[] = {2, 3, 4, 5, 6, 7, 8, 9};  // 8 relay pins
const int NUM_RELAYS = 8;
const int BUTTON_PIN = 10;
const int BUTTON_LED_PIN = 11;

bool allOn = false;  // Track if all relays are on or off
bool lastButtonState = HIGH;
bool sequenceRunning = false;  // Prevent button presses during sequence

void setup() {
  // Initialize all relay pins
  for (int i = 0; i < NUM_RELAYS; i++) {
    pinMode(RELAY_PINS[i], OUTPUT);
    digitalWrite(RELAY_PINS[i], HIGH);  // All relays OFF (active LOW)
  }

  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(BUTTON_LED_PIN, OUTPUT);
  digitalWrite(BUTTON_LED_PIN, LOW);  // Button LED OFF

  randomSeed(analogRead(0));  // Seed random number generator
}

void loop() {
  bool buttonState = digitalRead(BUTTON_PIN);

  // Detect button press (only if sequence not running)
  if (buttonState == LOW && lastButtonState == HIGH && !sequenceRunning) {
    // Wait for button release before starting sequence
    while (digitalRead(BUTTON_PIN) == LOW) {
      delay(10);
    }

    sequenceRunning = true;

    if (!allOn) {
      turnOnSequence();
      allOn = true;
      digitalWrite(BUTTON_LED_PIN, HIGH);  // Button LED ON
    } else {
      turnOffSequence();
      allOn = false;
      digitalWrite(BUTTON_LED_PIN, LOW);  // Button LED OFF
    }

    sequenceRunning = false;
    delay(200);  // Debounce delay
  }

  lastButtonState = buttonState;
}

void turnOnSequence() {
  int order[NUM_RELAYS];

  // Create array of relay indices
  for (int i = 0; i < NUM_RELAYS; i++) {
    order[i] = i;
  }

  // Shuffle array (Fisher-Yates shuffle)
  for (int i = NUM_RELAYS - 1; i > 0; i--) {
    int j = random(i + 1);
    int temp = order[i];
    order[i] = order[j];
    order[j] = temp;
  }

  // Turn on relays in random order with random delays
  for (int i = 0; i < NUM_RELAYS; i++) {
    digitalWrite(RELAY_PINS[order[i]], LOW);  // Relay ON (active LOW)
    if (i < NUM_RELAYS - 1) {  // Don't delay after last relay
      delay(random(0, 3001));  // Random delay 0-3000ms
    }
  }
}

void turnOffSequence() {
  int order[NUM_RELAYS];

  // Create array of relay indices
  for (int i = 0; i < NUM_RELAYS; i++) {
    order[i] = i;
  }

  // Shuffle array (Fisher-Yates shuffle)
  for (int i = NUM_RELAYS - 1; i > 0; i--) {
    int j = random(i + 1);
    int temp = order[i];
    order[i] = order[j];
    order[j] = temp;
  }

  // Turn off relays in random order with random delays
  for (int i = 0; i < NUM_RELAYS; i++) {
    digitalWrite(RELAY_PINS[order[i]], HIGH);  // Relay OFF (active LOW)
    if (i < NUM_RELAYS - 1) {  // Don't delay after last relay
      delay(random(0, 3001));  // Random delay 0-3000ms
    }
  }
}
