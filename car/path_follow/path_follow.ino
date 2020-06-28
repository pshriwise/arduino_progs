/*  ___   ___  ___  _   _  ___   ___   ____ ___  ____  
 * / _ \ /___)/ _ \| | | |/ _ \ / _ \ / ___) _ \|    \ 
 *| |_| |___ | |_| | |_| | |_| | |_| ( (__| |_| | | | |
 * \___/(___/ \___/ \__  |\___/ \___(_)____)___/|_|_|_|
 *                  (____/ 
 * Arduino Smart Car Tutorial Lesson 4
 * Tutorial URL http://osoyoo.com/2018/12/19/osoyoo-robot-car-kit-lesson-4-tracking-line-robot-car/
 * CopyRight www.osoyoo.com

 * This lesson will let robot car automatically track a black line in ground
 *  
 * 
 */


/*Declare L298N Dual H-Bridge Motor Controller directly since there is not a library to load.*/
//Define L298N Dual H-Bridge Motor Controller Pins
#define speedPinR 9   //  RIGHT PWM pin connect MODEL-X ENA
#define RightDirectPin1  12  //  Right Motor direction pin 1 to MODEL-X IN1 
#define RightDirectPin2  11   // Right Motor direction pin 2 to MODEL-X IN2
#define speedPinL 6     //  Left PWM pin connect MODEL-X ENB
#define LeftDirectPin1  7   // Left Motor direction pin 1 to MODEL-X IN3
#define LeftDirectPin2  8   //Left Motor direction pin 1 to MODEL-X IN4

/*From left to right, connect to D3,A1-A3 ,D10*/
#define LFSensor_0 A0
#define LFSensor_1 A1
#define LFSensor_2 A2
#define LFSensor_3 A3
#define LFSensor_4 A4

#define TURN_SPEED 220
#define FWD_SPEED 220
#define CONTROL_DELAY 50 // how long to allow each command to run

char sensor[5];
 /*read sensor value string, 1 stands for black, 0 starnds for white, i.e 10000 means the first sensor(from left) detect black line, other 4 sensors detected white ground */
String read_sensor_values()
{   
  int sensorvalue=32;
  sensor[0] = !digitalRead(LFSensor_0);
 
  sensor[1] = !digitalRead(LFSensor_1);
 
  sensor[2] = !digitalRead(LFSensor_2);
 
  sensor[3] = !digitalRead(LFSensor_3);
 
  sensor[4] = !digitalRead(LFSensor_4);
  
  sensorvalue +=sensor[0]*16+sensor[1]*8+sensor[2]*4+sensor[3]*2+sensor[4];
 
  String senstr = String(sensorvalue,BIN);
  Serial.println(sensorvalue);
  return senstr.substring(1,6);
}

void go_Advance(void)  //Forward
{
  digitalWrite(RightDirectPin1, HIGH);
  digitalWrite(RightDirectPin2,LOW);
  digitalWrite(LeftDirectPin1,HIGH);
  digitalWrite(LeftDirectPin2,LOW);
}

void go_Left(void)  //Turn left
{
  digitalWrite(RightDirectPin1, HIGH);
  digitalWrite(RightDirectPin2,LOW);
  digitalWrite(LeftDirectPin1,LOW);
  digitalWrite(LeftDirectPin2,HIGH);
}

void go_Right(void)  //Turn right
{
  digitalWrite(RightDirectPin1, LOW);
  digitalWrite(RightDirectPin2,HIGH);
  digitalWrite(LeftDirectPin1,HIGH);
  digitalWrite(LeftDirectPin2,LOW);
}

void go_Back(void)  //Reverse
{
  digitalWrite(RightDirectPin1, LOW);
  digitalWrite(RightDirectPin2,HIGH);
  digitalWrite(LeftDirectPin1,LOW);
  digitalWrite(LeftDirectPin2,HIGH);
}

void stop_Stop()    //Stop
{
  digitalWrite(RightDirectPin1, LOW);
  digitalWrite(RightDirectPin2,LOW);
  digitalWrite(LeftDirectPin1,LOW);
  digitalWrite(LeftDirectPin2,LOW);
}

/*set motor speed */
void set_Motorspeed(int speed_L,int speed_R)
{
  analogWrite(speedPinL, speed_L); 
  analogWrite(speedPinR, speed_R);   
}

void setup()
{
 pinMode(speedPinL, OUTPUT); //left motor PWM pin
 pinMode(speedPinR, OUTPUT); //rignt motor PWM  pin
 pinMode(RightDirectPin1, OUTPUT); //left motor direction pin1
 pinMode(RightDirectPin2, OUTPUT); //left motor direction pin2
 pinMode(LeftDirectPin1, OUTPUT); //right motor direction Pin 1
 pinMode(LeftDirectPin2, OUTPUT);  //right motor direction Pin 2

  /*line follow sensors */
 pinMode(LFSensor_0, INPUT);
 pinMode(LFSensor_1, INPUT);
 pinMode(LFSensor_2, INPUT);
 pinMode(LFSensor_3, INPUT);
 pinMode(LFSensor_4, INPUT); 
 Serial.begin(9600);
}

void auto_tracking(){
 String sensorval= read_sensor_values();
 Serial.println(sensorval);
 if (sensorval=="10000") {
    go_Left();
    set_Motorspeed(-0.5*TURN_SPEED, TURN_SPEED);
    delay(CONTROL_DELAY);
    stop_Stop();
 }

 if (sensorval=="11000" or sensorval=="01000") {
    go_Left();
    set_Motorspeed(-0.25*TURN_SPEED, TURN_SPEED);
    delay(CONTROL_DELAY);
    stop_Stop();
 }
 
 if (sensorval=="10100" or sensorval=="00100" or sensorval=="01100" or sensorval=="11100" or sensorval=="11000" or sensorval=="11110") { 
    //The black line is in the left of the car, need  left turn 
    go_Left();  //Turn left
    set_Motorspeed(0, TURN_SPEED);
    delay(CONTROL_DELAY);
    stop_Stop();
  }

 if (sensorval=="00001") {
    go_Right();
    set_Motorspeed(TURN_SPEED, -0.5*TURN_SPEED);
    delay(CONTROL_DELAY);
    stop_Stop();
 }

 if (sensorval=="00011" or sensorval=="00010") {
    go_Right();
    set_Motorspeed(TURN_SPEED, -0.25*TURN_SPEED);
    delay(CONTROL_DELAY);
    stop_Stop();
 }
 
 if (sensorval=="00101" or sensorval=="00110" or sensorval=="00111" or sensorval=="00011" or sensorval=="01111") { 
    //The black line is on the right of the car, need right turn 
    go_Right();  //Turn right
    set_Motorspeed(TURN_SPEED, 0);
    delay(CONTROL_DELAY);
    stop_Stop();
  }

// for a track with a finish line
// if (sensorval=="11111" or sensorval=="01111" or sensorval=="11110"){
//     stop_Stop();   //The car front touch stop line, need stop
//     set_Motorspeed(0, 0);
//    }

  if (sensorval=="00000" ) {
     go_Back();   //The car front touch stop line, need stop
     set_Motorspeed(FWD_SPEED, FWD_SPEED);
     delay(CONTROL_DELAY);
     stop_Stop();
  }

  if (sensorval=="01110" or sensorval=="11111") {
     go_Advance();
     set_Motorspeed(FWD_SPEED, FWD_SPEED);
     delay(CONTROL_DELAY);
     stop_Stop();
   }
}

void loop() {
  auto_tracking();
}
