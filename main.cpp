#define ML_Ctrl 4     //define the direction control pin of B motor
#define ML_PWM 5      //define the PWM control pin of B motor
#define MR_Ctrl 2     //define the direction control pin of A motor
#define MR_PWM 9      //define the PWM control pin of A motor
#define USSt 12       //define the UltraSonicSensor trig pin
#define USSe 13       //define the UltraSonicSensor echo pin
#define ServoPin 10
long distance;
int pos=4;
int RAV; //Random Acces Variable


void setup()
{
  Serial.begin(9600);
  pinMode(ML_Ctrl, OUTPUT);//set direction control pin of B motor to OUTPUT
  pinMode(ML_PWM, OUTPUT);//set the PWM control pin of B motor to OUTPUT 
  pinMode(MR_Ctrl, OUTPUT);//set the direction control pin of A motor to OUTPUT
  pinMode(MR_PWM, OUTPUT);//set PWM control pin of A motor to OUTPUT
  pinMode(USSt, OUTPUT);//set trigPin to OUTPUT
  pinMode(USSe, INPUT);//set echoPin to INPUT
  pinMode(ServoPin, OUTPUT);
  SetAngel(4);
}

void SetAngel(int myangel){
  int p = myangel*11+500;
  digitalWrite(ServoPin, HIGH);
  delayMicroseconds(p);
  digitalWrite(ServoPin, LOW);
  return(p);
  
}

long readUSS()
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


void fd()
{
  digitalWrite(MR_Ctrl, HIGH);
  analogWrite(MR_PWM, 85);
  digitalWrite(ML_Ctrl, HIGH);
  analogWrite(ML_PWM, 87);
}

  

// Grader = 4-171

void loop()
{
  for(pos=75;pos<=171;pos+=1){
    SetAngel(pos);
    RAV = millis();
    while(millis()<RAV+(40-p/1000))
    {
        if(pos<105 and pos>75)
        {
          distance = readUSS()
        }
    }
  }
//  Serial.println(distance);
}