//Pin Output
#define ML_Ctrl 4     //define the direction control pin of B motor
#define ML_PWM 5      //define the PWM control pin of B motor
#define MR_Ctrl 2     //define the direction control pin of A motor
#define MR_PWM 9      //define the PWM control pin of A motor
#define USSt 12       //define the UltraSonicSensor trig pin
#define USSe 13       //define the UltraSonicSensor echo pin
#define ServoPin 10


//In program variables
int P=171;     //Position                                                        /*Position*/
int SAD;       //SetAngleDelay                                                   /*SetAngleDelay*/
long M;         //Millis                                                          /*Millis*/
bool D = false;//Direction       //true = to the left, false is to the midle     /*Direction*/

//Trimming variables
int MSR = 85;  //MaxSpeedRight   85                                              /*MaxSpeedRight*/
int MSL = 87;  //MaxSpeedLeft    87                                              /*MaxmumSpeedLeft*/
int MSAS = 63;  //MaxSideAdaptionSpeed                                           /*MaxSideAdaptionSpeed*/
int LTSR = 200;  //LeftTurnSpeedRight                                            /*LeftTurnSpeedRight*/
int LTSL = 70;  //LeftTurnSpeedLeft                                              /*leftTurnSpeedLeft*/

int FWD = 300;   //FrontWarningDistance                                            /*rontWarningDistance*/
int SWD = 160;   //SideWarningDistance                                             /*SideWarningDistance*/
int NSWD = 0;  //NoSideWarningDistance                                           /*NoSideWarningDistance*/
int NSD = 0;   //NoSideDistance                                                  /*NoSideDistance*/

int NSBPT = 0; //NoSideBeginingPresisionTime                                     /*NoSideBeginingPresisionTime*/
int LTT = 0.25;   //LeftTurnTime                                                 /*LeftTurnTime*/


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
  SetAngle(171);
}

int SetAngle(int myangel)                                                         //done
{
  int SAD = myangel*11+500;
  digitalWrite(ServoPin, HIGH);
  delayMicroseconds(SAD);
  digitalWrite(ServoPin, LOW);
  return(10);
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
  }
  else if (relativeDir == Right)
  {
    digitalWrite(MR_Ctrl, LOW);
    digitalWrite(ML_Ctrl, HIGH);
  }
  analogWrite(MR_PWM, MSR);
  analogWrite(ML_PWM, MSL);

  delay(10*Degrees); //behöver prövas ut hur lång tid det tar att vrida en grad
  
  analogWrite(MR_PWM, 0);
  analogWrite(ML_PWM, 0);
}

void FW()        //FrontWarning                                                    //To do
{
  MSR=MSR;

  SetAngle(85);
  digitalWrite(MR_Ctrl, HIGH);
  analogWrite(MR_PWM, 50);
  digitalWrite(ML_Ctrl, HIGH);
  analogWrite(ML_PWM, 52);

  while(readUSS() > 110)
  {
    delay(3);
  }

  digitalWrite(MR_Ctrl, HIGH);
  analogWrite(MR_PWM, 0);
  digitalWrite(ML_Ctrl, HIGH);
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
  analogWrite(MR_PWM, MSR +MSAS*pow((int(readUSS())/SWD), 2));
  Serial.print(MSR +MSAS*pow((int(readUSS())/SWD), 2))
  digitalWrite(ML_Ctrl, HIGH);
  analogWrite(ML_PWM, MSL);
  while(readUSS>SW){delay(3);}
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
  delay(3);
}





void loop()                                                                      //If it works then done
{
 for(P=75;P<=171;P+=2)
  {
  if(P>105 and P<110){P+=45;}
   SAD = SetAngle(P);
   M = millis();
   while(millis()<M+(SAD))
   {
       if(75<P and P<105)
       {
               if(readUSS<FWD){FW();}
               else{Fd();}
       }
       else if(151<P and P<171)
       {
               if(readUSS<SWD){SW();}
               else if(readUSS>NSWD){NSW();}
               else{Fd();}
       }
       else{Fd();}
       Serial.println(readUSS());
    }
  }
  delay(SAD+20);
  SAD=SetAngle(60);
  delay(SAD+200);
}
