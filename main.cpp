//Pin Output
#define ML_Ctrl 4     //define the direction control pin of B motor
#define ML_PWM 5      //define the PWM control pin of B motor
#define MR_Ctrl 2     //define the direction control pin of A motor
#define MR_PWM 9      //define the PWM control pin of A motor
#define USSt 12       //define the UltraSonicSensor trig pin
#define USSe 13       //define the UltraSonicSensor echo pin
#define ServoPin 10


//In program variables
int P;     //Position      servo                                             /*Position*/
//long M;         //Millis                                                         /*Millis*/
int D;         //Distance

//Trimming variables
int MSR = 85;  //MaxSpeedRight   85                                              /*MaxSpeedRight*/
int MSL = 87;  //MaxSpeedLeft    87                                              /*MaxmumSpeedLeft*/
int MSAS = 63;  //MaxSideAdaptionSpeed                                           /*MaxSideAdaptionSpeed*/
int LTSR = 200;  //LeftTurnSpeedRight                                            /*LeftTurnSpeedRight*/
int LTSL = 70;  //LeftTurnSpeedLeft                                              /*leftTurnSpeedLeft*/

int FWD = 300;   //FrontWarningDistance                                            /*rontWarningDistance*/
int SWD = 160;   //SideWarningDistance                                             /*SideWarningDistance*/
int NSWD = 0;  //NoSideWarningDistance                                           /*NoSideWarningDistance*/

double turnTime = 6.5;                                               /*Time it takes to turn the robot one degree*/


void setup()                                                                    //done
{
  Serial.begin(9600);
  pinMode(ML_Ctrl, OUTPUT); //set direction control pin of B motor to OUTPUT
  pinMode(ML_PWM, OUTPUT);  //set the PWM control pin of B motor to OUTPUT 
  pinMode(MR_Ctrl, OUTPUT); //set the direction control pin of A motor to OUTPUT
  pinMode(MR_PWM, OUTPUT);  //set PWM control pin of A motor to OUTPUT
  pinMode(USSt, OUTPUT);    //set trigPin to OUTPUT
  pinMode(USSe, INPUT);     //set echoPin to INPUT
  pinMode(ServoPin, OUTPUT);//set Some servopin to OUTPUT
}

void SetAngle(int degrees)                                                         //done
{
  int deg = 180-degrees%180;
  if (degrees == 180)
    deg=0;
  int SAD = (deg-5.2-90)*11.8/(deg*0.0007+1)+1420;
  digitalWrite(ServoPin, HIGH);
  delayMicroseconds(SAD);
  digitalWrite(ServoPin, LOW);
  delay(10);
}

long readUSS()                                                                    //done
{
    int duration;
    int mm;
    digitalWrite(USSt, LOW);
    delayMicroseconds(2);
    digitalWrite(USSt, HIGH);
    delayMicroseconds(10);
    digitalWrite(USSt, LOW);
    duration = pulseIn(USSe, HIGH);
    mm = duration*0.5*0.344632;
    return mm;
}





float greenLights(int dir)
{
  SetAngle(dir);
  if (readUSS() >= 400)
  {
    return true;
  }else{
    return false;
  }
}

enum Direction
{
    Right,
    Left
};

void Turn(int Degrees, float relativeDir)
{
  if(relativeDir == Left)
  {
  digitalWrite(MR_Ctrl, HIGH);
  digitalWrite(ML_Ctrl, LOW);
  analogWrite(MR_PWM, MSR-3);
  analogWrite(ML_PWM, MSL-3);
  }
  else if (relativeDir == Right)
  {
    digitalWrite(MR_Ctrl, LOW);
    digitalWrite(ML_Ctrl, HIGH);
    analogWrite(MR_PWM, MSR);
    analogWrite(ML_PWM, MSL);
  }

  delay(turnTime*Degrees); //behöver prövas ut hur lång tid det tar att vrida en grad
  
  analogWrite(MR_PWM, 0);
  analogWrite(ML_PWM, 0);
}

void FW()        //FrontWarning                                                    //To do
{

  SetAngle(90);
  digitalWrite(MR_Ctrl, HIGH);
  analogWrite(MR_PWM, 50);
  digitalWrite(ML_Ctrl, HIGH);
  analogWrite(ML_PWM, 52);

  while(readUSS() > 110)
  {
    delay(3);
  }

  analogWrite(MR_PWM, 0);
  analogWrite(ML_PWM, 0);

  if (greenLights(171) == true)
    {
      Turn(90, Left);
    }

  else if (greenLights(4) == true)
    {
      Turn(90, Right);
    }
  else{
    Turn(180, Right);
    
  }

}




void SW()                                                                         //If it works then done
{
  digitalWrite(MR_Ctrl, HIGH);
  digitalWrite(ML_Ctrl, HIGH);
  analogWrite(MR_PWM, MSR -MSAS*min(pow((int(readUSS())/SWD), 2), 200));
  analogWrite(ML_PWM, MSL);
  while(readUSS()<SW)
  {
    analogWrite(MR_PWM, MSR -MSAS*pow((int(readUSS())/SWD), 2));
  }
  analogWrite(MR_PWM, 0);
  analogWrite(ML_PWM, 0);
}

void NSW()                                                                        //If it works then done
{
  digitalWrite(MR_Ctrl, HIGH);
  analogWrite(MR_PWM, LTSR);
  digitalWrite(ML_Ctrl, HIGH);
  analogWrite(ML_PWM, LTSL);
  delay(LTT);
}

void Fd()                                                                         //Done
{
  digitalWrite(MR_Ctrl, HIGH);
  analogWrite(MR_PWM, MSR);
  digitalWrite(ML_Ctrl, HIGH);
  analogWrite(ML_PWM, MSL);
}




void loop()
{
//Locks to the front
  for(P=75;P<=105;P+=5)
  {
    SetAngle(P);
    D+=readUSS();
  }

//Makes an average value of 7 distances mesured to the front
  D=D/7;
  Serial.println(D);

//If there is a wall near to the front, it  looks left and right and desides which way to go, else it goes forward
  if(D<FWD){FW();}
  else{Fd();}
  
//Sets angle to
  SetAngle(0);

//Locks to the 
  for(P=0;P<=15;P+=3)
  {
    SetAngle(P);
    D+=readUSS();
  }
  
//Makes an average value of 5 distances mesured to the left
  D=D/6;
  Serial.println(D);

//If the robot is to near the wal to the left, it adds some speed to the left motor, so that it turns to the right, depending on how near the wal it is, it turns more or less.
  if(D<SWD){SW();}

//
  else if(D>NSWD){NSW();}

//else it simple goeas forward
  else{Fd();}
}