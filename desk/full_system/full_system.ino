// SONAR
#define TRIGGER_CH 9
#define ECHO_CH    8
#define LED_CH     13

#define MIN_RANGE 4.0 // inches
#define MAX_RANGE 8.0 // inches
#define BUFFER    0.25 // inches

// required delay to avoid overlapping signals
#define TRIGGER_DELAY 70 // milliseconds

// how many times measurement is made to average
#define NUM_READS 3

// how long between distance reads
#define TIME_BETWEEN_MEASUREMENT 250 // milliseconds

// MOTOR
#define IN1 5
#define IN2 6
#define ENA 7
#define LED 13

#define MOTOR_DUTY 200


// CONTROLS
#define STP_BTN 2
#define FWD_BTN 3
#define BWD_BTN 4

double DIST_CONV = 1.0/148.0; // SOS in inches per second halved

bool MOTOR_ENABLED = true;

// triggers the sonar signal
void trigger() {
  digitalWrite(TRIGGER_CH, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIGGER_CH, LOW);
}

// reads a distance (in inches) from the sonar sensor
double dist_read() {

  unsigned int echo_sum = 0;
  
  for (int i = 0; i < NUM_READS; i = i+1) {
    // trigger signal
    trigger();  
    echo_sum = echo_sum + pulseIn(ECHO_CH, HIGH);
    delay(TRIGGER_DELAY);
  }
  double dist = (double)echo_sum / (double)NUM_READS;
  return dist * DIST_CONV;
}

void setup() {
  // put your setup code here, to run once:

  // trigger is output from board
  pinMode(TRIGGER_CH, OUTPUT);
  pinMode(ECHO_CH,     INPUT);  

  // initalize motor pins
  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(LED, OUTPUT);

  // setup signal pin
  pinMode(LED_CH, OUTPUT);

  //setup controls
  pinMode(STP_BTN, INPUT_PULLUP);
  pinMode(FWD_BTN, INPUT_PULLUP);
  pinMode(BWD_BTN, INPUT_PULLUP);
  
  // setup output
  Serial.begin(9600);
  Serial.print("Starting Distance Echo \n");
}

bool motor_disabled() {
  return !MOTOR_ENABLED;
}

// Motor Functions
void motor_backward() {
  if (motor_disabled()) return;
  // set switches
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  // set power
  analogWrite(ENA, MOTOR_DUTY);
}

void motor_forward() {
  if (motor_disabled()) return;
  digitalWrite(LED, HIGH);  
  // set switches
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  // set power
  analogWrite(ENA, MOTOR_DUTY);
}

void motor_stop() {
  digitalWrite(LED, LOW);
  // set switches
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  // set power
  analogWrite(ENA, 0);
}

bool motor_stop_check() {
  // motor disable check
  if (digitalRead(STP_BTN) == LOW) {
    MOTOR_ENABLED = !MOTOR_ENABLED;
    if (motor_disabled()) motor_stop();
    press_wait(STP_BTN);
  }
  return motor_disabled();
}

void run_forward() {
  motor_forward();

  double dist;
  while(true) {
    if (motor_stop_check()) return;

    dist = dist_read();

    if ( dist > MAX_RANGE || dist < MIN_RANGE - BUFFER ) {
      motor_stop();
      return;
    }
    
    if (digitalRead(FWD_BTN) == LOW) {
      press_wait(FWD_BTN);
      motor_stop();
      return;
    }
    
  }
}

void run_backward() {
  motor_backward();

  double dist;
  while(true) {
    if (motor_stop_check()) return;

    dist = dist_read();

    if ( dist < MIN_RANGE || dist > MAX_RANGE + BUFFER ) {
      motor_stop();
      return;
    }

    if (digitalRead(BWD_BTN) == LOW) {
      press_wait(BWD_BTN);
      motor_stop();
      return;
    }
  }
}


// "main"
void loop() {


  motor_stop_check();
  
  if (digitalRead(FWD_BTN) == LOW) {
    press_wait(FWD_BTN);
    run_forward();
  }

  if (digitalRead(BWD_BTN) == LOW) {
    press_wait(BWD_BTN);
    run_backward();
  }
  // wait, and read again
  delay(TIME_BETWEEN_MEASUREMENT);
}

  // wait for button state to return to low
void press_wait(int channel) {
  while (true) {
    if (digitalRead(channel) == HIGH) { return; }
  }
}
