

// include the library code:
#include <LiquidCrystal.h>

//Motor Defination 
const int Motor_pos=13;
const int Motor_neg=12;
//LED Distnation
const int led=19;
const int Rled=18;

//IF the user release Bottom if will show in screen 
const int First_bm=5;
const int Second_bm=4;
const int Third_bm=3;
const int Fourth_bm=2;
//if the elevator reach a floor it will show in screen
const int First_sw=14;
const int Second_sw=15;
const int Third_sw=16;
const int Fourth_sw=17;
//variable for Bottoms and limitswitches
int Botton=0;
int Limit=0;
int Flour=0;

int DoorFlag=0; //show the door for specific function

//Global Variables
char s=0,pv=0,p=0;
char prev=0;
   
//Combine Switches and LimitSitches into two variables   
#define  inputSensor     ((0x0F)&((digitalRead(17)<<3)|(digitalRead(16)<<2)|(digitalRead(15)<<1)|(digitalRead(14))))
#define  inputSw         ((0x0F)&((digitalRead(2)<<3)|(digitalRead(3)<<2)|(digitalRead(4)<<1)|(digitalRead(5))))
char inputr,inputw;   

//state machine functions and datatypes    
typedef struct {
  char out;
  char next[3];
}Moving;

typedef struct {
  char po;
  char next[16];
}posit;

#define stay  (0x00)
#define down  (0x01)
#define up    (0x02)


Moving Move[3]{
  {stay,{stay,down,up}},
  {down,{stay,down,up}},
  {up  ,{stay,down,up}}
};

#define  t  125
#define  k  125

posit pos[4]{
  {0x01,{0,0,2,2,2,2,2,2,2,2,2,2,2,2,2,2}},  //first
  {0x02,{0,1,0,1,2,k,2,k,2,k,2,k,2,k,2,k}},  //second 
  {0x04,{0,1,1,1,0,1,1,1,2,t,t,t,2,t,t,t}},  //third
  {0x08,{0,1,1,1,1,1,1,1,0,1,1,1,1,1,1,1}}   //fourth
};

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(6, 7, 11, 10, 9, 8);

void setup() {
 // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  // Print a message to the LCD.
  lcd.print("hello, world!");

  pinMode(Motor_pos,OUTPUT);
  pinMode(Motor_neg,OUTPUT);
  pinMode(led,OUTPUT);
  pinMode(Rled,OUTPUT);
  
  inputr=inputSensor;
  
  output(Move[1].out);
  while(inputSensor!=pos[s].po){view();}
  output(Move[s].out);
}

void loop() {
 inputw = inputSw;
 inputr=inputSensor;

     p|=inputw; // store floor 
//sensor sensing the floor 
        if(inputSensor==0x01){pv=0;}
   else if(inputSensor==0x02){pv=1;}
   else if(inputSensor==0x04){pv=2;}
   else if(inputSensor==0x08){pv=3;}

//state machine is in work
   s=pos[pv].next[p];
   if(s==125){s=prev; }
   
  /*Just for Safety*/
   if(inputSensor==0x01&&Move[s].out==0x01){s=0;}
   if(inputSensor==0x08&&Move[s].out==0x02){s=0;}
 
  output(Move[s].out);
   prev=s;

     /*When Reach Distination ......*/
  view(); // view door and distination
  RLED(inputSensor);
  if(((inputSensor&0x01)==0x01)&&((Move[s].out==0x01))&&((p&0x01)==0x01))                                       {output(0x00); DoorFlag=1; LED();}
  if(((inputSensor&0x02)==0x02)&&((Move[s].out==0x01)|(Move[s].out==0x02))&&DoorFlag!=2&&((p&0x02)==0x02))      {output(0x00); DoorFlag=2; LED();}
  if(((inputSensor&0x04)==0x04)&&((Move[s].out==0x01)|(Move[s].out==0x02))&&DoorFlag!=3&&((p&0x04)==0x04))      {output(0x00); DoorFlag=3; LED();}
  if(((inputSensor&0x08)==0x08)&&((Move[s].out==0x02))&&((p&0x08)==0x08))                                       {output(0x00); DoorFlag=4; LED();}
  
  p&=~(inputSensor); //remove floor
}


//combine output into one variable
void output(char out)
{
  digitalWrite(13,out>>1);
  digitalWrite(12,(out&0x01));
}
void RLED(int Data)
{
  if(Data!=0x00)
  {
    digitalWrite(Rled,HIGH);
  }
  else{
    digitalWrite(Rled,LOW);
  }
}


void LED(void)
{
  for(int i=0;i<2;i++)
  {
  digitalWrite(led,HIGH);
  delay(500);
  digitalWrite(led,LOW);
  delay(500);
  }
}



void view(void)
{
   if(digitalRead(First_bm))
 {
  Botton=1;
  lcd.setCursor(1,1);
  lcd.print("First");
  }
   if(digitalRead(Second_bm))
 {
  Botton=2;
   lcd.setCursor(1,1);
  lcd.print("Second");
  }
   if(digitalRead(Third_bm))
 {
  Flour=3;
   lcd.setCursor(1,1);
  lcd.print("Third");
  }
   if(digitalRead(Fourth_bm))
 {
  Botton=4;
   lcd.setCursor(1,1);
  lcd.print("Fourth");
  }

  if(digitalRead(First_sw))
 {
  Limit=1;
  lcd.setCursor(9,1);
  lcd.print("First");
  }
   if(digitalRead(Second_sw))
 {
  Limit=2;
   lcd.setCursor(9,1);
  lcd.print("Second");
  }
   if(digitalRead(Third_sw))
 {
  Limit=3;
   lcd.setCursor(9,1);
  lcd.print("Third");
  }
   if(digitalRead(Fourth_sw))
 {
  Limit=4;
   lcd.setCursor(9,1);
  lcd.print("Fourth");
  }
}
