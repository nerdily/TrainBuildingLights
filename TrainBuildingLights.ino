#include <Arduino.h>

const int RELAY_PINS[] = {2, 3, 4, 5, 6, 7, 8, 9};  // 8 relay pins
const int NUM_RELAYS = 8;
const int BUTTON_PIN = 10;
const int BUTTON_LED_PIN = 11;

bool allOn = false;  // Track if all relays are on or off
bool lastButtonState = HIGH;
bool sequenceRunning = false;  // Prevent button presses during sequence

volatile bool ledBlinking = false;  // Controls whether LED should blink (volatile for ISR)

void setupBlinkTimer() {
  // Configure Timer1 for 200ms interrupt
  noInterrupts();
  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1 = 0;

  // 16MHz / 1024 prescaler = 15625 Hz
  // 15625 * 0.2 = 3125 ticks for 200ms
  OCR1A = 3125;

  TCCR1B |= (1 << WGM12);             // CTC mode
  TCCR1B |= (1 << CS12) | (1 << CS10); // 1024 prescaler
  TIMSK1 |= (1 << OCIE1A);            // Enable compare interrupt

  interrupts();
}

// Timer1 interrupt - toggles LED independently of main code
ISR(TIMER1_COMPA_vect) {
  if (ledBlinking) {
    digitalWrite(BUTTON_LED_PIN, !digitalRead(BUTTON_LED_PIN));
  }
}

void startBlinking() {
  ledBlinking = true;
  digitalWrite(BUTTON_LED_PIN, HIGH);
}

void stopBlinking(bool finalState) {
  ledBlinking = false;
  digitalWrite(BUTTON_LED_PIN, finalState ? HIGH : LOW);
}

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
  setupBlinkTimer();
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
      stopBlinking(true);   // LED stays ON
    } else {
      turnOffSequence();
      allOn = false;
      stopBlinking(false);  // LED stays OFF
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

  startBlinking();

  // Turn on relays in random order with random delays
  for (int i = 0; i < NUM_RELAYS; i++) {
    delay(random(500, 3001));  // LED blinks via timer interrupt
    digitalWrite(RELAY_PINS[order[i]], LOW);  // Relay ON (active LOW)
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

  startBlinking();

  // Turn off relays in random order with random delays
  for (int i = 0; i < NUM_RELAYS; i++) {
    delay(random(500, 3001));  // LED blinks via timer interrupt
    digitalWrite(RELAY_PINS[order[i]], HIGH);  // Relay OFF (active LOW)
  }
}
