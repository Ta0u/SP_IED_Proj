#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <string.h>
// addr,en,rw,rs,d4,d5,d6,d7,bl,blpol
// addr can be 0x3F or 0x27
LiquidCrystal_I2C lcd(0x27, 16, 2);
#define tripin_left 8
#define echopin_left 7
#define tripin_right 11
#define echopin_right 10
#define IR1Pin 4 
#define IR2Pin 2
#define target A1
#define buzzer A0
int IR1_Val = 0, IR2_Val = 0, junc_count = 0;
int address_arr [3] = {2,0,3}; // 0 = left, 1 = forward, 2 = right, 3 = stop
// int AddrList[3][3]= {{1,2,3},{4,5,6},{7,8,9}};
int halt = 0 , targetvalue = 1;

// left 9 6
// right 5 3

#define left_a 9
#define left_b 6
#define right_a 5
#define right_b 3
void move(int motor, int action, int duration, int velocity);
/* motor -> 1: left, 2: right, 3: both
action -> 1: no run, 2: forward, 3: backwards, 4: brake
duration -> (1 ~ 1000 ms) duration the motor runs for
velocity -> (1 ~ 255) speed of motor */
void ultrasound(void);
//void lcdscreen(int address);
void buzzersoundsound(void);
void targetboardboard(void);
void IR(void);
//void targetboard(void);

void setup() {
  pinMode(9, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4,INPUT_PULLUP); //IR1 right
  pinMode(2,INPUT_PULLUP);//IR2 left
  pinMode(target,INPUT_PULLUP);
  Serial.begin(9600);
  pinMode(tripin_left, OUTPUT);
  pinMode(echopin_left, INPUT);
  pinMode(tripin_right, OUTPUT);
  pinMode(echopin_right, INPUT);
  pinMode(buzzer,OUTPUT);
  lcd.init();
  lcd.setCursor(0,0);
  lcd.begin(16, 2); // 16 characters, 2 lines
  lcd.backlight();  // turn backlight on
}

// the loop function runs over and over again until power down or reset
void loop() {
  if (junc_count < 3)
  {
    IR();
    IR();
    ultrasound();
  }else{
  lcd.clear();
  lcd.println("Reached");
  lcd.setCursor(0,1);
  lcd.println("Destination");
  }
}

void IR(void) {
IR1_Val = digitalRead(IR1Pin); // Reading and storing IR sensor 1 signal value
IR2_Val = digitalRead(IR2Pin); // Reading and storing IR sensor 2 signal value
  // 1=black 0=white pin2,IR2=left pin4,IR1=right
if (IR1_Val == 1 && IR2_Val == 1){
  //both black=junction, decide direction to proceed
  lcd.println("Junction");
    switch (address_arr[junc_count]) // direction to take at junction
    {
    case 0:
      move(2,3,470,150);//left
      ++junc_count;
      break;
    case 1:
      move(3,2,420,150); //forward
      ++junc_count;
      break;
    case 2:
      move(1,2,470,150); //right 
      ++junc_count;
      break;
    case 3:
      move(3,4,150,80); //stop
      ++junc_count;
      break; 
    default:
      move(3,4,150,80); //stop
      break;
    }    
}else if(IR1_Val == 1 && IR2_Val == 0){
// turn right
    lcd.println("Right"); 
    move(1,2,120,120);move(2,2,120,120);
}else if(IR1_Val == 0 && IR2_Val ==1){
// turn left
    lcd.println("Left");
    move(2,3,120,120);move(1,3,120,120);
}else if (IR1_Val == 0 && IR2_Val == 0){
// forward
    lcd.println("Forward");
    move(3,2,120,100);
}else{
// stop
    lcd.println("Stop");
    move(3,4,150,80);
}
    lcd.clear();
}
/* motor -> 1: left, 2: right, 3: both
action -> 1: no run, 2: forward, 3: backwards, 4: brake
duration -> (1 ~ 1000 ms) duration the motor runs for
velocity -> (1 ~ 255) speed of motor */
void move(int motor, int action, int duration, int velocity)
{
  int c, d;
  switch (action)
  {
  case 1:
  {
    c = 0;d = 0;// no run
  }
    break;
  case 2:
  {
    c = 0;d = velocity;// forward
  }
    break;
  case 3:
  {
    c = velocity;d = 0;// backwards
  }
    break;
  case 4:
  {
    c = 200;d = 200;// brake
  }
    break;
  }
    switch (motor)
    {
    case 1:
    {
      analogWrite(left_a, c);
      analogWrite(left_b, d);
      delay(duration);
      analogWrite(left_a, 200);
      analogWrite(left_b, 200);
      return;
      // left
    }
    break;
    case 2:
    {
      analogWrite(right_a, c);
      analogWrite(right_b, d);
      delay(duration);
      analogWrite(right_a, 200);
      analogWrite(right_b, 200);
      return;
      // right
    }
    break;
    case 3:
    {
      analogWrite(left_a, c);
      analogWrite(left_b, d);
      analogWrite(right_a, d);
      analogWrite(right_b, c);
      delay(duration);
      analogWrite(left_a, 200);
      analogWrite(left_b, 200);
      analogWrite(right_a, 200);
      analogWrite(right_b, 200);
      return;
      // forward
    }
    break;
    } 
}


void ultrasound (void)
{
  digitalWrite(tripin_left, HIGH);
  delayMicroseconds(3);
  digitalWrite(tripin_left, LOW);
  delayMicroseconds(3);
  int dist_1 = pulseIn(echopin_left, HIGH) / 58;
  digitalWrite(tripin_right, HIGH);
  delayMicroseconds(3);
  digitalWrite(tripin_right, LOW);
  delayMicroseconds(3);
  int dist_2 = pulseIn(echopin_right, HIGH) / 58; // Sending Ultrasonic signals
  if (dist_1 < 10||dist_2 < 10)
  {
    lcd.println("Path Blocked!!!");
     move (3,3,50,200);
     targetboardboard();
  }
  lcd.clear();
  return;
  targetvalue = 1;
  return;
}


void buzzersoundsound (void)
{
    digitalWrite(buzzer,HIGH);
    delay(2);
    digitalWrite(buzzer,LOW);
    delay(1);
}

void targetboardboard (void){
     for (targetvalue = 1; targetvalue != 0;)
     {
     targetvalue = digitalRead(target);
     buzzersoundsound();
     }
}

