// SONAR
#define TRIGGER_CH 9
#define ECHO_CH    8
#define LED_CH     13

#define MIN_RANGE 4.0 // inches
#define MAX_RANGE 8.0 // inches

// required delay to avoid overlapping signals
#define TRIGGER_DELAY 70 // milliseconds

// how many times measurement is made to average
#define NUM_READS 3

// how long between distance reads
#define TIME_BETWEEN_MEASUREMENT 2500 // milliseconds

// MOTOR
#define IN1 5
#define IN2 6
#define ENA 7
#define LED 13

#define MOTOR_DUTY 200


// CONTROLS
#define STOP_BTN 2

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
  pinMode(STOP_BTN, INPUT);
  
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

// "main"
void loop() {

  // motor disable check
  if (digitalRead(STOP_BTN)) {
    MOTOR_ENABLED = !MOTOR_ENABLED;
    if (motor_disabled()) motor_stop();
    press_wait();
  }
    
  // read echo and print to console
  double dist = dist_read();

  //  digitalWrite(LED_CH, HIGH);
  
  if (dist <= MAX_RANGE && dist >= MIN_RANGE) {
    digitalWrite(LED_CH, HIGH);
    motor_forward();
  } else {
    digitalWrite(LED_CH, LOW);
    motor_stop();
  }
  
  // wait, and read again
  //  delay(TIME_BETWEEN_MEASUREMENT);
}

  // wait for button state to return to low
void press_wait() {
  while (true) {
    if (!digitalRead(STOP_BTN)) { return; }
  }
}
