#define PIR_OUT 24



void setup() {
  Serial.begin(9600); // what does this do?
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(PIR_OUT, INPUT);
  
}

void loop() {
  if (digitalRead(PIR_OUT) == HIGH) {
    digitalWrite(LED_BUILTIN, HIGH);
  } else {
    digitalWrite(LED_BUILTIN, LOW);
  }
  delay(100);
}
