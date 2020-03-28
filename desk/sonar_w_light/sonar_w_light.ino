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
  return dist * DIST_CONV;
}

void setup() {
  // put your setup code here, to run once:

  // trigger is output from board
  pinMode(TRIGGER_CH, OUTPUT);
  // echo is a read into the board
  pinMode(ECHO_CH,     INPUT);  
  // setup led ch
  pinMode(LED_CH, OUTPUT);

  // setup output
  Serial.begin(9600);
  Serial.print("Starting Distance Echo \n");
}


// "main"
void loop() {
  // put your main code here, to run repeatedly:

  // read echo and print to console
  double dist = dist_read();

  //  digitalWrite(LED_CH, HIGH);
  
  if ( dist <= MAX_RANGE && dist >= MIN_RANGE ) {
    digitalWrite(LED_CH, HIGH);
  } else {
    digitalWrite(LED_CH, LOW);
  }

   Serial.print("\n"); 
   Serial.print("Measured Distance: \n");
   Serial.print(dist); Serial.print(" inches");
   Serial.print("\n");

  // wait, and read again
  delay(TIME_BETWEEN_MEASUREMENT);
}
