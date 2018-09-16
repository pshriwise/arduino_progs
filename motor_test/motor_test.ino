#define ENA 9
#define IN1 6
#define IN2 7

#define MOTOR_DUTY 200

void setup() {
  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
}

void motor_forward() {
  // set switches
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  // set power
  analogWrite(ENA, MOTOR_DUTY);
}

void motor_backward() {
  // set switches
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  // set power
  analogWrite(ENA, MOTOR_DUTY);
}

void motor_stop() {
  // set switches
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  // set power
  analogWrite(ENA, 0);
}

void pause() {
  // pause for 2 sec
  delay(2000); 
}

void loop() {
  // put your main code here, to run repeatedly:
  motor_forward();
  pause();
  motor_stop();
  pause();
  motor_backward();
  pause();
}
