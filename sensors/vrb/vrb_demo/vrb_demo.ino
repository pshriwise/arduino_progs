
// Pin definitions
#define LED_PIN LED_BUILTIN
#define VRB_IN A0

// state variables
int present_condition = 0;
int previous_condition = 0;

void led_blink() {
  for (int i = 0; i < 3; i++) {
    digitalWrite(LED_BUILTIN, HIGH);
    delay(500);
    digitalWrite(LED_BUILTIN, LOW);
    delay(500);
  }
}

void setup() {
  // put your setup code here, to run once:
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(VRB_IN, INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  previous_condition = present_condition;
  present_condition = digitalRead(VRB_IN);

  // if the reading changes, blink the LED
  if (previous_condition != present_condition) {
    led_blink();
  }
}
