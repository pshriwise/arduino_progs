
// Pin definitions
#define PIR_OUT 24

void setup() {
  Serial.begin(9600); // set serial read speed

  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(PIR_OUT, INPUT);
}

void loop() {
  // if pin-in is high, light LED
  if (digitalRead(PIR_OUT) == HIGH) {
    digitalWrite(LED_BUILTIN, HIGH);
  } else {
    digitalWrite(LED_BUILTIN, LOW);
  }
  // wait 100 ms
  delay(100);
}
