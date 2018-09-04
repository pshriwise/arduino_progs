#define TRIGGER_CH 12
#define ECHO_CH    13

// required delay to avoid overlapping signals
#define TRIGGER_DELAY 70 // milliseconds

// how many times measurement is made to average
#define NUM_READS 3

// how long between distance reads
#define TIME_BETWEEN_MEASUREMENT 2500 // milliseconds

#define CALIBRATION_OFFSET 0.0 // inches (TO-DO: map out this value)

double DIST_CONV = 1.0/148.0; // SOS in inches per second halved

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
  return (dist * DIST_CONV) + CALIBRATION_OFFSET;
}

void setup() {
  // put your setup code here, to run once:

  // trigger is output from board
  pinMode(TRIGGER_CH, OUTPUT);
  // echo is a read into the board
  pinMode(ECHO_CH,     INPUT);  

  // setup output
  Serial.begin(9600);
  Serial.print("Starting Distance Echo \n");
}

// "main"
void loop() {
  // put your main code here, to run repeatedly:

  // read echo and print to console
  double dist = dist_read();
  Serial.print("\n"); 
  Serial.print("Measured Distance: \n");
  Serial.print(dist); Serial.print(" inches");
  Serial.print("\n");

  // wait, and read again
  delay(TIME_BETWEEN_MEASUREMENT);
}
