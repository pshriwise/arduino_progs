#define LED_PIN 13
#define BUTTON_PIN 2

// variables
bool led_state = false;

void setup() {
  // put your setup code here, to run once:
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT);
}

void loop() {

  if (digitalRead(BUTTON_PIN)) {
    press_wait();
    led_state = !led_state;

    // update led status
    if (led_state) {
      // turn LED on
      digitalWrite(LED_PIN, HIGH);
    } else {
      // turn LED off
      digitalWrite(LED_PIN, LOW);
    }

  }

}

// wait for button state to return to low
void press_wait() {
  while (true) {
    if (!digitalRead(BUTTON_PIN)) { return; }
  }
}
