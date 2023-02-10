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
int IR1_Val = 0, IR2_Val = 0;
#define buzzer A0
// int AddrList[3][3]= {{1,2,3},{4,5,6},{7,8,9}};
int il = 0 ,yl = 2 ,chek = 0;
int Mohg [3] = {2,0,3};
// int mohg = 2 ; Address Variable 1 ~ 9 **WIP
int halt = 0 , targetvalue = 1;



// left 9 6
// right 5 3

#define left_a 9
#define left_b 6
#define right_a 5
#define right_b 3
void move(int x, int y, int z, int v);
void ultrasound(void);
void lcdscreen(int address);
void buzzersoundsound(void);
void IR(void);
// void Pathfindr(void); **WIP
void targetboard(void);
char x;
int y, z;

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
// x -> direction, y -> action, z -> delay, v -> speed
// x, 1 -> left motor , 2 -> right motor , 3 -> both motors
// y, 1 -> no run, 2 -> forward, 3 -> reverse, 4 -> brake
// z -> delay
// v -> speed
void loop() {
  if (chek < 3)
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
  //stops
  lcd.println("stop");
    switch (Mohg[chek])
    {
    case 0:
  move(2,3,470,150);
      //left 
      ++chek;
      break;
    case 1:
  move(3,2,420,150); //forward
   ++chek;
      break;
    case 2:
  move(1,2,470,150);
    //right 
      ++chek;
      break;
    case 3:
  move(3,4,150,80); //stop
      ++chek;
      break; 
    default:
  move(3,4,150,80); //stop
      break;
    }
    
}else if(IR1_Val == 1 && IR2_Val == 0){
// turn right
lcd.println("right"); 
 move(1,2,120,120);move(2,2,120,120);
}else if(IR1_Val == 0 && IR2_Val ==1){
// turn left
lcd.println("left");
  move(2,3,120,120);move(1,3,120,120);
}else if (IR1_Val == 0 && IR2_Val == 0){
// forward
lcd.println("forward");
  move(3,2,120,100);
  // x, 1 -> left motor , 2 -> right motor , 3 -> both motors
// y, 1 -> no run, 2 -> forward, 3 -> reverse, 4 -> brake
}else{
// stop
lcd.println("stop");
  move(3,4,150,80);
}
 lcd.clear();
}

void move(int x, int y, int z, int v)
{
  int c, d;
  switch (y)
  {
  case 1:
  {
    c = 0;
    d = 0;
    // no run
  }
    break;
  case 2:
  {
    c = 0;
    d = v;
    // forward
  }
    break;
  case 3:
  {
    c = v;
    d = 0;
    // reverse
  }
    break;
  case 4:
  {
    c = 200;
    d = 200;
    // brake
  }
    break;
  }
    switch (x)
    {
    case 1:
    {
      analogWrite(left_a, c);
      analogWrite(left_b, d);
      delay(z);
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
      delay(z);
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
      delay(z);
      analogWrite(left_a, 200);
      analogWrite(left_b, 200);
      analogWrite(right_a, 200);
      analogWrite(right_b, 200);
      return;
      // both
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
        int dist_2 = pulseIn(echopin_right, HIGH) / 58;
  if (dist_1 < 10||dist_2 < 10)
  {
    lcd.println("Path Blocked!!!");
     move (3,3,50,200);
     for (targetvalue = 1; targetvalue != 0;)
     {
     targetvalue = digitalRead(target);
     digitalWrite(buzzer,HIGH);
     delay(2);
    digitalWrite(buzzer,LOW);
    delay(1);
     }
  lcd.clear();
  return;
  }
  targetvalue = 1;
  
  return;
}

void lcdscreen(int address)
{
  lcd.setCursor(0,0);
  lcd.print("Robot is ready");
  delay(500);
  lcd.setCursor(0,1);
  lcd.println ("Robot is heading to ");
  lcd.println (address);
}

void buzzersoundsound (void)
{
  for(int i=0;i<11;i++)
    {
    digitalWrite(buzzer,HIGH);
    delay(1);
                digitalWrite(buzzer,LOW);
                delay(1);
    }
       delay(500);
}

/* void Pathfindr(void)
{
   for (int il = -1; il < 2; il++)
  {
    for (int yl = -1; yl < 2; yl++)
    {
      if (AddrList[il][yl] != mohg)
      {}
      else
      {
        return;
      }
    }
    yl = 0;
  } ** WIP
} */

void targetboard(void)
{
targetvalue = digitalRead(target);
if (targetvalue == 0)
{
  while (halt < 1)
  {
    lcd.println("halt");
    move(3,4,50,90);
    delay(1000);
    if (target == 0)
    {
      halt = 1;
      lcd.clear();
    }
    else
    {
      halt = 0;
      lcd.clear();
    }
  }
}
halt = 0;
lcd.clear();
return;
}