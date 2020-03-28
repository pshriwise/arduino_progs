/// PWM Settings \\\
// Channels
#define ENA 9
#define IN1 6
#define IN2 7
// Motor Signal
#define MOTOR_DUTY 200

/// Sonar Sensor Settings \\\
// Channels
#define TRIGGER_CH 12
#define ECHO_CH    13
// required delay to avoid overlapping signals
#define TRIGGER_DELAY 70 // milliseconds
// how many times measurement is made to average
#define NUM_READS 3
// SOS in inches per second halved
double DIST_CONV = 1.0/148.0; 

void setup() {
  // setup PWM channels
  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  // setup sonar channels
  pinMode(TRIGGER_CH, OUTPUT);
  pinMode(ECHO_CH, INPUT);
  // console output 
  Serial.begin(9600);
  Serial.print("Starting program...\n");
}



// Motor Functions
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

// Sonar Sensor Functions
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

void pause() {
  // pause for 2 sec
  delay(2000); 
}

void loop() {
  // put your main code here, to run repeatedly:

  double boundary = 6.00;
  
  double dist = dist_read();
  Serial.print("Distance: ");
  Serial.print(dist);
  Serial.print(" inches.\n");  
  if( dist < boundary ) {
    motor_stop();
  }
  else {
    motor_forward();
  }

}

