#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <string.h>
// addr,en,rw,rs,d4,d5,d6,d7,bl,blpol
// addr can be 0x3F or 0x27
LiquidCrystal_I2C lcd(0x27, 16, 2);
#define tripin 11
#define echopin 10
#define IR1Pin 4 
#define IR2Pin 2
#define target A1
int IR1_Val = 0, IR2_Val = 0;
#define buzzer A1
int AddrList[3][3]= {{1,2,3},{4,5,6},{7,8,9}};
int il = 2 ,yl = 0 ,chek = 1;
int mohg = 2 ; //Address Variable 1 ~ 9
int halt = 0 , targetvalue;



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
void Pathfindr(void);
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
        pinMode(tripin, OUTPUT);
        pinMode(echopin, INPUT);
  pinMode(buzzer,OUTPUT);
        lcd.init();
        lcd.setCursor(0,0);
        lcd.begin(16, 2); // 16 characters, 2 lines
        lcd.backlight();  // turn backlight on
}

// the loop function runs over and over again until power down or reset
// x -> direction, y -> action, z -> duration, v -> speed
// x, 1 -> left motor , 2 -> right motor , 3 -> both motors
// y, 1 -> no run, 2 -> forward, 3 -> reverse, 4 -> brake
// z -> duration
// v -> speed
void loop() {
  if (chek < 4)
  {
    IR();
    ultrasound();
  }
}

void IR(void) {
IR1_Val = digitalRead(IR1Pin); // Reading and storing IR sensor 1 signal value
IR2_Val = digitalRead(IR2Pin); // Reading and storing IR sensor 2 signal value
  // 1=black 0=white pin2,IR2=left pin4,IR1=right
if (IR1_Val == 1 && IR2_Val == 1){
  //stops
  lcd.println("stop");
  switch (chek)
  {
  case 1:
    switch (il)
    {
    case 0:
  move(2,3,600,120);
      //left 
      ++chek;
      break;
    case 1:
  move(3,2,400,120); //forward
   ++chek;
      break;
    case 2:
  move(1,2,600,120);
    //right 
      ++chek;
      break;
    default:
      break;
    }
    break;
 case 2:
 switch (yl)
    {
    case 0:
     move(2,3,600,120);
      //left
       ++chek;
      break;
    case 1:
    move(3,2,400,120);
    //forward 
    ++chek;
      break;
    case 2:
    move(1,2,600,120);
    //right 
    ++chek;
      break;
  default:
    break;
  }
}
}else if(IR1_Val == 1 && IR2_Val == 0){
// turn right
lcd.println("right"); 
 move(1,2,50,155);move(2,2,50,165);
}else if(IR1_Val == 0 && IR2_Val ==1){
// turn left
lcd.println("left");
  move(2,3,100,155);move(1,3,50,165);
}else if (IR1_Val == 0 && IR2_Val == 0){
// forward
lcd.println("forward");
  move(3,2,50,130); 
  // for x, 1 -> left motor , 2 -> right motor , 3 -> both motors
// for y, 1 -> no run, 2 -> forward, 3 -> reverse, 4 -> brake
}else{
// stop
lcd.println("stop");
  move(3,4,50,90);
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
  digitalWrite(tripin, HIGH);
        delayMicroseconds(100);
        digitalWrite(tripin, LOW);
        delayMicroseconds(100);
        int dist = pulseIn(echopin, HIGH) / 58;
  while (dist<10)
  {
    lcd.println("Path Blocked!!!");
  move (3,4,2000,200);
  int targetvalue = digitalRead(target);
  while (targetvalue == 1)
  {
     lcd.clear();
     lcd.println("Halt!!!");
  }
  lcd.clear();
  return;
  }
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

void Pathfindr(void)
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
  }
}

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