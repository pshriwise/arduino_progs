
/*  ___   ___  ___  _   _  ___   ___   ____ ___  ____
 * / _ \ /___)/ _ \| | | |/ _ \ / _ \ / ___) _ \|    \
 *| |_| |___ | |_| | |_| | |_| | |_| ( (__| |_| | | | |
 * \___/(___/ \___/ \__  |\___/ \___(_)____)___/|_|_|_|
 *                  (____/
 * www.osoyoo.com IR remote control smart car
 * program tutorial http://osoyoo.com/2018/12/14/osoyoo-smart-car-lesson-2-control-robot-car-through-infrared-remote/
 *  Copyright John Yu
 */
#include <IRremote.h>
#define IR_PIN    10 //IR receiver Signal pin connect to Arduino pin D2
 IRrecv IR(IR_PIN);  //   IRrecv object  IR get code from IR remoter
 decode_results IRresults;
#define speedPinR 9    //  RIGHT PWM pin connect MODEL-X ENA
#define RightDirectPin1  12    //Right Motor direction pin 1 to MODEL-X IN1
#define RightDirectPin2  11    //Right Motor direction pin 2 to MODEL-X IN2
#define speedPinL 6    // Left PWM pin connect MODEL-X ENB
#define LeftDirectPin1  7    //Left Motor direction pin 1 to MODEL-X IN3
#define LeftDirectPin2  8   //Left Motor direction pin 1 to MODEL-X IN4

 #define IR_ADVANCE       0x00FF18E7       //code from IR controller "▲" button
 #define IR_BACK          0x00FF4AB5       //code from IR controller "▼" button
 #define IR_RIGHT         0x00FF5AA5       //code from IR controller ">" button
 #define IR_LEFT          0x00FF10EF       //code from IR controller "<" button
 #define IR_STOP          0x00FF38C7       //code from IR controller "OK" button
 #define IR_turnsmallleft 0x00FFB04F       //code from IR controller "#" button

#define MAX_SPEED 200
#define MIN_SPEED 50
#define dSpeed 5

int speed_L = 0;
int speed_R = 0;

void set_Motorspeed(int speed_L, int speed_R)
{
  if (speed_L > 0) {
    digitalWrite(LeftDirectPin1, HIGH);
    digitalWrite(LeftDirectPin2, LOW);
  } else {
    digitalWrite(LeftDirectPin1, LOW);
    digitalWrite(LeftDirectPin2, HIGH);
  }

  if (speed_R > 0) {
    digitalWrite(RightDirectPin1, HIGH);
    digitalWrite(RightDirectPin2, LOW);
  } else {
    digitalWrite(RightDirectPin1, LOW);
    digitalWrite(RightDirectPin2, HIGH);
  }

  analogWrite(speedPinL, abs(speed_L));
  analogWrite(speedPinR, abs(speed_R));
}

int sign(int val) {
  if (val < 0) { return -1; }
  else if (val > 0) { return 1; }
  return 0;
}

void adjust_Motorspeed(int delta_L,
                       int delta_R) {

  int sign_L = sign(speed_L);
  int sign_R = sign(speed_R);

  speed_L += delta_L;
  speed_R += delta_R;

  int tmp_L;
  int tmp_R;

  // limit top speeds
  if (abs(speed_L) > MAX_SPEED) {
    speed_L = sign_L * MAX_SPEED;
  }

  if (abs(speed_R) > MAX_SPEED) {
    speed_R = sign_R * MAX_SPEED;
  }

  // if below min speed,
  // set to zero to remove motor whine
  if (abs(speed_L) < MIN_SPEED) {
    tmp_L = 0;
    speed_L += 2 * delta_L;
  } else {
    tmp_L = speed_L;
  }

  if (abs(speed_R) < MIN_SPEED) {
    tmp_R = 0;
    speed_R += 2 * delta_R;
  }
 else {
   tmp_R = speed_R;
 }

  set_Motorspeed(tmp_L, tmp_R);
}
void balance_speed() {
  Serial.println("Balancing speed");
  // use the larger of the two speeds
  int sign_L = sign(speed_L);
  int sign_R = sign(speed_R);

//  if (abs(speed_L) > abs(speed_R)) {
//    speed_R = speed_L;
//  } else {
//    speed_L = speed_R;
//  }

  int speed = (speed_L + speed_R) /2;
  speed_L = speed;
  speed_R = speed;

  set_Motorspeed(speed_L, speed_R);
}


enum DN
{
  GO_ADVANCE, //go forward
  GO_LEFT, //left turn
  GO_RIGHT,//right turn
  GO_BACK,//backward
  STOP_STOP,
  DO_AGAIN,
  DEF
}Drive_Num=DEF;

DN Prev_Drive_Num=DEF;

bool stopFlag = true;//set stop flag
bool JogFlag = false;
uint16_t JogTimeCnt = 0;
uint32_t JogTime=0;
uint8_t motor_update_flag = 0;
/***************motor control***************/
void go_Advance(int t=0)  //Forward
{
  adjust_Motorspeed(dSpeed, dSpeed);
  delay(t);
}
void go_Left(int t=0)  //Turn left
{
  adjust_Motorspeed(-dSpeed, dSpeed);
  delay(t);
}
void go_Right(int t=0)  //Turn right
{
  adjust_Motorspeed(dSpeed, -dSpeed);
  delay(t);
}
void go_Back(int t=0)  //Reverse
{
  adjust_Motorspeed(-dSpeed, -dSpeed);
  delay(t);
}
void stop_Stop()    //Stop
{
  digitalWrite(RightDirectPin1, LOW);
  digitalWrite(RightDirectPin2,LOW);
  digitalWrite(LeftDirectPin1,LOW);
  digitalWrite(LeftDirectPin2,LOW);
  speed_L = 0;
  speed_R = 0;
}

/**************detect IR code***************/
void do_IR_Tick()
{
  if(IR.decode(&IRresults))
  {
    Serial.println("IR Signal decoded");
    Serial.println(IRresults.value);
    Serial.println(IR_ADVANCE);

    if(IRresults.value == 4294967295) {
     Drive_Num = DO_AGAIN;
    }
    else if(IRresults.value==IR_ADVANCE || IRresults.value == 741245183)
    {
      Drive_Num=GO_ADVANCE;
    }
    else if(IRresults.value==IR_RIGHT || IRresults.value == 3692585004)
    {
       Drive_Num=GO_RIGHT;
    }
    else if(IRresults.value==IR_LEFT || IRresults.value == 2507695210)
    {
       Drive_Num=GO_LEFT;
    }
    else if(IRresults.value==IR_BACK || IRresults.value == 1511671869)
    {
        Drive_Num=GO_BACK;
    }
    else if(IRresults.value==IR_STOP || IRresults.value == 3409756287)
    {
      Drive_Num=STOP_STOP;
    }
    IRresults.value = 0;
    IR.resume();
  }
}

/**************car control**************/
void do_Drive_Tick()
{

  if (Drive_Num == DO_AGAIN) { 
    Drive_Num = Prev_Drive_Num; 
  }
  
  switch (Drive_Num)
    {
    case GO_ADVANCE: balance_speed(); go_Advance();JogFlag = true;JogTimeCnt = 1;JogTime=millis();break;//if GO_ADVANCE code is detected, then go advance
    case GO_LEFT: go_Left();JogFlag = true;JogTimeCnt = 1;JogTime=millis();break;//if GO_LEFT code is detected, then turn left
    case GO_RIGHT:  go_Right();JogFlag = true;JogTimeCnt = 1;JogTime=millis();break;//if GO_RIGHT code is detected, then turn right
    case GO_BACK: balance_speed(); go_Back();JogFlag = true;JogTimeCnt = 1;JogTime=millis();break;//if GO_BACK code is detected, then backward
    case STOP_STOP: stop_Stop();JogTime = 0;break;//stop
    default:break;
    }
  if (Drive_Num != DEF) { Prev_Drive_Num = Drive_Num; }
  Drive_Num=DEF;
  //keep current moving mode for  200 millis seconds
  if(millis()-JogTime>=200)
    {
      JogTime=millis();
      if(JogFlag == true)
        {
          stopFlag = false;
          if(JogTimeCnt <= 0)
            {
              JogFlag = false; stopFlag = true;
            }
          JogTimeCnt--;
        }
      if(stopFlag == true)
        {
          JogTimeCnt=0;
          //stop_Stop();
        }
    }
}

void setup()
{
  Serial.begin(9600);
  pinMode(RightDirectPin1, OUTPUT);
  pinMode(RightDirectPin2, OUTPUT);
  pinMode(speedPinL, OUTPUT);
  pinMode(LeftDirectPin1, OUTPUT);
  pinMode(LeftDirectPin2, OUTPUT);
  pinMode(speedPinR, OUTPUT);
  stop_Stop();

  pinMode(IR_PIN, INPUT);
  digitalWrite(IR_PIN, HIGH);
  IR.enableIRIn();
  Serial.print("Setup complete");
}

void loop()
{
  do_IR_Tick();
  do_Drive_Tick();
}
