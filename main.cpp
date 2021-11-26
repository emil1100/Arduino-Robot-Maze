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
int M;         //Millis                                                          /*Millis*/
bool D = false;//Direction       //true = to the left, false is to the midle     /*Direction*/

//Trimming variables
int MSR = 85;  //MaxSpeedRight                                                   /*MaxSpeedRight*/
int MSL = 87;  //MaxSpeedLeft                                                    /*MaxmumSpeedLeft*/
int MSAS = 0;  //MaxSideAdaptionSpeed                                            /*MaxSideAdaptionSpeed*/
int RTSR = 0;  //RightTurnSpeedRight                                             /*RightTurnSpeedRight*/
int RTSL = 0;  //RightTurnSpeedLeft                                              /*RightTurnSpeedLeft*/

int FWD = 400?;   //FrontWarningDistance                                            /*rontWarningDistance*/
int SWD = 100?;   //SideWarningDistance                                             /*SideWarningDistance*/
//går inte ovanstående och understående att lägga ihop?
//Typ försök hålla ett avstånd till kanten på SWD mm.

int NSWD = 0;  //NoSideWarningDistance                                           /*NoSideWarningDistance*/
int NSD = 300?;   //NoSideDistance                                                  /*NoSideDistance*/

int NSBPT = 0; //NoSideBeginingPresisionTime                                     /*NoSideBeginingPresisionTime*/
int RTT = 0;   //RightTurnTime                                                   /*RightTurnTime*/





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

  //Kan vara bra med en sleep, innan den börjar köra...
  delay(1500);
}

int SetAngle(int myangel)                                                         //done
{
  int SAD = myangel*11+500;
  digitalWrite(ServoPin, HIGH);
  delayMicroseconds(SAD);
  digitalWrite(ServoPin, LOW);
  return SAD;
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
//////////////////////////////////////////////////////
float greenLights(dir)
{
  SetAngle(dir);
  if (ReadUSS() >= 400)

  {
    return true;
  }else{
    return false;
  }
}


void Turn(degrees, relativDir)
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

  delay(10*degrees); //behöver prövas ut hur lång tid det tar att vrida en grad
  
  analogWrite(MR_PWM, 0);
  analogWrite(ML_PWM, 0);
  }
}

void FW()        //FrontWarning                                                    //To do
{
  MSR=MSR;

  digitalWrite(MR_Ctrl, HIGH);
  analogWrite(MR_PWM, 0);
  digitalWrite(ML_Ctrl, HIGH);
  analogWrite(ML_PWM, 0);

  if (greenLights(4) == true)
    {
      Turn(90, Right);
    }

  else if (greenLights(171) == true)
    {
      Turn(90, Left);
    }
  else{
    Turn(180, Right);
    
  }

}

////////////////////////////////////////////////////////77

void SW()                                                                         //If it works then done
{
  digitalWrite(MR_Ctrl, HIGH);
  analogWrite(MR_PWM, MSR +MSAS*(int(readUSS)/SWD));
  digitalWrite(ML_Ctrl, HIGH);
  analogWrite(ML_PWM, MSL);
}

void NSW()                                                                        //If it works then done
{
  digitalWrite(MR_Ctrl, LOW);
  analogWrite(MR_PWM, MSR);
  digitalWrite(ML_Ctrl, LOW);
  analogWrite(ML_PWM, MSL);
  
  while(readUSS>NSWD){delay(NSBPT);}  //makes that it will wait until the distance is bigger then the variable NoSideWarningDistance
  Fd();

  while(readUSS<NSD){delay(3);}       //makes that it will wait until the distance is smaler then the variable NoSideDistance
  digitalWrite(MR_Ctrl, HIGH);
  analogWrite(MR_PWM, RTSR);
  digitalWrite(ML_Ctrl, HIGH);
  analogWrite(ML_PWM, RTSL);
  M = millis;
  while(millis<M+RTT)
  {
    SetAngle(90);
    if(readUSS>FWD)
    {
      Fd();
    }
  }
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
 for(P=75;P<=171;P+=1)
  {
  if(i>105 and i<110){P+=45;}
   SAD = SetAngle(P);
   M = millis();
   while(millis()<M+(40-SAD/1000))
   {
       if(75<P<105)
       {
               if(readUSS<FWD){FW();}
               else{Fd();}
       }
       else if(151<P<171)
       {
               if(readUSS<SWD){SW();}
               else if(readUSS>NSWD){NSW();}
               else{Fd();}
       }
       else{Fd();}
       Serial.println(readUSS());
    }
  }
}