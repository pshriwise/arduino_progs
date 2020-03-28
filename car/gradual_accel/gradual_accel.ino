/*  ___   ___  ___  _   _  ___   ___   ____ ___  ____
 * / _ \ /___)/ _ \| | | |/ _ \ / _ \ / ___) _ \|    \
 *| |_| |___ | |_| | |_| | |_| | |_| ( (__| |_| | | | |
 * \___/(___/ \___/ \__  |\___/ \___(_)____)___/|_|_|_|
 *                  (____/
 * Arduino Smart Car Tutorial Lesson 1
 * Tutorial URL http://osoyoo.com/2018/12/07/new-arduino-smart-car-lesson1/
 * CopyRight www.osoyoo.com

 * After running the code, smart car will go forward 2 seconds, then go backward 2
 * seconds, then left turn for 2 seconds then right turn for 2 seconds then stop.
 *
 */
#define speedPinR 9    //  RIGHT PWM pin connect MODEL-X ENA
#define RightMotorDirPin1  12    //Right Motor direction pin 1 to MODEL-X IN1
#define RightMotorDirPin2  11    //Right Motor direction pin 2 to MODEL-X IN2
#define speedPinL 6    // Left PWM pin connect MODEL-X ENB
#define LeftMotorDirPin1  7    //Left Motor direction pin 1 to MODEL-X IN3
#define LeftMotorDirPin2  8   //Left Motor direction pin 1 to MODEL-X IN4


/* Utility Functions */

/* set motor speed */
void set_Motorspeed(int speed_L,int speed_R=-1)
{
  if (speed_R < 0) speed_R = speed_L;

  analogWrite(speedPinL,speed_L);
  analogWrite(speedPinR,speed_R);
}


/*motor control*/
void go_Advance(void)  //Forward
{
  digitalWrite(RightMotorDirPin1, HIGH);
  digitalWrite(RightMotorDirPin2,LOW);
  digitalWrite(LeftMotorDirPin1,HIGH);
  digitalWrite(LeftMotorDirPin2,LOW);
  int max_speed = 200;
  int speed = 50;
  int increment = 30;
  int t_wait = 250;
  while (speed < max_speed) {
    set_Motorspeed(speed);
    speed += increment;
    delay(t_wait);
  }
}
void go_Back(int t=0)  //Reverse
{
  digitalWrite(RightMotorDirPin1, LOW);
  digitalWrite(RightMotorDirPin2,HIGH);
  digitalWrite(LeftMotorDirPin1,LOW);
  digitalWrite(LeftMotorDirPin2,HIGH);
  analogWrite(speedPinL,200);
  analogWrite(speedPinR,200);
  delay(t);
}
void stop_Stop()    //Stop
{
  digitalWrite(RightMotorDirPin1, LOW);
  digitalWrite(RightMotorDirPin2,LOW);
  digitalWrite(LeftMotorDirPin1,LOW);
  digitalWrite(LeftMotorDirPin2,LOW);
}

//Pins initialize
void init_GPIO()
{
  pinMode(RightMotorDirPin1, OUTPUT);
  pinMode(RightMotorDirPin2, OUTPUT);
  pinMode(speedPinL, OUTPUT);

  pinMode(LeftMotorDirPin1, OUTPUT);
  pinMode(LeftMotorDirPin2, OUTPUT);
  pinMode(speedPinR, OUTPUT);
  stop_Stop();
}

void setup()
{
  init_GPIO();
  go_Advance();//Forward

  delay(1000);

  stop_Stop();//Stop
}

void loop(){
}
