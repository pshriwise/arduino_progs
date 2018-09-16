#define GREEN_LED 13
#define BLUE_LED  12

#define DELAY 500

void setup() {
  // put your setup code here, to run once:
  pinMode(GREEN_LED,OUTPUT);
  pinMode(BLUE_LED,OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(GREEN_LED,LOW);
  digitalWrite(BLUE_LED,HIGH);
  delay(DELAY);
  digitalWrite(GREEN_LED,HIGH);
  digitalWrite(BLUE_LED,LOW);
  delay(DELAY);
}
