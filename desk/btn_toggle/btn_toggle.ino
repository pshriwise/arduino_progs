#define LEDA_PIN 12
#define LEDB_PIN 13

#define BUTTONA_PIN 2
#define BUTTONB_PIN 3

// variables
bool led_state = false;

void setup() {
  // put your setup code here, to run once:
  pinMode(LEDA_PIN, OUTPUT);
  pinMode(BUTTONA_PIN, INPUT_PULLUP);
  pinMode(LEDB_PIN, OUTPUT);
  pinMode(BUTTONB_PIN, INPUT_PULLUP);

}

void loop() {

  if (digitalRead(BUTTONA_PIN) == LOW) {
    press_wait_a();
    led_state = !led_state;

    // update led status
    if (led_state) {
      // turn LED on
      digitalWrite(LEDA_PIN, HIGH);
    } else {
      // turn LED off
      digitalWrite(LEDA_PIN, LOW);
    }

  }

  if (digitalRead(BUTTONB_PIN) == LOW) {
    press_wait_b();
    led_state = !led_state;

    // update led status
    if (led_state) {
      // turn LED on
      digitalWrite(LEDB_PIN, HIGH);
    } else {
      // turn LED off
      digitalWrite(LEDB_PIN, LOW);
    }

  }

  
}

// wait for button state to return to low
void press_wait_a() {
  while (true) {
    if (digitalRead(BUTTONA_PIN) == HIGH) { return; }
  }
}

void press_wait_b() {
  while (true) {
    if (digitalRead(BUTTONB_PIN) == HIGH) { return; }
  }
}
